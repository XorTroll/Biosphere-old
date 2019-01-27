#include <bio/biocoenosis/flora/applet/Types.hpp>

namespace bio::applet
{
    ResultWrap<os::Event*> SelfController::GetLibraryAppletLaunchableEvent()
    {
        u32 alev = 0;
        Result rc = this->ProcessRequest<9>(hipc::OutHandle<0>(alev));
        return ResultWrap<os::Event*>(rc, new os::Event(alev, false));
    }

    ResultWrap<u64> WindowController::GetAppletResourceUserId()
    {
        u64 aruid = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutRaw<u64>(aruid));
        return ResultWrap<u64>(rc, aruid);
    }

    ResultWrap<u64> StorageAccessor::GetSize()
    {
        u64 sz = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutRaw<u64>(sz));
        return ResultWrap<u64>(rc, sz);
    }

    Result StorageAccessor::Write(u64 Offset, void *Data, size_t Size)
    {
        size_t qbufsize = this->QueryPointerBufferSize().AssertOk();
        return this->ProcessRequest<10>(hipc::InRaw<u64>(Offset), hipc::InSmartBuffer(Data, Size, 0, qbufsize));
    }

    ResultWrap<void*> StorageAccessor::Read(u64 Offset, size_t Size)
    {
        void *data = malloc(Size);
        size_t qbufsize = this->QueryPointerBufferSize().AssertOk();
        Result rc = this->ProcessRequest<11>(hipc::InRaw<u64>(Offset), hipc::OutSmartBuffer(data, Size, 0, qbufsize));
        return ResultWrap<void*>(rc, data);
    }

    ResultWrap<StorageAccessor*> Storage::Open()
    {
        u32 osta = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutObjectId<0>(osta));
        return ResultWrap<StorageAccessor*>(rc, new StorageAccessor(this, osta));
    }

    ResultWrap<os::Event*> LibraryAppletAccessor::GetAppletStateChangedEvent()
    {
        u32 ascev = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutHandle<0>(ascev));
        return ResultWrap<os::Event*>(rc, new os::Event(ascev, false));
    }

    Result LibraryAppletAccessor::Start()
    {
        return this->ProcessRequest<10>(hipc::Simple());
    }

    Result LibraryAppletAccessor::GetResult()
    {
        return this->ProcessRequest<30>(hipc::Simple());
    }

    Result LibraryAppletAccessor::PushInData(Storage *Data)
    {
        u32 soid = Data->GetObjectId();
        return this->ProcessRequest<100>(hipc::InObjectId(soid));
    }

    ResultWrap<Storage*> LibraryAppletAccessor::PopOutData()
    {
        u32 ost = 0;
        Result rc = this->ProcessRequest<101>(hipc::OutObjectId<0>(ost));
        return ResultWrap<Storage*>(rc, new Storage(this, ost));
    }

    ResultWrap<os::Event*> LibraryAppletAccessor::GetPopOutDataEvent()
    {
        u32 podev = 0;
        Result rc = this->ProcessRequest<105>(hipc::OutHandle<0>(podev));
        return ResultWrap<os::Event*>(rc, new os::Event(podev, false));
    }

    ResultWrap<LibraryAppletAccessor*> LibraryAppletCreator::CreateLibraryApplet(AppletId Id, AppletMode Mode)
    {
        u32 olaa = 0;
        Result rc = this->ProcessRequest<0>(hipc::InRaw<u32>(static_cast<u32>(Id)), hipc::InRaw<u32>(static_cast<u32>(Mode)), hipc::OutObjectId<0>(olaa));
        return ResultWrap<LibraryAppletAccessor*>(rc, new LibraryAppletAccessor(this, olaa));
    }

    ResultWrap<Storage*> LibraryAppletCreator::CreateStorage(u64 Size)
    {
        u32 ost = 0;
        Result rc = this->ProcessRequest<10>(hipc::InRaw<u64>(Size), hipc::OutObjectId<0>(ost));
        return ResultWrap<Storage*>(rc, new Storage(this, ost));
    }

    Result HomeMenuFunctions::LockForeground()
    {
        return this->ProcessRequest<11>(hipc::Simple());
    }

    Result HomeMenuFunctions::UnlockForeground()
    {
        return this->ProcessRequest<12>(hipc::Simple());
    }

    ResultWrap<os::Event*> ApplicationAccessor::GetAppletStateChangedEvent()
    {
        u32 ascev = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutHandle<0>(ascev));
        return ResultWrap<os::Event*>(rc, new os::Event(ascev, false));
    }

    Result ApplicationAccessor::Start()
    {
        return this->ProcessRequest<10>(hipc::Simple());
    }

    Result ApplicationAccessor::RequestForApplicationToGetForeground()
    {
        return this->ProcessRequest<101>(hipc::Simple());
    }

    Result ApplicationAccessor::PushLaunchParameter(ParameterKind Kind, Storage *Data)
    {
        return this->ProcessRequest<121>(hipc::InRaw<u32>(static_cast<u32>(Kind)), hipc::InObjectId(Data->GetObjectId()));
    }

    ResultWrap<ApplicationAccessor*> ApplicationCreator::CreateApplication(ApplicationId Id)
    {
        u32 oaa = 0;
        Result rc = this->ProcessRequest<0>(hipc::InRaw<u64>(Id.Id), hipc::OutObjectId<0>(oaa));
        return ResultWrap<ApplicationAccessor*>(rc, new ApplicationAccessor(this, oaa));
    }

    ResultWrap<SelfController*> ApplicationProxy::GetSelfController()
    {
        u32 osch = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutObjectId<0>(osch));
        return ResultWrap<SelfController*>(rc, new SelfController(this, osch));
    }

    ResultWrap<LibraryAppletCreator*> ApplicationProxy::GetLibraryAppletCreator()
    {
        u32 olac = 0;
        Result rc = this->ProcessRequest<11>(hipc::OutObjectId<0>(olac));
        return ResultWrap<LibraryAppletCreator*>(rc, new LibraryAppletCreator(this, olac));
    }
}