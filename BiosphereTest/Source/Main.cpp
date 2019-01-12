#include <bio/Biosphere>
#include <string>

// Latest test:
// - Press A to ask for a user, and then error with error code 2101-101 and the user's name.
// - Press Plus to exit.

int main()
{
    bio::os::OverrideHeap(0x10000000).AssertOk();
    bio::app::Initialize(bio::app::RunMode::LibraryApplet);

    bio::input::InputManager *inm = new bio::input::InputManager(0);
    bio::input::Player *handheld = inm->GetPlayer(bio::input::Controller::HandHeld);

    while(true)
    {
        u64 ipt = handheld->GetInput();
        if(ipt & bio::input::Key::A)
        {
            bio::app::Applet *psel = new bio::app::Applet(bio::applet::AppletId::PlayerSelect);
            u8 data[0xa0] = { 0 };
            psel->SendDataViaStorage(data, 0xa0).AssertOk();
            psel->Launch().AssertOk();
            psel->WaitFinish().AssertOk();
            u8 *outdata = (u8*)psel->ReceiveDataFromStorage(24).AssertOk();
            u128 uid = *(u128*)&outdata[8];
            delete psel;

            bio::hipc::Object *acc = bio::sm::GetService("acc:u0").AssertOk();
            u32 hprof = 0;
            acc->ProcessRequest<5>(bio::hipc::InRaw<u128>(uid), bio::hipc::OutHandle<0>(hprof)).AssertOk();
            bio::hipc::Object *prof = new bio::hipc::Object(hprof);

            struct PBase
            {
                u128 uid;
                u64 ts;
                char uname[0x20];
            } BIO_PACKED;

            PBase pb;
            memset(&pb, 0, sizeof(PBase));

            prof->ProcessRequest<1>(bio::hipc::OutRaw<PBase>(pb)).AssertOk();

            delete prof;
            delete acc;

            bio::app::ErrorApplet *eapp = new bio::app::ErrorApplet(bio::app::ErrorAppletMode::SystemError, bio::Result(101, 101));
            eapp->SetErrorText(std::string(pb.uname));
            eapp->Show().AssertOk();
            delete eapp;
        }
        else if(ipt & bio::input::Key::Plus) break;
    }

    bio::app::Finalize();
    return 0;
}