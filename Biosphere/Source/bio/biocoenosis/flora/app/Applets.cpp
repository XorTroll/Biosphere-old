#include <bio/biocoenosis/flora/app/Applets.hpp>
#include <bio/biocoenosis/flora/app/App.hpp>

namespace bio::app
{
    Applet::Applet(applet::AppletId Id)
    {
        this->scargs = false;
        this->commonargs = (u8*)calloc(0x20, 1);
        this->lac = GetLibraryAppletCreator();
        this->laa = this->lac->CreateLibraryApplet(Id, applet::AppletMode::AllForeground).AssertOk();
        applet::SelfController *sc = GetSelfController();
        this->elaunchable = sc->GetLibraryAppletLaunchableEvent().AssertOk();
        this->estate = this->laa->GetAppletStateChangedEvent().AssertOk();
        this->epod = this->laa->GetPopOutDataEvent().AssertOk();
        *(u32*)&this->commonargs[0] = 1;
        *(u32*)&this->commonargs[0x4] = 0x20;
        *(u64*)&this->commonargs[0x18] = svc::GetSystemTick();
    }

    Applet::~Applet()
    {
        this->Finalize();
    }

    void Applet::SetAPIVersion(u32 Version)
    {
        *(u32*)&this->commonargs[0x8] = Version;
    }

    void Applet::SetPlayStartupSound(bool PlaySound)
    {
        this->commonargs[0x10] = (u8)PlaySound;
    }

    Result Applet::SendCommonArguments()
    {
        Result rc;
        if(!this->scargs)
        {
            applet::Storage *st = this->lac->CreateStorage(0x20).AssertOk();
            applet::StorageAccessor *ast = st->Open().AssertOk();
            ast->Write(0, this->commonargs, 0x20);
            delete ast;
            rc = this->laa->PushInData(st);
            delete st;
            this->scargs = true;
        }
        return rc;
    }

    Result Applet::SendDataViaStorage(void *Data, size_t Size)
    {
        Result rc = SendCommonArguments();
        if(rc.IsFailure()) return rc;
        applet::Storage *st = this->lac->CreateStorage(Size).AssertOk();
        applet::StorageAccessor *ast = st->Open().AssertOk();
        ast->Write(0, Data, Size);
        delete ast;
        rc = this->laa->PushInData(st);
        delete st;
        return rc;
    }

    ResultWrap<void*> Applet::ReceiveDataFromStorage(size_t Size)
    {
        this->epod->Wait(U64_MAX).AssertOk();
        applet::Storage *st = this->laa->PopOutData().AssertOk();
        applet::StorageAccessor *ast = st->Open().AssertOk();
        void *data = ast->Read(0, Size).AssertOk();
        delete ast;
        delete st;
        return ResultWrap<void*>(0, data);
    }

    Result Applet::Launch()
    {
        Result rc = SendCommonArguments();
        if(rc.IsFailure()) return rc;
        this->elaunchable->Wait(U64_MAX).AssertOk();
        return this->laa->Start();
    }

    Result Applet::WaitFinish()
    {
        this->estate->Wait(U64_MAX).AssertOk();
        return this->GetLaunchResult();
    }

    Result Applet::GetLaunchResult()
    {
        return this->laa->GetResult();
    }

    void Applet::Finalize()
    {
        delete this->estate;
        delete this->epod;
        delete this->elaunchable;
        delete this->laa;
    }

    ErrorApplet::ErrorApplet(ErrorAppletMode Mode, Result Res) : Applet(applet::AppletId::Error)
    {
        switch(Mode)
        {
            case ErrorAppletMode::Default:
                this->eargssize = 20;
                this->errorargs = (u8*)calloc(20, 1);
                *(u32*)&this->errorargs[16] = (u32)Res;
                break;
            case ErrorAppletMode::SystemError:
                this->eargssize = 4120;
                this->errorargs = (u8*)calloc(4120, 1);
                this->errorargs[0] = 1;
                *(u64*)&this->errorargs[8] = (u64)ErrorCode(Res);
                break;
        }
    }

    void ErrorApplet::SetErrorText(std::string Text)
    {
        strcpy((char*)&this->errorargs[0x18], Text.c_str());
    }

    void ErrorApplet::SetLongErrorText(std::string Text)
    {
        strcpy((char*)&this->errorargs[0x818], Text.c_str());
    }

    Result ErrorApplet::Show()
    {
        Result rc = this->SendDataViaStorage(this->errorargs, this->eargssize);
        if(rc.IsFailure()) return rc;
        rc = this->Launch();
        if(rc.IsFailure()) return rc;
        rc = this->WaitFinish();
        return rc;
    }
}