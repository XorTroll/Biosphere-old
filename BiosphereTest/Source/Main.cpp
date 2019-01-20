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
            bio::app::WebApplet *web = new bio::app::WebApplet("https://github.com/XorTroll");
            web->Show().AssertOk();
            delete web;
        }
        else if(ipt & bio::input::Key::Plus) break;
    }

    bio::app::Finalize();
    return 0;
}