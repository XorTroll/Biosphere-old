#include <bio/biocoenosis/fauna/os/Thread.hpp>
#include <bio/biocoenosis/flora/err.hpp>

extern const u8 __tdata_lma[];
extern const u8 __tdata_lma_end[];
extern u8 __tls_start[];
extern u8 __tls_end[];

struct ThreadEntryArgs
{
    bio::os::Thread *t;
    bio::ThreadFunction ent;
    void *arg;
    struct _reent *rnt;
    void *tls;
    void *padd;
};

void bioThreadEntry(ThreadEntryArgs *Args)
{
    bio::os::ThreadVariables *vars = bio::os::Thread::GetThreadVariables();
    vars->Magic = 0x21545624;
    vars->Pointer = Args->t;
    vars->Reent = Args->rnt;
    vars->ThreadLocalSegment = (u8*)Args->tls - 2 * sizeof(void*);
    vars->Handle = Args->t->Handle;
    Args->ent(Args->arg);
    bio::svc::ExitThread();
}

namespace bio::os
{
    Thread::Thread(ThreadFunction Entry, void *Arguments, size_t StackSize, int Priority, int CPUId)
    {
        StackSize = ((StackSize + 0xfff) &~ 0xfff);
        u32 rc = 0;
        size_t reents = ((sizeof(struct _reent) + 0xf) &~ 0xf);
        size_t tlss = ((__tls_end - __tls_start + 0xf) &~ 0xf);
        void *stack = memalign(0x1000, StackSize + reents + tlss);
        if(stack == NULL) bio::err::Throw(bio::ResultOutOfMemory);
        else
        {
            void *stackm = VirtualMemory::ReserveMap(StackSize);
            rc = svc::MapMemory(stackm, stack, StackSize);
            if(rc == 0)
            {
                u64 stackt = ((u64)stackm) + StackSize - sizeof(ThreadEntryArgs);
                ThreadEntryArgs *args = (ThreadEntryArgs*)stackt;
                u32 hdl = 0;
                rc = svc::CreateThread(&hdl, (void*)&bioThreadEntry, args, (void*)stackt, Priority, CPUId);
                if(rc == 0)
                {
                    this->Handle = hdl;
                    this->StackMemory = stack;
                    this->StackMemoryMirror = stackm;
                    this->StackSize = StackSize;
                    args->t = this;
                    args->ent = Entry;
                    args->arg = Arguments;
                    args->rnt = (struct _reent*)((u8*)stack + StackSize);
                    args->tls = (u8*)stack + StackSize + reents;
                    _REENT_INIT_PTR(args->rnt);
                    struct _reent *cur = Thread::GetThreadVariables()->Reent;
                    args->rnt->_stdin  = cur->_stdin;
                    args->rnt->_stdout = cur->_stdout;
                    args->rnt->_stderr = cur->_stderr;
                    size_t tlsloads = __tdata_lma_end - __tdata_lma;
                    size_t tlsbsss = tlss - tlsloads;
                    if(tlsloads) memcpy(args->tls, __tdata_lma, tlsloads);
                    if(tlsbsss) memset(args->tls + tlsloads, 0, tlsbsss);
                }
                else svc::UnmapMemory(stackm, stack, StackSize);
            }
            else
            {
                VirtualMemory::FreeMap(stackm, StackSize);
                free(stack);
            }
        }
    }

    Thread::~Thread()
    {
        this->Close();
    }

    Result Thread::Start()
    {
        return svc::StartThread(this->Handle);
    }

    Result Thread::WaitForExit()
    {
        s32 temp = 0;
        return svc::WaitSynchronization(&temp, &this->Handle, 1, -1);
    }

    Result Thread::Close()
    {
        u32 rc = svc::UnmapMemory(this->StackMemoryMirror, this->StackMemory, this->StackSize);
        VirtualMemory::FreeMap(this->StackMemoryMirror, this->StackSize);
        free(this->StackMemory);
        svc::CloseHandle(this->Handle);
        return rc;
    }

    Result Thread::Pause()
    {
        return svc::SetThreadActivity(this->Handle, 1);
    }

    Result Thread::Resume()
    {
        return svc::SetThreadActivity(this->Handle, 0);
    }

    ThreadVariables *Thread::GetThreadVariables()
    {
        return (ThreadVariables*)((u8*)(arm::GetThreadLocalStorage()) + 0x1e0);
    }
}