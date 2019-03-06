
#pragma once
#include <bio/root.hpp>
#include <bio/biotope/svc.hpp>
#include <functional>

namespace bio::os
{
    enum class Executable
    {
        NRO,
        NSO,
    };

    enum class FirmwareMajor
    {
        Major1,
        Major2,
        Major3,
        Major4,
        Major5,
        Major6,
    };

    class HandleObject
    {
        public:
            HandleObject(u32 Handle);
            u32 GetHandle();
        protected:
            u32 handle;
    };

    struct FinalizeCheckInfo
    {
        std::function<bool()> CheckCallback;
        std::function<void()> FinalizeCallback;
    };

    int GetHostArgc();
    char **GetHostArgv();
    FirmwareMajor GetFirmwareMajorVersion();
    u32 GetCurrentProcessHandle();
    u64 EncodeString(const char *Text);
    Executable GetExecutableType();
    Result OverrideHeap(u64 CustomSize);
    bool IsHeapOverrided();
    void RestoreOverridedHeap();
    void AddFinalizeCheckingFor(std::function<bool()> CheckCallback, std::function<void()> FinalizeCallback);

    static const u32 Module = 1;
    static const Result ResultNotImplemented(Module, 33);
    static const Result ResultInvalidHandle(Module, 114);
    static const Result ResultInvalidEnum(Module, 120);
    static const Result ResultPortRemoteDead(Module, 123);
    static const Result ResultPortClosed(Module, 131);
    static const Result ResultResourceLimitExceeded(Module, 132);
}