
#pragma once
#include <bio/biocoenosis/flora/applet/Types.hpp>

namespace bio::applet::ae
{
    class SystemAppletProxy : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<CommonStateGetter*> GetCommonStateGetter();
            ResultWrap<SelfController*> GetSelfController();
    };

    class LibraryAppletProxy : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<SelfController*> GetSelfController();
            ResultWrap<LibraryAppletCreator*> GetLibraryAppletCreator();
    };

    class OverlayAppletProxy : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class AeService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<SystemAppletProxy*> OpenSystemAppletProxy(u64 Reserved);
            ResultWrap<LibraryAppletProxy*> OpenLibraryAppletProxyOld(u64 Reserved);
            ResultWrap<OverlayAppletProxy*> OpenOverlayAppletProxy(u64 Reserved);
            ResultWrap<ApplicationProxy*> OpenSystemApplicationProxy(u64 Reserved);
    };

    ResultWrap<AeService*> Initialize();
}