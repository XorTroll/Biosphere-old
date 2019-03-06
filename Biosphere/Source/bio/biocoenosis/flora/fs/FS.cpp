#include <bio/biocoenosis/flora/fs/FS.hpp>
#include <bio/biocoenosis/fauna/utf.hpp>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace bio::fs
{
    struct inFile
    {
        fsp::File *fd;
        int flags;
        u64 offset;
    };

    struct inDirectory
    {
        u32 magic;
        fsp::Directory *fd;
        ssize_t idx;
        size_t size;
        fsp::DirectoryEntry data[0x20];
    };

    static bool fsinit = false;
    static s32 devdefault = -1;
    static s32 devcwd = -1;
    static std::vector<Device*> devices;
    static char cwd[4097] = "/";
    static __thread char fixedpath[4097];
    static fsp::FspService *sfsp;

    static int inFS_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
    static int inFS_close(struct _reent *r, void *fd);
    static ssize_t inFS_write(struct _reent *r, void *fd, const char *ptr, size_t len);
    static ssize_t inFS_write_safe(struct _reent *r, void *fd, const char *ptr, size_t len);
    static ssize_t inFS_read(struct _reent *r, void *fd, char *ptr, size_t len);
    static ssize_t inFS_read_safe(struct _reent *r, void *fd, char *ptr, size_t len);
    static off_t inFS_seek(struct _reent *r, void *fd, off_t pos, int dir);
    static int inFS_fstat(struct _reent *r, void *fd, struct stat *st);
    static int inFS_stat(struct _reent *r, const char *file, struct stat *st);
    static int inFS_link(struct _reent *r, const char *existing, const char *newLink);
    static int inFS_unlink(struct _reent *r, const char *name);
    static int inFS_chdir(struct _reent *r, const char *name);
    static int inFS_rename(struct _reent *r, const char *oldName, const char *newName);
    static int inFS_mkdir(struct _reent *r, const char *path, int mode);
    static DIR_ITER *inFS_diropen(struct _reent *r, DIR_ITER *dirState, const char *path);
    static int inFS_dirreset(struct _reent *r, DIR_ITER *dirState);
    static int inFS_dirnext(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat);
    static int inFS_dirclose(struct _reent *r, DIR_ITER *dirState);
    static int inFS_statvfs(struct _reent *r, const char *path, struct statvfs *buf);
    static int inFS_ftruncate(struct _reent *r, void *fd, off_t len);
    static int inFS_fsync(struct _reent *r, void *fd);
    static int inFS_chmod(struct _reent *r, const char *path, mode_t mode);
    static int inFS_fchmod(struct _reent *r, void *fd, mode_t mode);
    static int inFS_rmdir(struct _reent *r, const char *name);

    static devoptab_t newlibTab =
    {
        .structSize = sizeof(inFile),
        .open_r = inFS_open,
        .close_r = inFS_close,
        .write_r = inFS_write,
        .read_r = inFS_read,
        .seek_r = inFS_seek,
        .fstat_r = inFS_fstat,
        .stat_r = inFS_stat,
        .link_r = inFS_link,
        .unlink_r = inFS_unlink,
        .chdir_r = inFS_chdir,
        .rename_r = inFS_rename,
        .mkdir_r = inFS_mkdir,
        .dirStateSize = sizeof(inDirectory),
        .diropen_r = inFS_diropen,
        .dirreset_r = inFS_dirreset,
        .dirnext_r = inFS_dirnext,
        .dirclose_r = inFS_dirclose,
        .statvfs_r = inFS_statvfs,
        .ftruncate_r = inFS_ftruncate,
        .fsync_r = inFS_fsync,
        .deviceData = 0,
        .chmod_r = inFS_chmod,
        .fchmod_r = inFS_fchmod,
        .rmdir_r = inFS_rmdir,
    };

    Device *FindDevice(const char *Name)
    {
        u32 total = devices.size();
        Device *dev = NULL;
        if(!fsinit) return NULL;
        if(Name && (Name[0] == '/'))
        {
            if(devdefault == -1) return NULL;
            dev = devices[devdefault];
            if(!dev->Valid) return NULL;
            return dev;
        }
        for(u32 i = 0; i < total; i++)
        {
            dev = devices[i];
            if(Name == NULL)
            {
                if(!dev->Valid) return dev;
            }
            else if(dev->Valid)
            {
                if(strncmp(dev->Name, Name, strlen(dev->Name)) == 0) return dev;
            }
        }
        return NULL;
    }

    static const char *inFixPath(struct _reent *r, const char *path, Device **dev)
    {
        ssize_t units = 0;
        uint32_t code;
        const uint8_t *p = (const uint8_t*)path;
        const char *device_path = path;
        do
        {
            units = utf::DecodeUtf8(p, &code);
            if(units < 0)
            {
                r->_errno = EILSEQ;
                return NULL;
            }
            p += units;
        } while(code != ':' && code != 0);
        if(code == ':') path = (const char*)p;
        p = (const uint8_t*)path;
        do
        {
            units = utf::DecodeUtf8(p, &code);
            if(units < 0)
            {
                r->_errno = EILSEQ;
                return NULL;
            }
            if(code == ':')
            {
                r->_errno = EINVAL;
                return NULL;
            }
            p += units;
        } while(code != 0);
        if(path[0] == '/') strncpy(fixedpath, path, PATH_MAX);
        else
        {
            strncpy(fixedpath, cwd, PATH_MAX);
            fixedpath[PATH_MAX] = '\0';
            strncat(fixedpath, path, PATH_MAX - strlen(cwd));
        }
        if(fixedpath[PATH_MAX] != 0)
        {
            fixedpath[PATH_MAX] = 0;
            r->_errno = ENAMETOOLONG;
            return NULL;
        }
        if(dev)
        {
            if(path[0] == '/') *dev = FindDevice(device_path);
            else
            {
                *dev = NULL;
                if(devcwd != -1) *dev = devices[devcwd];
            }
            if(*dev == NULL)
            {
                r->_errno = ENODEV;
                return NULL;
            }
        }
        return fixedpath;
    }

    static int inGetFSPath(struct _reent *r, const char *path, Device **dev, char *out)
    {
        if(inFixPath(r, path, dev) == NULL) return -1;
        memcpy(out, fixedpath, 0x300);
        out[0x300] = '\0';
        return 0;
    }

    static ssize_t inConvertFromFSPath(u8 *out, u8 *in, size_t len)
    {
        strncpy((char*)out, (char*)in, len);
        return strnlen((char*)out, len);
    }

    void Initialize()
    {
        u32 total = 256;
        if(!fsinit)
        {
            sfsp = fsp::Initialize().AssertOk();
            devices.reserve(total);
            for(u32 i = 0; i < total; i++)
            {
                Device *dev = (Device*)malloc(sizeof(Device));
                memcpy(&dev->InternalDevice, &newlibTab, sizeof(newlibTab));
                dev->InternalDevice.name = dev->Name;
                dev->Id = i;
                devices.push_back(dev);
            }
            devdefault = -1;
            fsinit = true;
        }
    }

    void Finalize()
    {
        UnmountAllDevices();
        for(u32 i = 0; i < devices.size(); i++) free(devices[i]);
        devices.clear();
        delete sfsp;
        sfsp = NULL;
        fsinit = false;
    }

    static int inMountDevice(const char *dname, fsp::FileSystem *ifs, Device **out)
    {
        Device *dev = NULL;
        if(FindDevice(dname)) return -1;
        if(!fsinit) return -1;
        dev = FindDevice(NULL);
        if(dev == NULL) return -1;
        dev->Session = ifs;
        memset(dev->Name, 0, sizeof(dev->Name));
        strncpy(dev->Name, dname, sizeof(dev->Name) - 1);
        int rdev = AddDevice(&dev->InternalDevice);
        if(rdev == -1) return rdev;
        dev->Valid = true;
        if(out) *out = dev;
        return rdev;
    }

    static int inDeviceDispose(Device *dev, bool close)
    {
        char name[0x22];
        if(!dev->Valid) return 0;
        memset(name, 0, sizeof(name));
        strncpy(name, dev->Name, sizeof(name) - 2);
        strncat(name, ":", sizeof(name) - strlen(name) - 1);
        RemoveDevice(name);
        if(close) dev->Session->Close();
        if(dev->Id == devdefault) devdefault = -1;
        if(dev->Id == devcwd) devcwd = devdefault;
        dev->Valid = false;
        memset(dev->Name, 0, sizeof(dev->Name));
        return 0;
    }

    Result Mount(fsp::FileSystem *FS, const char *DeviceName)
    {
        return inMountDevice(DeviceName, FS, NULL);
    }

    Result MountSdCard(const char *DeviceName)
    {
        fsp::FileSystem *sdfs = sfsp->OpenSdCardFileSystem().AssertOk();
        return Mount(sdfs, DeviceName);
    }

    void Unregister(const char *DeviceName)
    {
        Device *dev = FindDevice(DeviceName);
        if(dev != NULL) inDeviceDispose(dev, false);
    }

    void Unmount(const char *DeviceName)
    {
        Device *dev = FindDevice(DeviceName);
        if(dev != NULL) inDeviceDispose(dev, true);
    }

    fsp::FileSystem *GetFileSystem(const char *DeviceName)
    {
        Device *dev = FindDevice(DeviceName);
        if(dev == NULL) return NULL;
        return dev->Session;
    }

    void UnmountAllDevices()
    {
        u32 total = devices.size();
        if(fsinit) for(u32 i = 0; i < total; i++) inDeviceDispose(devices[i], true);
    }

    static int inFSGetErrno(Result FsError)
    {
        int rc = EIO;
        if(FsError == fsp::ResultPathDoesntExist) rc = ENOENT;
        else if(FsError == fsp::ResultPathAlreadyExists) rc = EEXIST;
        else if(FsError == fsp::ResultInvalidInput) rc = EINVAL;
        else if(FsError == fsp::ResultPathTooLong) rc = ENAMETOOLONG;
        else if(FsError == fsp::ResultPathAlreadyExists) rc = EEXIST;
        return rc;
    }

    static int inFS_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode)
    {
        fsp::File *fd;
        Result rc = 0;
        u32 dflags = 0;
        u32 attrs = 0;
        char fpath[4096];
        Device *dev = NULL;
        if(inGetFSPath(r, path, &dev, fpath) == -1) return -1;
        inFile *inf = (inFile*)fileStruct;
        switch(flags & O_ACCMODE)
        {
            case O_RDONLY:
                dflags |= BIO_BITMASK(0);
                if(flags & O_APPEND)
                {
                    r->_errno = EINVAL;
                    return -1;
                }
                break;
            case O_WRONLY:
                // FINISH THIS!!
                break;
        }
        return -1;
    }

    static int inFS_close(struct _reent *r, void *fd)
    {
        return -1;
    }

    static ssize_t inFS_write(struct _reent *r, void *fd, const char *ptr, size_t len)
    {
        return -1;
    }

    static ssize_t inFS_write_safe(struct _reent *r, void *fd, const char *ptr, size_t len)
    {
        return -1;
    }

    static ssize_t inFS_read(struct _reent *r, void *fd, char *ptr, size_t len)
    {
        return -1;
    }
    
    static ssize_t inFS_read_safe(struct _reent *r, void *fd, char *ptr, size_t len)
    {
        return -1;
    }

    static off_t inFS_seek(struct _reent *r, void *fd, off_t pos, int dir)
    {
        return -1;
    }
    
    static int inFS_fstat(struct _reent *r, void *fd, struct stat *st)
    {
        return -1;
    }
    
    static int inFS_stat(struct _reent *r, const char *file, struct stat *st)
    {
        return -1;
    }
    
    static int inFS_link(struct _reent *r, const char *existing, const char *newLink)
    {
        return -1;
    }
    
    static int inFS_unlink(struct _reent *r, const char *name)
    {
        return -1;
    }
    
    static int inFS_chdir(struct _reent *r, const char *name)
    {
        return -1;
    }
    
    static int inFS_rename(struct _reent *r, const char *oldName, const char *newName)
    {
        return -1;
    }
    
    static int inFS_mkdir(struct _reent *r, const char *path, int mode)
    {
        return -1;
    }
    
    static DIR_ITER *inFS_diropen(struct _reent *r, DIR_ITER *dirState, const char *path)
    {
        return NULL;
    }
    
    static int inFS_dirreset(struct _reent *r, DIR_ITER *dirState)
    {
        return -1;
    }
    
    static int inFS_dirnext(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat)
    {
        return -1;
    }
    
    static int inFS_dirclose(struct _reent *r, DIR_ITER *dirState)
    {
        return -1;
    }
    
    static int inFS_statvfs(struct _reent *r, const char *path, struct statvfs *buf)
    {
        return -1;
    }

    static int inFS_ftruncate(struct _reent *r, void *fd, off_t len)
    {
        inFile *inf = (inFile*)fd;
        if(len < 0)
        {
            r->_errno = EINVAL;
            return -1;
        }
        Result rc = inf->fd->SetSize(len);
        if(rc.IsSuccess()) return 0;
        r->_errno = inFSGetErrno(rc);
        return -1;
    }

    static int inFS_fsync(struct _reent *r, void *fd)
    {
        inFile *inf = (inFile*)fd;
        Result rc = inf->fd->Flush();
        if(rc.IsSuccess()) return 0;
        r->_errno = inFSGetErrno(rc);
        return -1;
    }

    static int inFS_chmod(struct _reent *r, const char *path, mode_t mode)
    {
        r->_errno = ENOSYS;
        return -1;
    }

    static int inFS_fchmod(struct _reent *r, void *fd, mode_t mode)
    {
        r->_errno = ENOSYS;
        return -1;
    }

    static int inFS_rmdir(struct _reent *r, const char *name)
    {
        Result rc = 0;
        char fpath[0x301];
        Device *dev = NULL;
        if(inGetFSPath(r, name, &dev, fpath) == -1) return -1;
        rc = dev->Session->DeleteDirectory(fpath);
        if(rc.IsSuccess()) return 0;
        r->_errno = inFSGetErrno(rc);
        return -1;
    }
}