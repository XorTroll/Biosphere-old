#include <bio/Biosphere>
using namespace bio;

int main()
{
    sm::ServiceManager *ssm = sm::Initialize().AssertOk();
    ssm->Initialize().AssertOk();

    hipc::Object *appletoe = ssm->GetService("appletOE").AssertOk();
    appletoe->ConvertToDomain().AssertOk();

    /*
    void *haddr;
    Result(svc::SetHeapSize(&haddr, 0x14000000)).AssertOk();
    */

    u32 oiap = 0;
    appletoe->ProcessRequest<0>(hipc::InProcessId(), hipc::InRaw<u64>(0), hipc::InHandle<hipc::HandleMode::Copy>(0xffff8001), hipc::OutObjectId<0>(oiap)).AssertOk();
    hipc::Object *iap = new hipc::Object(appletoe, oiap);

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

    u32 ocargs2 = 0;
    ilac->ProcessRequest<10>(hipc::InRaw<u64>(0x10), hipc::OutObjectId<0>(ocargs2)).AssertOk();
    hipc::Object *cargs2 = new hipc::Object(ilac, ocargs2);

    u32 oacargs2 = 0;
    cargs->ProcessRequest<0>(hipc::OutObjectId<0>(oacargs2)).AssertOk();
    hipc::Object *acargs2 = new hipc::Object(cargs2, oacargs2);

    u8 data[0x10] = { 0 };

    size_t qbsize2 = acargs->QueryPointerBufferSize().AssertOk();
    
    acargs2->ProcessRequest<10>(hipc::InRaw<u64>(0), hipc::InSmartBuffer(data, 0x10, 0, qbsize2)).AssertOk();

    delete acargs2;

    ilaa->ProcessRequest<100>(hipc::InObjectId(ocargs2)).AssertOk();

    elaunch->Wait(UINT64_MAX).AssertOk();

    ilaa->ProcessRequest<10>(hipc::Simple()).AssertOk();

    while(true);

    delete elaunch;

    delete ilaa;

    delete ilac;

    delete isc;

    delete iap;

    delete appletoe;

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