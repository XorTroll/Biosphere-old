#include <bio/biocoenosis/flora/app/App.hpp>

namespace bio::app
{
    static bool ainit = false;
    static RunMode rmode = RunMode::Subprocess;
    static hipc::Object *apsrv = NULL;
    static hipc::Object *approxy = NULL;
    static applet::LibraryAppletCreator *lac = NULL;
    static applet::SelfController *sc = NULL;

    Result Initialize(RunMode Mode)
    {
        Result rc;
        if(!ainit)
        {
            sm::Initialize().AssertOk();
            switch(Mode)
            {
                case RunMode::Application:
                    apsrv = (hipc::Object*)applet::oe::Initialize().AssertOk();
                    approxy = (hipc::Object*)((applet::oe::OeService*)apsrv)->OpenApplicationProxy(0).AssertOk();
                    sc = ((applet::ApplicationProxy*)approxy)->GetSelfController().AssertOk();
                    lac = ((applet::ApplicationProxy*)approxy)->GetLibraryAppletCreator().AssertOk();
                    break;
                case RunMode::LibraryApplet:
                    apsrv = (hipc::Object*)applet::ae::Initialize().AssertOk();
                    approxy = (hipc::Object*)((applet::ae::AeService*)apsrv)->OpenLibraryAppletProxyOld(0).AssertOk();
                    sc = ((applet::ae::LibraryAppletProxy*)approxy)->GetSelfController().AssertOk();
                    lac = ((applet::ae::LibraryAppletProxy*)approxy)->GetLibraryAppletCreator().AssertOk();
                    break;
                case RunMode::SystemApplet:
                    apsrv = (hipc::Object*)applet::ae::Initialize().AssertOk();
                    approxy = (hipc::Object*)((applet::ae::AeService*)apsrv)->OpenSystemAppletProxy(0).AssertOk();
                    sc = ((applet::ae::LibraryAppletProxy*)approxy)->GetSelfController().AssertOk();
                    lac = ((applet::ae::LibraryAppletProxy*)approxy)->GetLibraryAppletCreator().AssertOk();
                    break;
                default:
                    break;
            }
            rmode = Mode;
            ainit = true;
        }
        return rc;
    }

    void Finalize()
    {
        if(ainit)
        {
            if(IsAppletOrApplication())
            {
                delete lac;
                delete sc;
                delete approxy;
                delete apsrv;
            }
            sm::Finalize();
            ainit = false;
        }
    }

    bool HasInitialized()
    {
        return ainit;
    }

    bool IsApplet()
    {
        return ((rmode == RunMode::LibraryApplet) || (rmode == RunMode::SystemApplet) || (rmode == RunMode::OverlayApplet));
    }

    bool IsApplication()
    {
        return ((rmode == RunMode::Application) || (rmode == RunMode::SystemApplication));
    }

    bool IsAppletOrApplication()
    {
        return (rmode != RunMode::Subprocess);
    }

    bool IsSubprocess()
    {
        return !IsAppletOrApplication();
    }

    hipc::Object *GetProxyObject()
    {
        return approxy;
    }

    applet::SelfController *GetSelfController()
    {
        return sc;
    }

    applet::LibraryAppletCreator *GetLibraryAppletCreator()
    {
        return lac;
    }
}