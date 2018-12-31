#include <bio/biotope/init/Initialize.hpp>
#include <bio/biocoenosis/fauna/os/Memory.hpp>
#include <bio/biocoenosis/flora/diag.hpp>
#include <bio/root/Types.hpp>
#include <vector>
#include <elf.h>
#include <cstdlib>
#include <ctime>
#include <cerrno>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <sys/reent.h>
#include <sys/lock.h>
#include <sys/time.h>
#include <sys/iosupport.h>

typedef void BIO_NORETURN (*LoaderReturn)(int Res);

extern char *fake_heap_start;
extern char *fake_heap_end;
extern const u8 __tdata_lma[];
extern const u8 __tdata_lma_end[];
extern u8 __tls_start[];
extern u32 argdata;
extern std::vector<std::pair<u64, u32>> overrides;

BIO_WEAK alignas(16) u8 excstack[0x400];
BIO_WEAK u64 excstsize = sizeof(excstack);
BIO_WEAK u32 excignoredebug = 0;
bio::arm::ThreadExceptionDump excdump;

LoaderReturn retaddr = NULL;
u32 mainth = 0;
u64 hintsvc[2] = { 0 };
bool lnso = false;
char *nextpath = NULL;
char *nextargv = NULL;
void *heapaddr = NULL;
size_t heapsze = 0;
void *stackt = NULL;
u64 heapsize = 0;
void *oargv = NULL;
bio::os::VirtualRegion addrspace;
bio::os::VirtualRegion regs[4];
bio::Application apptype = bio::Application::None;
u32 prochandle = 0;
u32 lastres = 0;
int __argc = 0;
char **__argv = NULL;
BIO_WEAK size_t vheapsize = 0;
u64 btime = 0;
u64 bticks = 0;
bool hasrseed = false;
u64 rseed[2] = { 0, 0 };

extern "C"
{
    extern void __libc_init_array(void);
    extern void __libc_fini_array(void);
    void BIO_WEAK BIO_NORETURN bioQuickExit(int Code);
    void BIO_NORETURN bioQuickPostExit(u32 Code, LoaderReturn Addr);
}

extern void bioMain();

namespace bio::init
{
    Result InitializeHomebrewEnvironment(void *Context, u32 MainThread, void *SavedLoader)
    {
        if(SavedLoader == NULL) retaddr = (LoaderReturn)&svc::ExitProcess;
        else retaddr = (LoaderReturn)SavedLoader;
        if((int)MainThread != -1)
        {
            mainth = MainThread;
            lnso = true;
            hintsvc[0] = hintsvc[1] = -1ull;
        }
        else
        {
            HomebrewContext *hctx = (HomebrewContext*)Context;
            while(hctx->Key != 0)
            {
                switch(hctx->Key)
                {
                    case 1:
                        mainth = hctx->Value[0];
                        break;
                    case 2:
                        nextpath = (char*)hctx->Value[0];
                        nextargv = (char*)hctx->Value[1];
                        break;
                    case 3:
                        heapaddr = (void*)hctx->Value[0];
                        heapsize = hctx->Value[1];
                        break;
                    case 4:
                        overrides.push_back(std::pair<u64, u32>(hctx->Value[0], (u32)hctx->Value[1]));
                        break;
                    case 5:
                        oargv = (void*)hctx->Value[0];
                        break;
                    case 6:
                        hintsvc[0] = hctx->Value[0];
                        hintsvc[1] = hctx->Value[1];
                        break;
                    case 7:
                        apptype = (bio::Application)hctx->Value[0];
                        if((hctx->Value[1] & BIO_BITMASK(0)) && apptype == bio::Application::SystemApplication) apptype = bio::Application::Application;
                        break;
                    case 10:
                        prochandle = hctx->Value[0];
                        break;
                    case 11:
                        lastres = hctx->Value[0];
                        break;
                    case 14:
                        hasrseed = true;
                        rseed[0] = hctx->Value[0];
                        rseed[1] = hctx->Value[1];
                        break;
                    default:
                        if(hctx->Flags & BIO_BITMASK(0))
                        {
                            bio::Result rc(346, 100 + hctx->Key);
                            u32 rcode = rc;
                            bioQuickPostExit(rcode, retaddr);
                        }
                        break;
                }
                hctx++;
            }
        }
        return 0;
    }

