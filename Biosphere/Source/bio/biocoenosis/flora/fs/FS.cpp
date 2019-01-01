#include <bio/biocoenosis/flora/fs/FS.hpp>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>
#include <sys/iosupport.h>
#include <sys/param.h>
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

    struct inDevice
    {
        bool setup;
        s32 id;
        devoptab_t device;
        fsp::FileSystem *fs;
        char name[0x20];
    };

    static bool fsinit = false;
    static s32 devdefault = -1;
    static s32 devcwd = -1;
    static inDevice devices[0x20];
    static char cwd[4097] = "/";
    static __thread char fixedpath[4097];

    static inDevice *inFindDevice(const char *name)
    {
        u32 total = (sizeof(devices) / sizeof(inDevice));
        inDevice *dev = NULL;
        if(!fsinit) return NULL;
        if(name && (name[0] == '/'))
        {
            if(devdefault == -1) return NULL;
            dev = &devices[devdefault];
            if(!dev->setup) return NULL;
            return dev;
        }
        for(u32 i = 0; i < total; i++)
        {
            dev = &devices[i];
            if(name == NULL)
            {
                if(!dev->setup) return dev;
            }
            else if(dev->setup)
            {
                if(strncmp(dev->name, name, strlen(dev->name)) == 0) return dev;
            }
        }
        return NULL;
    }

    static const char *inFixPath(struct _reent *r, const char *path, inDevice **dev)
    {
        
    }


    static devoptab_t devoptab =
    {
        .structSize = sizeof(inFile),
        .open_r = [&](struct _reent *r, void *fstruct, const char *path, int flags, int mode)
        {
            fsp::File *fd;

        },
    };
}