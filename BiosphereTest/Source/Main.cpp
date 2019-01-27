#include <bio/Biosphere>
#include <string>

// Latest test: as a system applet, attempts to launch Goldleaf as an installed title. MUST BE RUN AS A QLAUNCH REPLACEMENT ON ATMOSPHERE!!

int main()
{
    bio::os::OverrideHeap(0x10000000).AssertOk();
    bio::app::Initialize(bio::app::RunMode::SystemApplet).AssertOk();
    bio::svc::SleepThread(1000000000);

    bio::applet::ae::SystemAppletProxy *isap = (bio::applet::ae::SystemAppletProxy*)bio::app::GetProxyObject();
    bio::applet::WindowController *wc = isap->GetWindowController().AssertOk();
    bio::applet::HomeMenuFunctions *hmf = isap->GetHomeMenuFunctions().AssertOk();
    bio::applet::ApplicationCreator *ac = isap->GetApplicationCreator().AssertOk();
    bio::applet::LibraryAppletCreator *lac = bio::app::GetLibraryAppletCreator();

    bio::app::PlayerSelectApplet *psel = new bio::app::PlayerSelectApplet();
    bio::account::Uid userid = psel->Show().AssertOk();

    bio::applet::ApplicationAccessor *aa = ac->CreateApplication(bio::ApplicationId(0x01006A800016E000)).AssertOk();
    bio::os::Event *chev = aa->GetAppletStateChangedEvent().AssertOk();
    hmf->UnlockForeground().AssertOk();

    struct UserData
    {
        u32  m;
        u8   unk1;
        u8   pad[3];
        u128 userID;
        u8   unk2[0x70];
    } BIO_PACKED;
    UserData udata = { 0 };
    udata.m = 0xc79497ca;
    udata.unk1 = 1;
    udata.userID = userid;

    bio::applet::Storage *ust = lac->CreateStorage(sizeof(UserData)).AssertOk();
    bio::applet::StorageAccessor *usta = ust->Open().AssertOk();
    usta->Write(0, &udata, sizeof(UserData)).AssertOk();
    delete usta;
    aa->PushLaunchParameter(bio::applet::ParameterKind::SelectedUser, ust).AssertOk();

    aa->RequestForApplicationToGetForeground().AssertOk();
    aa->Start().AssertOk();

    chev->Wait(UINT64_MAX).AssertOk();
    delete chev;

    while(true);

    bio::app::Finalize();
    return 0;
}

int oldmain()
{
    bio::os::OverrideHeap(0x18000000).AssertOk();
    bio::sm::Initialize();
    // appletAE
    bio::applet::ae::AeService *ae = bio::applet::ae::Initialize().AssertOk();
    bio::err::SetDefaultThrowMode(bio::err::ThrowMode::Fatal);
    bio::svc::SleepThread(3000000000);
    // appletAE::OpenSystemAppletProxy(u64 Reserved = 0) -> ISystemAppletProxy
    bio::applet::ae::SystemAppletProxy *isap = ae->OpenSystemAppletProxy(0).AssertOk();
    u32 oihmf = 0;
    // ISystemAppletProxy::GetHomeMenuFunctions() -> IHomeMenuFunctions
    isap->ProcessRequest<20>(bio::hipc::OutObjectId<0>(oihmf)).AssertOk();
    bio::hipc::Object *ihmf = new bio::hipc::Object(isap, oihmf);
    u32 oiwc = 0;
    isap->ProcessRequest<2>(bio::hipc::OutObjectId<0>(oiwc)).AssertOk();
    bio::hipc::Object *iwc = new bio::hipc::Object(isap, oiwc);
    iwc->ProcessRequest<10>(bio::hipc::Simple()).AssertOk();
    u32 oilac = 0;
    // ISystemAppletProxy::GetLibraryAppletCreator() -> ILibraryAppletCreator
    isap->ProcessRequest<11>(bio::hipc::OutObjectId<0>(oilac)).AssertOk();
    bio::applet::LibraryAppletCreator *ilac = new bio::applet::LibraryAppletCreator(isap, oilac);
    u32 oiac = 0;
    // ISystemAppletProxy::GetApplicationCreator() -> IAppicationCreator
    isap->ProcessRequest<22>(bio::hipc::OutObjectId<0>(oiac)).AssertOk();
    bio::hipc::Object *iac = new bio::hipc::Object(isap, oiac);
    u32 oiaa = 0;
    // IApplicationCreator::CreateApplication(u64 ApplicationId) -> IApplicationAccessor
    iac->ProcessRequest<0>(bio::hipc::InRaw<u64>(0x01000320000CC000), bio::hipc::OutObjectId<0>(oiaa)).AssertOk();
    bio::hipc::Object *iaa = new bio::hipc::Object(iac, oiaa);
    // IHomeMenuFunctions::UnlockForeground()
    ihmf->ProcessRequest<12>(bio::hipc::Simple()).AssertOk();
    /*
    struct UserData
    {
        u32  m;
        u8   unk1;
        u8   pad[3];
        u128 userID;
        u8   unk2[0x70];
    } BIO_PACKED;
    UserData udata = { 0 };
    udata.m = 0xc79497ca;
    udata.unk1 = 1;
    udata.userID = (((u128)(0x37e67802c2321) << 64) | (u128)(0x083be39af5365e18));
    bio::applet::Storage *ist = ilac->CreateStorage(sizeof(UserData)).AssertOk();
    bio::applet::StorageAccessor *ista = ist->Open().AssertOk();
    ista->Write(0, &udata, sizeof(UserData)).AssertOk();
    delete ista;
    iaa->ProcessRequest<121>(bio::hipc::InRaw<u32>(2), bio::hipc::InObjectId(ist->GetObjectId())).AssertOk();
    */
    u32 evh = 0;
    iaa->ProcessRequest<0>(bio::hipc::OutHandle<0>(evh)).AssertOk();
    bio::os::Event *ev = new bio::os::Event(evh, false);
    // IApplicationAccessor::RequestForApplicationToGetForeground()
    iaa->ProcessRequest<101>(bio::hipc::Simple()).AssertOk();
    // IApplicationAccessor::Start()
    iaa->ProcessRequest<10>(bio::hipc::Simple()).AssertOk();
    ev->Wait(UINT64_MAX).AssertOk();
    delete ev;
    while(true);
    bio::sm::Finalize();
    return 0;
}