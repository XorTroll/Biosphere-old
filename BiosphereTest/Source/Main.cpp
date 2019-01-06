#include <bio/biosphere>

int bio::Main()
{
    sm::ServiceManager *ssm = sm::Initialize().AssertOk();
    ssm->Initialize().AssertOk();

    hipc::Object *appletae = ssm->GetService("appletAE").AssertOk();
    appletae->ConvertToDomain().AssertOk();

    void *haddr;
    Result(svc::SetHeapSize(&haddr, 0x14000000)).AssertOk();

    u32 oilap;
    appletae->ProcessRequest<200>(hipc::InProcessId(), hipc::InRaw<u64>(0), hipc::InHandle<hipc::HandleMode::Copy>(0xffff8001), hipc::OutObjectId<0>(oilap)).AssertOk();
    hipc::Object *ilap = new hipc::Object(appletae, oilap);

    u32 oilac = 0;
    ilap->ProcessRequest<11>(hipc::OutObjectId<0>(oilac)).AssertOk();
    hipc::Object *ilac = new hipc::Object(appletae, oilac);

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

    ilaa->ProcessRequest<10>(hipc::Simple()).AssertOk();

    while(true);

    delete ilaa;

    delete ilac;

    delete ilap;

    delete appletae;

    delete ssm;

    return 0;
}