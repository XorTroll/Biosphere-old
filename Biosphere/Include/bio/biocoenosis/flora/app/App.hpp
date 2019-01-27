
#pragma once
#include <bio/biocoenosis/flora/app/Types.hpp>

namespace bio::app
{
    Result Initialize(RunMode Mode);
    void Finalize();
    bool HasInitialized();
    bool IsApplet();
    bool IsApplication();
    bool IsAppletOrApplication();
    bool IsSubprocess();
    hipc::Object *GetProxyObject();
    applet::SelfController *GetSelfController();
    applet::LibraryAppletCreator *GetLibraryAppletCreator();
}