    Result InitializeStandardSysCalls()
    {
        __syscalls.exit = bioQuickExit;
        __syscalls.gettod_r = [](struct _reent *Reent, struct timeval *Time, struct timezone *Zone) -> int
        {
            if(Time != NULL)
            {
                if(btime == UINT64_MAX)
                {
                    Reent->_errno = EIO;
                    return -1;
                }
                u64 now = bio::svc::GetSystemTick() - bticks;
                u64 bsecs = now / 19200000ull;
                Time->tv_sec = bsecs + btime;
                Time->tv_usec = (now - bsecs * 19200000ull) * 10ull / 192ull;
            }
            if(Zone != NULL)
            {
                Zone->tz_minuteswest = 0;
                Zone->tz_dsttime = 0;
            }
            return 0;
        };
        __syscalls.getreent = []() -> struct _reent*
        {
            bio::os::ThreadVariables *v = bio::os::Thread::GetThreadVariables();
            if(v->Magic != 0x21545624)
            {
                if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
                bio::diag::AssertResultOk(bio::ResultWrongReentStruct);
            }
            return v->Reent;
        };
        __syscalls.clock_gettime = [](clockid_t ClockID, struct timespec *Spec) -> int
        {
            if(ClockID != CLOCK_MONOTONIC && ClockID != CLOCK_REALTIME)
            {
                errno = EINVAL;
                return -1;
            }
            if(Spec)
            {
                if(btime == UINT64_MAX)
                {
                    errno = EIO;
                    return -1;
                }
                u64 now = bio::svc::GetSystemTick() - bticks;
                u64 bsecs = now / 19200000ull;
                Spec->tv_sec = bsecs + btime;
                u64 nsecs = (now -  bsecs * 19200000ULL) * 10000ULL / 192ULL;
                Spec->tv_nsec = nsecs - nsecs % (1000000000ULL / 19200000ULL);
                return 0;
            }
            else
            {
                errno = EINVAL;
                return -1;
            }
        };
        __syscalls.clock_getres = [](clockid_t ClockID, struct timespec *Spec) -> int
        {
            if(ClockID != CLOCK_MONOTONIC && ClockID != CLOCK_REALTIME)
            {
                errno = EINVAL;
                return -1;
            }
            if(Spec)
            {
                Spec->tv_sec = 0;
                Spec->tv_nsec = (1000000000ULL / 19200000ULL);
                return 0;
            }
            else
            {
                errno = EFAULT;
                return -1;
            }
        };
        __syscalls.clock_settime = [](clockid_t ClockID, const struct timespec *Spec) -> int
        {
            errno = ENOSYS;
            return -1;
        };
        __syscalls.nanosleep = [](const struct timespec *Request, struct timespec *Remaining) -> int
        {
            svc::SleepThread(Request->tv_sec * 1000000000ull + Request->tv_nsec);
            return 0;
        };
        __syscalls.lock_init = [](_LOCK_T *Lock)
        {
            *Lock = 0;
        };
        __syscalls.lock_acquire = [](_LOCK_T *Lock)
        {
            u32 self = bio::os::Thread::GetThreadVariables()->Handle;
            while(true)
            {
                u32 cur = __sync_val_compare_and_swap((u32*)Lock, 0, self);
                if(cur == 0) return;
                if((cur &~ 0x40000000) == self) return;
                if(cur & 0x40000000) svc::ArbitrateLock(cur &~ 0x40000000, (u32*)Lock, self);
                else
                {
                    u32 old = __sync_val_compare_and_swap((u32*)Lock, cur, cur | 0x40000000);
                    if(old == cur) svc::ArbitrateLock(cur, (u32*)Lock, self);
                }
            }
        };
        __syscalls.lock_release = [](_LOCK_T *Lock)
        {
            u32 old = __sync_val_compare_and_swap((u32*)Lock, bio::os::Thread::GetThreadVariables()->Handle, 0);
            if(old & 0x40000000) svc::ArbitrateUnlock((u32*)Lock);
        };
        __syscalls.lock_init_recursive = [](_LOCK_RECURSIVE_T *RLock)
        {
            RLock->lock = 0;
            RLock->thread_tag = 0;
            RLock->counter = 0;
        };
        __syscalls.lock_acquire_recursive = [](_LOCK_RECURSIVE_T *RLock)
        {
            if(RLock->thread_tag != bio::os::Thread::GetThreadVariables()->Handle)
            {
                u32 self = bio::os::Thread::GetThreadVariables()->Handle;
                while(true)
                {
                    u32 cur = __sync_val_compare_and_swap((u32*)&RLock->lock, 0, self);
                    if(cur == 0) return;
                    if((cur &~ 0x40000000) == self) return;
                    if(cur & 0x40000000) svc::ArbitrateLock(cur &~ 0x40000000, (u32*)&RLock->lock, self);
                    else
                    {
                        u32 old = __sync_val_compare_and_swap((u32*)&RLock->lock, cur, cur | 0x40000000);
                        if(old == cur) svc::ArbitrateLock(cur, (u32*)&RLock->lock, self);
                    }
                }
                RLock->thread_tag = bio::os::Thread::GetThreadVariables()->Handle;
            }
            RLock->counter++;
        };
        __syscalls.lock_release_recursive = [](_LOCK_RECURSIVE_T *RLock)
        {
            if(--RLock->counter == 0)
            {
                RLock->thread_tag = 0;
                u32 old = __sync_val_compare_and_swap((u32*)&RLock->lock, bio::os::Thread::GetThreadVariables()->Handle, 0);
                if(old & 0x40000000) svc::ArbitrateUnlock((u32*)&RLock->lock);
            }
        };
        bio::os::ThreadVariables *v = bio::os::Thread::GetThreadVariables();
        v->Magic = 0x21545624;
        v->Pointer = NULL;
        v->Reent = _impure_ptr;
        v->ThreadLocalSegment = __tls_start - (2 * sizeof(void*));
        v->Handle = mainth;
        u32 tlss = (__tdata_lma_end - __tdata_lma);
        if(tlss) memcpy(__tls_start, __tdata_lma, tlss);
        return 0;
    }

