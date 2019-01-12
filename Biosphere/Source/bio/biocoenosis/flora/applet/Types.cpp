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
        void *data;
        size_t qbufsize = this->QueryPointerBufferSize().AssertOk();
        Result rc = this->ProcessRequest<11>(hipc::InRaw<u64>(Offset), hipc::InSmartBuffer(data, Size, 0, qbufsize));
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