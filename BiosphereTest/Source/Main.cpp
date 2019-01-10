#include <bio/Biosphere>
using namespace bio;

int main()
{
    sm::ServiceManager *ssm = sm::Initialize().AssertOk();
    ssm->Initialize().AssertOk();

    hipc::Object *sapplet = ssm->GetService("appletAE").AssertOk();
    sapplet->ConvertToDomain().AssertOk();

    void *bhaddr;
    svc::SetHeapSize(&bhaddr, 0x14000000);

    u32 oiap = 0;
    sapplet->ProcessRequest<200>(hipc::InProcessId(), hipc::InRaw<u64>(0), hipc::InHandle<hipc::HandleMode::Copy>(0xffff8001), hipc::OutObjectId<0>(oiap)).AssertOk();
    hipc::Object *iap = new hipc::Object(sapplet, oiap);

    u32 oisc = 0;
    iap->ProcessRequest<1>(hipc::OutObjectId<0>(oisc)).AssertOk();
    hipc::Object *isc = new hipc::Object(iap, oisc);

    u32 oelaunch = 0;
    isc->ProcessRequest<9>(hipc::OutHandle<0>(oelaunch)).AssertOk();
    os::Event *elaunch = new os::Event(oelaunch, false);

    u32 oilac = 0;
    iap->ProcessRequest<11>(hipc::OutObjectId<0>(oilac)).AssertOk();
    hipc::Object *ilac = new hipc::Object(iap, oilac);

    u32 oilaa = 0;
    ilac->ProcessRequest<0>(hipc::InRaw<u32>(0xe), hipc::InRaw<u32>(0), hipc::OutObjectId<0>(oilaa)).AssertOk();
    hipc::Object *ilaa = new hipc::Object(ilac, oilaa);

    // CommonArgs

    u32 ocargs = 0;
    ilac->ProcessRequest<10>(hipc::InRaw<u64>(0x20), hipc::OutObjectId<0>(ocargs)).AssertOk();
    hipc::Object *cargs = new hipc::Object(ilac, ocargs);

    u32 oacargs = 0;
    cargs->ProcessRequest<0>(hipc::OutObjectId<0>(oacargs)).AssertOk();
    hipc::Object *acargs = new hipc::Object(cargs, oacargs);

    struct LArgs
    {
        u32 v;
        u32 sz;
        u32 lav;
        s32 tc;
        u8 p;
        u8 pad[7];
        u64 tick;
    } BIO_PACKED;

    LArgs args;
    memset(&args, 0, sizeof(LArgs));
    args.v = 1;
    args.sz = 0x20;
    args.lav = 0;
    args.tick = svc::GetSystemTick();

    size_t qbsize = acargs->QueryPointerBufferSize().AssertOk();
    
    acargs->ProcessRequest<10>(hipc::InRaw<u64>(0), hipc::InSmartBuffer(&args, sizeof(LArgs), 0, qbsize)).AssertOk();

    delete acargs;

    ilaa->ProcessRequest<100>(hipc::InObjectId(ocargs)).AssertOk();

    // Arg2

    u32 oargs2 = 0;
    ilac->ProcessRequest<10>(hipc::InRaw<u64>(4120), hipc::OutObjectId<0>(oargs2)).AssertOk();
    hipc::Object *args2 = new hipc::Object(ilac, oargs2);

    u32 oaargs2 = 0;
    args2->ProcessRequest<0>(hipc::OutObjectId<0>(oaargs2)).AssertOk();
    hipc::Object *aargs2 = new hipc::Object(args2, oaargs2);

    u8 data[4120] = { 0 };
    data[0] = 1;
    *(u64*)&data[8] = 12345678;
    const char *str = "Nintendo detected you're a sick pirate and a hacker. You've been sued by NOA in the last 10 minutes. Check your inbox for more details.";
    const char *str2 = "Weird chars:\n(line)\t(tab)\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nbunch of lines";
    strcpy((char*)&data[24], str);
    strcpy((char*)&data[2072], str2);

    size_t qbsize2 = aargs2->QueryPointerBufferSize().AssertOk();
    
    aargs2->ProcessRequest<10>(hipc::InRaw<u64>(0), hipc::InSmartBuffer(data, 4120, 0, qbsize2)).AssertOk();

    delete aargs2;

    ilaa->ProcessRequest<100>(hipc::InObjectId(oargs2)).AssertOk();

    // Send

    elaunch->Wait(UINT64_MAX).AssertOk();

    ilaa->ProcessRequest<10>(hipc::Simple()).AssertOk();

    while(true);

    delete elaunch;

    delete ilaa;

    delete ilac;

    delete isc;

    delete iap;

    delete sapplet;

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