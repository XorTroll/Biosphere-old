#include <bio/Biosphere>
#include <string>

// Latest test: as a system applet, attempts to launch Goldleaf as an installed title. MUST BE RUN AS A QLAUNCH REPLACEMENT ON ATMOSPHERE!!

int main()
{
    bio::os::OverrideHeap(0x10000000).AssertOk();
    bio::sm::Initialize();
    // appletAE
    bio::applet::ae::AeService *ae = bio::applet::ae::Initialize().AssertOk();
    bio::err::SetDefaultThrowMode(bio::err::ThrowMode::Fatal);
    bio::svc::SleepThread(1000000000);
    // appletAE::OpenSystemAppletProxy(u64 Reserved = 0) -> ISystemAppletProxy
    bio::applet::ae::SystemAppletProxy *isap = ae->OpenSystemAppletProxy(0).AssertOk();
    u32 oihmf = 0;
    isap->ProcessRequest<20>(bio::hipc::OutObjectId<0>(oihmf)).AssertOk();
    // ISystemAppletProxy::GetHomeMenuFunctions() -> IHomeMenuFunctions
    bio::hipc::Object *ihmf = new bio::hipc::Object(isap, oihmf);
    // IHomeMenuFunctions::UnlockForeground()
    ihmf->ProcessRequest<12>(bio::hipc::Simple()).AssertOk();
    u32 oiac = 0;
    // ISystemAppletProxy::GetApplicationCreator() -> IAppicationCreator
    isap->ProcessRequest<22>(bio::hipc::OutObjectId<0>(oiac)).AssertOk();
    bio::hipc::Object *iac = new bio::hipc::Object(isap, oiac);
    u32 oiaa = 0;
    // IApplicationCreator::CreateApplication(u64 ApplicationId) -> IApplicationAccessor
    iac->ProcessRequest<0>(bio::hipc::InRaw<u64>(0x050032a5cf12e000), bio::hipc::OutObjectId<0>(oiaa)).AssertOk();
    bio::hipc::Object *iaa = new bio::hipc::Object(iac, oiaa);
    // IApplicationAccessor::Start()
    iaa->ProcessRequest<10>(bio::hipc::Simple()).AssertOk();
    bio::svc::SleepThread(1000000000);
    // IApplicationAccessor::RequestForApplicationToGetForeground()
    iaa->ProcessRequest<101>(bio::hipc::Simple()).AssertOk();
    while(true);
    bio::sm::Finalize();
    return 0;
}