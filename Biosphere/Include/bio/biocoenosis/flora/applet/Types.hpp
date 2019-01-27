
#pragma once
#include <bio/biocoenosis/fauna/sm.hpp>

namespace bio::applet
{
    enum class AppletId
    {
        PlayerSelect = 0x10,
        Cabinet = 0xb,
        Controller = 0xc,
        Error = 0xe,
        Web = 0x13,
        MiiEdit = 0x12,
        Swkbd = 0x11,
    };

    enum class AppletMode
    {
        AllForeground,
        Background,
    };

    enum class ParameterKind
    {
        ApplicationCommon = 1,
        SelectedUser,
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
            ResultWrap<os::Event*> GetLibraryAppletLaunchableEvent();
    };

    class WindowController : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u64> GetAppletResourceUserId();
            Result AcquireForegroundRights();
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

    class StorageAccessor : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u64> GetSize();
            Result Write(u64 Offset, void *Data, size_t Size);
            ResultWrap<void*> Read(u64 Offset, size_t Size);
    };

    class Storage : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<StorageAccessor*> Open();
    };

    class LibraryAppletAccessor : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<os::Event*> GetAppletStateChangedEvent();
            Result Start();
            Result GetResult();
            Result PushInData(Storage *Data);
            ResultWrap<Storage*> PopOutData();
            ResultWrap<os::Event*> GetPopOutDataEvent();
    };

    class LibraryAppletCreator : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<LibraryAppletAccessor*> CreateLibraryApplet(AppletId Id, AppletMode Mode);
            ResultWrap<Storage*> CreateStorage(u64 Size);
    };

    class HomeMenuFunctions : public hipc::Object
    {
        public:
            using Object::Object;
            Result LockForeground();
            Result UnlockForeground();
    };

    class GlobalStateController : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ApplicationAccessor : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<os::Event*> GetAppletStateChangedEvent();
            Result Start();
            Result RequestForApplicationToGetForeground();
            Result PushLaunchParameter(ParameterKind Kind, Storage *Data);
    };

    class ApplicationCreator : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<ApplicationAccessor*> CreateApplication(ApplicationId Id);
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
            ResultWrap<SelfController*> GetSelfController();
            ResultWrap<LibraryAppletCreator*> GetLibraryAppletCreator();
    };
}