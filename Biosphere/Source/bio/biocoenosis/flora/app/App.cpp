#include <bio/biocoenosis/flora/app/App.hpp>

namespace bio::app
{
    static bool ainit = false;
    static RunMode rmode;
    static hipc::Object *appletsrv;
    static hipc::Object *appletproxy;

    Result Initialize(sm::ServiceManager *SM, RunMode Mode)
    {
        Result rc;
        if(!ainit)
        {
            switch(Mode)
            {
                case RunMode::Application:
                    appletsrv = (hipc::Object*)applet::oe::Initialize(SM).AssertOk();
                    appletproxy = (hipc::Object*)((applet::oe::OeService*)appletsrv)->OpenApplicationProxy(0).AssertOk();
                    break;
                case RunMode::LibraryApplet:
                    appletsrv = (hipc::Object*)applet::ae::Initialize(SM).AssertOk();
                    appletproxy = (hipc::Object*)((applet::ae::AeService*)appletsrv)->OpenLibraryAppletProxyOld(0).AssertOk();
                    break;
                case RunMode::SystemApplet:
                    appletsrv = (hipc::Object*)applet::ae::Initialize(SM).AssertOk();
                    appletproxy = (hipc::Object*)((applet::ae::AeService*)appletsrv)->OpenSystemAppletProxy(0).AssertOk();
                    break;
                default:
                    break;
            }
            rmode = Mode;
            ainit = true;
        }
        return rc;
    }

    void Finalize();

    bool HasInitialized();

    bool IsApplet();

    bool IsApplication();

    bool IsAppletOrApplication();

    bool IsSubprocess();

}