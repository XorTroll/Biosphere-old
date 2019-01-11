#include <bio/Biosphere>
#include <string>
using namespace bio;

int main()
{
    // Initialize SM
    sm::ServiceManager *ssm = sm::Initialize().AssertOk();
    ssm->Initialize().AssertOk();

    os::OverrideHeap(0x14000000);

    // Initialize appletAE
    applet::ae::AeService *sae = applet::ae::Initialize(ssm).AssertOk();

    applet::ae::LibraryAppletProxy *lap = sae->OpenLibraryAppletProxyOld(0).AssertOk();

    applet::SelfController *sc = lap->GetSelfController().AssertOk();

    os::Event *elaunch = sc->GetLibraryAppletLaunchableEvent().AssertOk();

    applet::LibraryAppletCreator *lac = lap->GetLibraryAppletCreator().AssertOk();

    applet::LibraryAppletAccessor *laa = lac->CreateLibraryApplet(applet::AppletId::Error, applet::AppletMode::AllForeground).AssertOk();

    applet::Storage *scommon = lac->CreateStorage(0x20).AssertOk();

    applet::StorageAccessor *ascommon = scommon->Open().AssertOk();

    struct CommonArgs
    {
        u32 v;
        u32 sz;
        u32 lav;
        s32 tc;
        u8 p;
        u8 pad[7];
        u64 tick;
    } BIO_PACKED;

    CommonArgs args;
    memset(&args, 0, sizeof(CommonArgs));
    args.v = 1;
    args.sz = 0x20;
    args.lav = 0;
    args.tick = svc::GetSystemTick();

    ascommon->Write(0, &args, sizeof(CommonArgs));

    delete ascommon;

    laa->PushInData(scommon).AssertOk();

    // Second

    applet::Storage *scustom = lac->CreateStorage(4120).AssertOk();

    applet::StorageAccessor *ascustom = scustom->Open().AssertOk();

    u8 data[4120] = { 0 };
    data[0] = 1;
    u32 err = bio::Result(168, 002);
    u64 ecode = (((err & 0x1ffu) + 2000) | (((err >> 9) & 0x1fff & 0x1fffll) << 32));
    *(u64*)&data[8] = ecode;
    std::string str = "Nintendo detected you're a sick pirate and a hacker. You've been sued by NOA in the last 10 minutes. Check your inbox for more details.";
    std::string str2 = "Hehe, this is another test!";
    strcpy((char*)&data[24], str.c_str());
    strcpy((char*)&data[2072], str2.c_str());

    ascustom->Write(0, data, 4120);

    delete ascustom;

    laa->PushInData(scustom).AssertOk();

    elaunch->Wait(UINT64_MAX).AssertOk();

    laa->Start().AssertOk();

    while(true);

    delete elaunch;
    delete scustom;
    delete scommon;
    delete laa;
    delete lac;
    delete lap;
    delete sae;
    delete ssm;

    return 0;
}

int vimain()
{
    sm::ServiceManager *ssm = sm::Initialize().AssertOk();
    ssm->Initialize().AssertOk();

    hipc::Object *vi = ssm->GetService("vi:m").AssertOk();

    u32 hiads = 0;
    vi->ProcessRequest<2>(hipc::InRaw<u32>(0), hipc::OutHandle<0>(hiads)).AssertOk();
    hipc::Object *iads = new hipc::Object(hiads);

    struct ViName
    {
        char name[0x40];
    } BIO_PACKED;

    ViName nam;
    memset(&nam, 0, sizeof(ViName));
    strcpy(nam.name, "Default");

    u64 dispid = 0;
    iads->ProcessRequest<1010>(hipc::InRaw<ViName>(nam), hipc::OutRaw<u64>(dispid)).AssertOk();

    alignas(8) u8 nwindow[0x100] = { 0 };
    u64 nwsize = 0;
    u64 lyid = 0;
    iads->ProcessRequest<2030>(hipc::InRaw<u32>(0), hipc::InRaw<u32>(0), hipc::InRaw<u64>(dispid), hipc::OutBuffer(nwindow, 0x100, 0), hipc::OutRaw<u64>(lyid), hipc::OutRaw<u64>(nwsize)).AssertOk();

    delete iads;

    delete vi;

    delete ssm;

    return 0;
}