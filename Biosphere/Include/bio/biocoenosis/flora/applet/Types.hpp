
#pragma once
#include <bio/biocoenosis/flora/sm.hpp>

namespace bio::applet
{
    enum class AppletId
    {
        Swkbd = 0x11,
    };

    enum class AppletMode
    {
        AllForeground,
    };

    class CommonStateGetter : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class SelfController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class WindowController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class AudioController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class DisplayController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ProcessWindingController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class LibraryAppletCreator : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class HomeMenuFunctions : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class GlobalStateController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ApplicationCreator : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class DebugFunctions : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class OverlayFunctions : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ApplicationFunctions : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ApplicationProxy : public hipc::Object
    {
        public:
            using Object::Object;
    };
}