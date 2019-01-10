
#pragma once
#include <cstdint>
#include <vector>
#include <bio/root/Typedefs.hpp>
#include <bio/root/Macros.hpp>

namespace bio
{
    enum class Application
    {
        None,
        Application,
        SystemApplet,
        LibraryApplet,
        OverlayApplet,
        SystemApplication,
    };

    enum class Memory
    {
        Unmapped = 0x0,
        Io = 0x1,
        Normal = 0x2,
        CodeStatic = 0x3,
        CodeMutable = 0x4,
        HeapMem = 0x5,
        SharedMem = 0x6,
        WeirdMappedMem = 0x7,
        ModuleCodeStatic = 0x8,
        ModuleCodeMutable = 0x9,
        IPCBuffer0 = 0xa,
        MappedMemory = 0xb,
        ThreadLocal = 0xc,
        TransferMemIsolated = 0xd,
        TransferMem = 0xe,
        ProcessMem = 0xf,
        Reserved = 0x10,
        IPCBuffer1 = 0x11,
        IPCBuffer3 = 0x12,
        KernelStack = 0x13,
        CodeReadOnly = 0x14,
        CodeWritable = 0x15,
    };

    enum class MemoryState
    {
        Type = 0xff,
        PermChangeAllowed = BIO_BITMASK(8),
        ForceRwByDebugSyscalls = BIO_BITMASK(9),
        IPCSendAllowedT0 = BIO_BITMASK(10),
        IPCSendAllowedT3 = BIO_BITMASK(11),
        IPCSendAllowedT1 = BIO_BITMASK(12),
        ProcessPermChangeAllowed = BIO_BITMASK(14),
        MapAllowed = BIO_BITMASK(15),
        UnmapProcessCodeMemAllowed = BIO_BITMASK(16), 
        TransferMemAllowed = BIO_BITMASK(17),
        QueryPAddrAllowed = BIO_BITMASK(18),
        MapDeviceAllowed = BIO_BITMASK(19),
        MapDeviceAlignedAllowed = BIO_BITMASK(20),
        IPCBufferAllowed = BIO_BITMASK(21),
        IsPoolAllocated = BIO_BITMASK(22),
        IsRefCounted = IsPoolAllocated,
        MapProcessAllowed = BIO_BITMASK(23),
        AttrChangeAllowed = BIO_BITMASK(24),
        CodeMemAllowed = BIO_BITMASK(25),
    };

    enum class MemoryAttribute
    {
        IsBorrowed = BIO_BITMASK(0),
        IsIPCMapped = BIO_BITMASK(1),
        IsDeviceMapped = BIO_BITMASK(2),
        IsUncached = BIO_BITMASK(3),
    };

    enum class Permission
    {
        NoPermission = 0,
        Read = BIO_BITMASK(0),
        Write = BIO_BITMASK(1),
        Execute = BIO_BITMASK(2),
        ReadWrite = (Read | Write),
        ReadExecute = (Read | Execute),
        DontCare = BIO_BITMASK(28),
    };

    enum class CodeMapOperation
    {
        MapOwner = 0,
        MapSlave = 1,
        UnmapOwner = 2,
        UnmapSlave = 3,
    };

    enum class LimitableResource
    {
        Memory = 0,
        Threads = 1,
        Events = 2,
        TransferMemories = 3,
        Sessions = 4,
    };

    enum class ProcessInfo
    {
        ProcessState = 0,
    };

    enum class ProcessState
    {
        Created = 0,
        DebugAttached = 1,
        DebugDetached = 2,
        Crashed = 3,
        Running = 4,
        Exiting = 5,
        Exited = 6,
        DebugSuspended = 7,
    };

    enum class DebugThreadParameter
    {
        ActualPriority = 0,
        State = 1,
        IdealCore = 2,
        CurrentCore = 3,
        CoreMask = 4,
    };

    struct ApplicationId
    {
        u64 Id;
        ApplicationId(u64 Id);
        bool operator==(const ApplicationId &Other);
        bool operator!=(const ApplicationId &Other);
        static ApplicationId GetInvalidId();
    };

    struct Result
    {
        u32 Module;
        u32 Description;
        Result();
        Result(u32 ResultCode);
        Result(u32 Module, u32 Description);
        bool IsFailure();
        bool IsSuccess();
        void AssertOk();
        bool operator==(const Result &Other);
        bool operator!=(const Result &Other);
        operator u32();
    };

    struct ErrorCode
    {
        u64 Value;
        ErrorCode();
        ErrorCode(Result ResultCode);
        ErrorCode(u32 Module, u32 Description);
        bool IsFailure();
        bool IsSuccess();
        operator u64();
    };

    template<class Wrapped>
    class ResultWrap
    {
        public:
            ResultWrap(Result Res, Wrapped Object);
            operator Result();
            Wrapped AssertOk();
        protected:
            Wrapped obj;
            Result res;
    };

    class ResultClass
    {
        public:
            Result GetResult();
            bool IsSuccess();
            bool IsFailure();
        protected:
            Result res;
    };

    struct MemoryInfo
    {
        u64 Address;
        u64 Size;
        u32 Type;
        u32 Attributes;
        u32 Permissions;
        u32 DeviceReferenceCount;
        u32 IPCReferenceCount;
        u32 Padding;
    };

    struct BIO_PACKED SecureMonitorArgs
    {
        u64 Args[8];
    };

    int Main();
    
    static const u32 Module = 420;
    static const Result ResultSuccess(0, 0);
    static const Result ResultOutOfMemory(Module, 1);
    static const Result ResultMemoryQuery(Module, 2);
    static const Result ResultWrongReentStruct(Module, 3);
    static const Result ResultWeirdKernelInformation(Module, 4);
    static const Result ResultRequestStackInformation(Module, 5);
    static const Result ResultRequestHeapInformation(Module, 6);
    static const Result ResultHeapAllocation(Module, 7);
    static const Result ResultWrongRelocate(Module, 8);
    static const Result ResultEventNotInitialized(Module, 9);
}

#include <bio/root/Types.ipp>