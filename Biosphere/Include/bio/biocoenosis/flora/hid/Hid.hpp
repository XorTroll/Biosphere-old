
#pragma once
#include <bio/biocoenosis/flora/hid/Types.hpp>

namespace bio::hid
{
    class AppletResource : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u32> GetSharedMemoryHandle();
    };

    class HidService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<AppletResource*> CreateAppletResource(u64 AppletResourceUserId);
            Result SetSupportedNpadStyleSet(u32 NpadStyleTag, u64 AppletResourceUserId);
            Result SetSupportedNpadIdType(u64 AppletResourceUserId, u32 *Controllers, size_t ControllerCount);
            Result ActivateNpad(u64 AppletResourceUserId);
            Result SetNpadJoyAssignmentModeSingle(u32 Controller, u64 AppletResourceUserId, u64 JoyType);
            Result SetNpadJoyAssignmentModeDual(u32 Controller, u64 AppletResourceUserId);
    };

    ResultWrap<HidService*> Initialize(sm::ServiceManager *SM);
}