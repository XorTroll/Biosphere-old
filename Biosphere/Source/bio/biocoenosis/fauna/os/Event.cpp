#include <bio/biocoenosis/fauna/os/Event.hpp>

namespace bio::os
{
    Event::Event(bool AutoClear)
    {
        this->cauto = AutoClear;
        this->whandle = 0;
        this->rhandle = 0;
        u32 w = 0;
        u32 r = 0;
        Result rc = svc::CreateEvent(&w, &r);
        if(rc.IsSuccess())
        {
            this->whandle = w;
            this->rhandle = r;
        }
    }

    Event::Event(u32 Handle, bool AutoClear)
    {
        this->cauto = AutoClear;
        this->whandle = 0;
        this->rhandle = Handle;
    }

    Event::~Event()
    {
        this->Close();
    }

    bool Event::IsValid()
    {
        return (this->rhandle != 0);
    }

    bool Event::AutoClears()
    {
        return this->cauto;
    }

    void Event::SetAutoClear(bool AutoClear)
    {
        this->cauto = AutoClear;
    }

    Result Event::Wait(u64 Timeout)
    {
        if(this->rhandle == 0) return ResultInvalidHandle;
        Result rc;
        u64 deadline = 0;
        if(Timeout != UINT64_MAX) deadline = arm::GetSystemTick() + ((Timeout * 12) / 625);
        do
        {
            do
            {
                s64 stime = -1;
                if(deadline)
                {
                    stime = deadline - arm::GetSystemTick();
                    stime = (((stime >= 0) ? stime : 0) * 625) / 12;
                }
                s32 svctmp = 0;
                rc = svc::WaitSynchronization(&svctmp, &this->rhandle, 1, stime);
                if(deadline && ((rc & 0x3fffff) == 0xea01)) return rc;
            } while(rc.IsFailure());
            if(this->cauto) svc::SignalEvent(this->rhandle);
        } while((rc & 0x3fffff) == 0xfa01);
        return rc;
    }

    Result Event::Fire()
    {
        if(this->rhandle == 0) return ResultInvalidHandle;
        return svc::SignalEvent(this->rhandle);
    }

    Result Event::Clear()
    {
        if(this->whandle != 0) return svc::ClearEvent(this->whandle);
        if(this->rhandle != 0) return svc::ResetSignal(this->rhandle);
        return ResultInvalidHandle;
    }

    void Event::Close()
    {
        if(this->whandle != 0) svc::CloseHandle(this->whandle);
        if(this->rhandle != 0) svc::CloseHandle(this->rhandle);
        this->whandle = 0;
        this->rhandle = 0;
    }
}