    Result InitializeStandardFileSystem()
    {
        // TODO: Implement filesystem devoptabs for newlib
        return 0;
    }

    Result InitializeStandardConstructors()
    {
        __libc_init_array();
        return 0;
    }
    
    Result InitializeKernel()
    {
        u64 base = 0;
        u64 size = 0;
        u32 rc = svc::GetInfo(&base, 12, bio::os::GetCurrentProcessHandle(), 0);
        if(rc == 0)
        {
            rc = svc::GetInfo(&size, 13, bio::os::GetCurrentProcessHandle(), 0);
            if(rc == 0)
            {
                addrspace.Start = base;
                addrspace.End = base + size;
            }
        }
        if(rc != 0)
        {
            rc = svc::UnmapMemory((void*)0xFFFFFFFFFFFFE000ULL, (void*)0xFFFFFE000ull, 0x1000);
            if(rc == 0xd401)
            {
                addrspace.Start = 0x200000ull;
                addrspace.End = 0x100000000ull;
            }
            else if(rc == 0xdc01)
            {
                addrspace.Start = 0x8000000ull;
                addrspace.End = 0x100000000ull;
            }
            else
            {
                if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
                bio::diag::AssertResultOk(bio::ResultWeirdKernelInformation);
            }
        }
        rc = svc::GetInfo(&base, 2, bio::os::GetCurrentProcessHandle(), 0);
        if(rc == 0)
        {
            rc = svc::GetInfo(&size, 3, bio::os::GetCurrentProcessHandle(), 0);
            if(rc == 0)
            {
                regs[static_cast<u32>(bio::os::Region::Stack)].Start = base;
                regs[static_cast<u32>(bio::os::Region::Stack)].End = base + size;
            }
        }
        if(rc != 0)
        {
            if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
            bio::diag::AssertResultOk(bio::ResultRequestStackInformation);
        }
        rc = svc::GetInfo(&base, 4, bio::os::GetCurrentProcessHandle(), 0);
        if(rc == 0)
        {
            rc = svc::GetInfo(&size, 5, bio::os::GetCurrentProcessHandle(), 0);
            if(rc == 0)
            {
                regs[static_cast<u32>(bio::os::Region::Heap)].Start = base;
                regs[static_cast<u32>(bio::os::Region::Heap)].End = base + size;
            }
        }
        if(rc != 0)
        {
            if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
            bio::diag::AssertResultOk(bio::ResultRequestHeapInformation);
        }
        rc = svc::GetInfo(&base, 14, bio::os::GetCurrentProcessHandle(), 0);
        if(rc == 0)
        {
            rc = svc::GetInfo(&size, 15, bio::os::GetCurrentProcessHandle(), 0);
            if(rc == 0)
            {
                regs[static_cast<u32>(bio::os::Region::NewStack)].Start = base;
                regs[static_cast<u32>(bio::os::Region::NewStack)].End = base + size;
            }
        }
        void *addr = NULL;
        size_t amem = 0;
        size_t umem = 0;
        if(heapaddr != NULL)
        {
            addr = heapaddr;
            heapsze = heapsize;
        }
        else
        {
            if(vheapsize == 0)
            {
                svc::GetInfo(&amem, 6, bio::os::GetCurrentProcessHandle(), 0);
                svc::GetInfo(&umem, 7, bio::os::GetCurrentProcessHandle(), 0);
                if(amem > umem + 0x200000) heapsze = (amem - umem - 0x200000) & ~0x1fffff;
                if(heapsze == 0) heapsze = (0x2000000 * 16);
            }
            else heapsze = vheapsize;
            rc = svc::SetHeapSize(&addr, heapsze);
            if(rc != 0)
            {
                if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
                bio::diag::AssertResultOk(bio::ResultHeapAllocation);
            }
        }
        fake_heap_start = (char*)addr;
        fake_heap_end = (char*)addr + heapsze;
        return rc;
    }

