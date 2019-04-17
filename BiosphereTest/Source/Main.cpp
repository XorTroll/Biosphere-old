#include <bio/Biosphere>
#include <string>

int vimain()
{
    bio::app::Initialize(bio::app::RunMode::LibraryApplet).AssertOk();
    bio::hipc::Object *vi = bio::sm::GetService("vi:m").AssertOk();
    u32 hiads = 0;
    vi->ProcessRequest<2>(bio::hipc::InRaw<u32>(0), bio::hipc::OutHandle<0>(hiads)).AssertOk();
    bio::hipc::Object *iads = new bio::hipc::Object(hiads);

    struct DisplayName
    {
        char Name[0x40];
    } BIO_PACKED;

    DisplayName dnam = { 0 };
    strcpy(dnam.Name, "Default");

    u64 dspid = 0;
    iads->ProcessRequest<1010>(bio::hipc::InRaw<DisplayName>(dnam), bio::hipc::OutRaw<u64>(dspid)).AssertOk();

    delete iads;
    delete vi;

    bio::app::Finalize();
    return 0;
}

int jsmain()
{
    bio::os::OverrideHeap(0x10000000L);
    bio::app::Initialize(bio::app::RunMode::LibraryApplet);
    bio::err::SetDefaultThrowMode(bio::err::ThrowMode::AppletDialog);

    bio::hipc::Object *jsu = bio::sm::GetService("test:u").AssertOk();

    char jscode[] = "var os = require('os');os.arch()";
    char outres[256];

    // Initialize
    jsu->ProcessRequest<0>(bio::hipc::InRaw<bool>(true)).AssertOk();

    // Evaluation test
    jsu->ProcessRequest<3>(bio::hipc::InBuffer(jscode, sizeof(jscode), 0), bio::hipc::OutBuffer(outres, sizeof(outres), 0)).AssertOk();
    
    bio::app::ErrorApplet *eapp = new bio::app::ErrorApplet(bio::app::ErrorAppletMode::SystemError, bio::Result(202, 203));
    eapp->SetErrorText(std::string(outres));
    eapp->Show();

    // Context edition test
    u32 hcedit = 0;
    jsu->ProcessRequest<4>(bio::hipc::OutHandle<0>(hcedit)).AssertOk();
    bio::hipc::Object *cedit = new bio::hipc::Object(hcedit);

    char vname[] = "testName";
    char vval[] = "die vertigo die";

    u32 hmdl = 0;
    cedit->ProcessRequest<3>(bio::hipc::OutHandle<0>(hmdl)).AssertOk();
    bio::hipc::Object *mdl = new bio::hipc::Object(hmdl);

    char nmdl[] = "mipc";
    mdl->ProcessRequest<0>(bio::hipc::InBuffer(nmdl, sizeof(nmdl), 0)).AssertOk();

    mdl->ProcessRequest<2>(bio::hipc::InBuffer(vname, sizeof(vname), 0), bio::hipc::InBuffer(vval, sizeof(vval), 0)).AssertOk();

    mdl->ProcessRequest<5>(bio::hipc::Simple()).AssertOk();

    delete mdl;

    delete cedit;

    char ncode[] = "var mipc = require('mipc');mipc.testName";
    char nout[256];

    jsu->ProcessRequest<3>(bio::hipc::InBuffer(ncode, sizeof(ncode), 0), bio::hipc::OutBuffer(nout, sizeof(nout), 0)).AssertOk();
    
    eapp = new bio::app::ErrorApplet(bio::app::ErrorAppletMode::SystemError, bio::Result(202, 299));
    eapp->SetErrorText(std::string(nout));
    eapp->Show();

    jsu->ProcessRequest<5>(bio::hipc::Simple()).AssertOk();

    delete jsu;

    bio::app::Finalize();
    return 0;
}

bool fcheck_Check()
{
    return true;
}

int main()
{
    bio::sm::Initialize();
    bio::fsp::FspService *fsp = bio::fsp::Initialize().AssertOk();
    /*
    bio::os::AddFinalizeCheckingFor(fcheck_Check, [&]()
    {
        bio::fsp::FileSystem *sdfs = fsp->OpenSdCardFileSystem().AssertOk();
        sdfs->CreateDirectory("dispose");
    });
    */
    bio::sm::Finalize();
    return 0;
}