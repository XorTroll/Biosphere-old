#include <bio/Biosphere>
#include <string>
using namespace bio;

int main()
{
    os::OverrideHeap(0x10000000).AssertOk();
    app::Initialize(app::RunMode::LibraryApplet);

    app::Applet *psel = new app::Applet(applet::AppletId::PlayerSelect);
    u8 data[0xa0] = { 0 };
    psel->SendDataViaStorage(data, 0xa0);
    psel->Launch().AssertOk();
    Result rc = psel->WaitFinish();
    delete psel;

    app::Finalize();
    return rc;
}

int vimain()
{
    hipc::Object *vi = sm::GetService("vi:m").AssertOk();

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

    return 0;
}