    Result InitializeArguments()
    {
        u32 rc = 0;
        bio::MemoryInfo minfo;
        u32 pinfo = 0;
        u8 *argd = (u8*)&argdata;
        u32 *arg32 = (u32*)argd;
        u64 argdata_allocs = 0;
        u64 argdata_strs = 0;
        u32 argvptr_pos = 0;
        u32 max_argv = 0;
        u32 argi = 0;
        u32 arglen = 0;
        bool quotef = 0;
        bool endf = 0;
        char *args = NULL;
        char *argstart = NULL;
        char *argstorage = NULL;
        __argc = 0;
        char *empty = NULL;
        __argv = &empty;
        bool cont = true;
        if(lnso)
        {
            memset(&minfo, 0, sizeof(minfo));
            rc = svc::QueryMemory(&minfo, &pinfo, (u64)argd);
            if((rc != 0) || (minfo.Permissions != 0x3)) cont = false;
            if(cont)
            {
                argdata_allocs = (u64)arg32[0];
                argdata_strs = (u64)arg32[1];
                args = (char*)&argd[0x20];
                if((argdata_allocs == 0) || (argdata_strs == 0)) cont = false;
                if(cont)
                {
                    if((u64)argd < minfo.Address) cont = false;
                    if(cont)
                    {
                        if((((u64)argd - minfo.Address) + argdata_allocs) > minfo.Size) cont = false;
                        if(cont)
                        {
                            argvptr_pos = (argdata_strs + 0x20 + 1);
                            if(argvptr_pos >= argdata_allocs) cont = false;
                        }
                    }
                }
            }
        }
        else
        {
            if(oargv != NULL)
            {
                args = (char*)oargv;
                argdata_allocs = 0x9000;
                argdata_strs = strlen(args);
                argd = (u8*)fake_heap_start;
                if(argdata_strs == 0) cont = false;
                if(cont)
                {
                    if((u64)(fake_heap_end - fake_heap_start) < argdata_allocs) cont = false;
                    if(cont)
                    {
                        fake_heap_start += argdata_allocs;
                        argvptr_pos = 0;
                        memset(argd, 0, argdata_allocs);
                    }
                }
            }
        }
        if(cont)
        {
            argstorage = (char*)&argd[argvptr_pos];
            argvptr_pos += ((argdata_strs + 0x9 + 1) & ~0x7);
            if(argvptr_pos >= argdata_allocs) cont = false;
            if(cont)
            {
                max_argv = ((argdata_allocs - argvptr_pos) >> 3);
                if(max_argv < 2) cont = false;
                if(cont)
                {
                    __argv = (char**)&argd[argvptr_pos];
                    argstart = NULL;
                    for(argi = 0; argi < argdata_strs; argi++)
                    {
                        if(argstart == NULL && isspace(args[argi])) continue;
                        if(argstart == NULL)
                        {
                            if(args[argi] == '"')
                            {
                                argstart = &args[argi + 1];
                                quotef = 1;
                            }
                            else if(args[argi] != 0)
                            {
                                argstart = &args[argi];
                                arglen++;
                            }
                        }
                        else
                        {
                            endf = 0;
                            if(quotef && args[argi] == '"') endf = 1;
                            else if(isspace(args[argi])) endf = 1;
                            if(endf == 0 && args[argi] != 0) arglen++;
                            if((args[argi] == 0 || endf) && arglen)
                            {
                                strncpy(argstorage, argstart, arglen);
                                argstorage[arglen] = 0;
                                __argv[__argc] = argstorage;
                                __argc++;
                                argstart = NULL;
                                quotef = 0;
                                argstorage += arglen + 1;
                                arglen = 0;
                                if((u32)__argc >= max_argv) break;
                            }
                        }
                    }
                    if(arglen && ((u32)__argc < max_argv))
                    {
                        strncpy(argstorage, argstart, arglen);
                        argstorage[arglen] = 0;
                        __argv[__argc] = argstorage;
                        __argc++;
                    }
                    __argv[__argc] = NULL;
                }
            }
        }
        btime = UINT64_MAX;
        bticks = svc::GetSystemTick();
        return rc;    
    }
}

extern "C"
{
    void BIO_WEAK bioInitStartup(void *Context, u32 MainThread, void *SavedLoader)
    {
        bio::init::InitializeHomebrewEnvironment(Context, MainThread, SavedLoader);
        bio::init::InitializeStandardSysCalls();
        bio::init::InitializeKernel();
        bio::init::InitializeArguments();
        bio::init::InitializeStandardConstructors();
    }

    void BIO_WEAK BIO_NORETURN bioQuickExit(int Code)
    {
        __libc_fini_array();
        bioQuickPostExit(Code, retaddr);
    }

    void bioInitDynamic(uintptr_t base, const Elf64_Dyn *dyn)
    {
        const Elf64_Rela *rela = NULL;
        u64 relasz = 0;
        for(; dyn->d_tag != DT_NULL; dyn++)
        {
            switch(dyn->d_tag)
            {
                case DT_RELA:
                    rela = (const Elf64_Rela*)(base + dyn->d_un.d_ptr);
                    break;
                case DT_RELASZ:
                    relasz = dyn->d_un.d_val / sizeof(Elf64_Rela);
                    break;
            }
        }
        if(rela == NULL)
        {
            if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
            bio::diag::AssertResultOk(bio::ResultWrongRelocate);
        }
        for(; relasz--; rela++)
        {
            switch(ELF64_R_TYPE(rela->r_info))
            {
                case R_AARCH64_RELATIVE:
                {
                    u64 *ptr = (u64*)(base + rela->r_offset);
                    *ptr = base + rela->r_addend;
                    break;
                }
            }
        }
    }
}

int main()
{
    return bio::Main();
}