#include <bio/biotope/svc/SysCalls.hpp>

extern "C"
{
    u32 bioSysCallSetHeapSize(void **OutAddress, u64 Size);
    u32 bioSysCallSetMemoryPermission(void *StartAddress, u64 Size, u32 Permissions);
    u32 bioSysCallSetMemoryAttribute(void *StartAddress, u64 Size, u32 State0, u32 State1);
    u32 bioSysCallMapMemory(void *DestAddress, void *SourceAddress, u64 Size);
    u32 bioSysCallUnmapMemory(void *DestAddress, void *SourceAddress, u64 Size);
    u32 bioSysCallQueryMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u64 Address);
    void BIO_NORETURN bioSysCallExitProcess(void);
    u32 bioSysCallCreateThread(u32 *Out_ThreadHandle, void *EntryPoint, void *Arguments, void *StackTop, int Priority, int CPUID);
    u32 bioSysCallStartThread(u32 ThreadHandle);
    void BIO_NORETURN bioSysCallExitThread(void);
    u32 bioSysCallSleepThread(u64 NanoSeconds);
    u32 bioSysCallGetThreadPriority(u32 *Out_Priority, u32 ThreadHandle);
    u32 bioSysCallSetThreadPriority(u32 ThreadHandle, u32 Priority);
    u32 bioSysCallGetThreadCoreMask(s32 *Out_PreferredCore, u32 *Out_AffinityMask, u32 ThreadHandle);
    u32 bioSysCallSetThreadCoreMask(u32 ThreadHandle, s32 PreferredCore, u32 AffinityMask);
    u32 bioSysCallGetCurrentProcessorNumber(void);
    u32 bioSysCallSignalEvent(u32 EventHandle);
    u32 bioSysCallClearEvent(u32 EventHandle);
    u32 bioSysCallMapSharedMemory(u32 VarHandle, void *Address, size_t Size, u32 Permissions);
    u32 bioSysCallUnmapSharedMemory(u32 VarHandle, void *Address, size_t Size);
    u32 bioSysCallCreateTransferMemory(u32 *Out_Handle, void *Address, size_t Size, u32 Permissions);
    u32 bioSysCallCloseHandle(u32 VarHandle);
    u32 bioSysCallResetSignal(u32 VarHandle);
    u32 bioSysCallWaitSynchronization(s32 *Index, const u32 *Handles, s32 HandleCount, u64 Timeout);
    u32 bioSysCallCancelSynchronization(u32 ThreadHandle);
    u32 bioSysCallArbitrateLock(u32 WaitTag, u32 *TagLocation, u32 SelfTag);
    u32 bioSysCallArbitrateUnlock(u32 *TagLocation);
    u32 bioSysCallWaitProcessWideKeyAtomic(u32 *Key, u32 *TagLocation, u32 SelfTag, u64 Timeout);
    u32 bioSysCallSignalProcessWideKey(u32 *Key, s32 Number);
    u64 bioSysCallGetSystemTick(void);
    u32 bioSysCallConnectToNamedPort(u32 *Out_IPCSession, const char *Name);
    u32 bioSysCallSendSyncRequest(u32 IPCSession);
    u32 bioSysCallSendSyncRequestWithUserBuffer(void *UserBuffer, u64 Size, u32 IPCSession);
    u32 bioSysCallSendAsyncRequestWithUserBuffer(u32 *Handle, void *UserBuffer, u64 Size, u32 IPCSession);
    u32 bioSysCallGetProcessId(u64 *Out_ProcessID, u32 VarHandle);
    u32 bioSysCallGetThreadId(u64 *Out_ThreadID, u32 VarHandle);
    u32 bioSysCallBreak(u32 BreakReason, u64 Param1, u64 Param2);
    u32 bioSysCallOutputDebugString(const char *String, u64 Size);
    void BIO_NORETURN bioSysCallReturnFromException(u32 Code);
    u32 bioSysCallGetInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID);
    u32 bioSysCallMapPhysicalMemory(void *Address, u64 Size);
    u32 bioSysCallUnmapPhysicalMemory(void *Address, u64 Size);
    u32 bioSysCallGetResourceLimitLimitValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which);
    u32 bioSysCallGetResourceLimitCurrentValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which);
    u32 bioSysCallSetThreadActivity(u32 ThreadHandle, bool Pause);
    u32 bioSysCallGetThreadContext3(bio::arm::ThreadContext *Context, u32 ThreadHandle);
    u32 bioSysCallCreateSession(u32 *ServerHandle, u32 *ClientHandle, u32 Unknown0, u64 Unknown1);
    u32 bioSysCallAcceptSession(u32 *SessionHandle, u32 PortHandle);
    u32 bioSysCallReplyAndReceive(s32 *Index, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout);
    u32 bioSysCallReplyAndReceiveWithUserBuffer(s32 *Index, void *UserBuffer, u64 Size, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout);
    u32 bioSysCallCreateEvent(u32 *ServerHandle, u32 *ClientHandle);
    u32 bioSysCallMapPhysicalMemoryUnsafe(void *Address, u64 Size);
    u32 bioSysCallUnmapPhysicalMemoryUnsafe(void *Address, u64 Size);
    u32 bioSysCallSetUnsafeLimit(u64 Size);
    u32 bioSysCallCreateCodeMemory(u32 *Out_CodeHandle, void *SourceAddress, u64 Size);
    u32 bioSysCallControlCodeMemory(u32 CodeHandle, bio::CodeMapOperation MapOperation, void *DestAddress, u64 Size, u64 Permissions);
    u32 bioSysCallReadWriteRegister(u32 *Out_Value, u64 RegisterAddress, u32 ReadWriteMask, u32 Value);
    u32 bioSysCallCreateSharedMemory(u32 *Out_Handle, size_t Size, u32 LocalPermissions, u32 OtherPermissions);
    u32 bioSysCallMapTransferMemory(u32 TransferMemory, void *Address, size_t Size, u32 Permissions);
    u32 bioSysCallUnmapTransferMemory(u32 TransferMemory, void *Address, size_t Size);
    u32 bioSysCallCreateInterruptEvent(u32 *Out_Event, u64 IRQNumber, u32 Flags);
    u32 bioSysCallQueryPhysicalAddress(u64 Out_Information[3], u64 VirtualAddress);
    u32 bioSysCallQueryIoMapping(u64 *Out_VirtualAddress, u64 PhysicalAddress, u64 Size);
    u32 bioSysCallCreateDeviceAddressSpace(u32 *Out_AddressSpace, u64 DevAddress, u64 DevSize);
    u32 bioSysCallAttachDeviceAddressSpace(u64 Device, u32 AddressSpace);
    u32 bioSysCallDetachDeviceAddressSpace(u64 Device, u32 AddressSpace);
    u32 bioSysCallMapDeviceAddressSpaceByForce(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions);
    u32 bioSysCallMapDeviceAddressSpaceAligned(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions);
    u32 bioSysCallUnmapDeviceAddressSpace(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 MapSize, u64 DevAddress);
    u32 bioSysCallDebugActiveProcess(u32 *ProcessHandle, u64 ProcessID);
    u32 bioSysCallBreakDebugProcess(u32 ProcessHandle);
    u32 bioSysCallGetDebugEvent(u8 *Out_Event, u32 ProcessHandle);
    u32 bioSysCallContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 *TitleIDs, u32 TitleIDCount);
    u32 bioSysCallLegacyContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 ThreadID);
    u32 bioSysCallGetProcessList(u32 *Out_ProcessCount, u64 *Out_ProcessIDs, u32 MaxProcessIDs);
    u32 bioSysCallGetThreadList(u32 *Out_ThreadCount, u64 *Out_TitleIDs, u32 MaxTitleIDs, u32 ProcessHandle);
    u32 bioSysCallGetDebugThreadContext(bio::arm::ThreadContext *Out_Context, u32 ProcessHandle, u64 ThreadID, u32 Flags);
    u32 bioSysCallSetDebugThreadContext(u32 ProcessHandle, u64 ThreadID, const bio::arm::ThreadContext *Out_Context, u32 Flags);
    u32 bioSysCallQueryDebugProcessMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u32 ProcessHandle, u64 Address);
    u32 bioSysCallReadDebugProcessMemory(void *Out_Buffer, u32 ProcessHandle, u64 Address, u64 Size);
    u32 bioSysCallWriteDebugProcessMemory(u32 ProcessHandle, void *Buffer, u64 Address, u64 Size);
    u32 bioSysCallGetDebugThreadParam(u64 *Out_64Bit, u32 *Out_32Bit, u32 ProcessHandle, u64 ThreadID, bio::DebugThreadParameter ThreadParam);
    u32 bioSysCallGetSystemInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID);
    u32 bioSysCallCreatePort(u32 *PortHandle, u32 *ClientHandle, s32 MaxSessions, bool IsLight, const char *Name);
    u32 bioSysCallManageNamedPort(u32 *PortHandle, const char *Name, s32 MaxSessions);
    u32 bioSysCallConnectToPort(u32 *IPCSession, u32 PortHandle);
    u32 bioSysCallSetProcessMemoryPermission(u32 ProcessHandle, u64 Address, u64 Size, u32 Permissions);
    u32 bioSysCallMapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size);
    u32 bioSysCallUnmapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size);
    u32 bioSysCallMapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size);
    u32 bioSysCallUnmapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size);
    u32 bioSysCallCreateProcess(u32 *Out_Process, void *ProcessInfo, u32 *Caps, u64 CapCount);
    u32 bioSysCallStartProcess(u32 ProcessHandle, s32 MainPriority, s32 DefaultCPU, u32 StackSize);
    u32 bioSysCallTerminateProcess(u32 ProcessHandle);
    u32 bioSysCallGetProcessInfo(u64 *Out_Value, u32 ProcessHandle, bio::ProcessInfo InfoType);
    u32 bioSysCallCreateResourceLimit(u32 *ResourceLimit);
    u32 bioSysCallSetResourceLimitLimitValue(u32 ResourceLimit, bio::LimitableResource Resource, u64 Value);
    u64 bioSysCallCallSecureMonitor(bio::SecureMonitorArgs *Args);
}

namespace bio::svc
{
    u32 SetHeapSize(void **OutAddress, u64 Size)
    {
        return bioSysCallSetHeapSize(OutAddress, Size);
    }

    u32 SetMemoryPermission(void *StartAddress, u64 Size, u32 Permissions)
    {
        return bioSysCallSetMemoryPermission(StartAddress, Size, Permissions);
    }

    u32 SetMemoryAttribute(void *StartAddress, u64 Size, u32 State0, u32 State1)
    {
        return bioSysCallSetMemoryAttribute(StartAddress, Size, State0, State1);
    }

    u32 MapMemory(void *DestAddress, void *SourceAddress, u64 Size)
    {
        return bioSysCallMapMemory(DestAddress, SourceAddress, Size);
    }
    u32 UnmapMemory(void *DestAddress, void *SourceAddress, u64 Size)
    {
        return bioSysCallUnmapMemory(DestAddress, SourceAddress, Size);
    }

    u32 QueryMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u64 Address)
    {
        return bioSysCallQueryMemory(MemInfo, PageInfo, Address);
    }
    void BIO_NORETURN ExitProcess()
    {
        bioSysCallExitProcess();
    }

    u32 CreateThread(u32 *Out_ThreadHandle, void *EntryPoint, void *Arguments, void *StackTop, int Priority, int CPUID)
    {
        return bioSysCallCreateThread(Out_ThreadHandle, EntryPoint, Arguments, StackTop, Priority, CPUID);
    }

    u32 StartThread(u32 ThreadHandle)
    {
        return bioSysCallStartThread(ThreadHandle);
    }

    void BIO_NORETURN ExitThread()
    {
        bioSysCallExitThread();
    }

    u32 SleepThread(u64 NanoSeconds)
    {
        return bioSysCallSleepThread(NanoSeconds);
    }

    u32 GetThreadPriority(u32 *Out_Priority, u32 ThreadHandle)
    {
        return bioSysCallGetThreadPriority(Out_Priority, ThreadHandle);
    }

    u32 SetThreadPriority(u32 ThreadHandle, u32 Priority)
    {
        return bioSysCallSetThreadPriority(ThreadHandle, Priority);
    }

    u32 GetThreadCoreMask(s32 *Out_PreferredCore, u32 *Out_AffinityMask, u32 ThreadHandle)
    {
        return bioSysCallGetThreadCoreMask(Out_PreferredCore, Out_AffinityMask, ThreadHandle);
    }

    u32 SetThreadCoreMask(u32 ThreadHandle, s32 PreferredCore, u32 AffinityMask)
    {
        return bioSysCallSetThreadCoreMask(ThreadHandle, PreferredCore, AffinityMask);
    }

    u32 GetCurrentProcessorNumber()
    {
        return bioSysCallGetCurrentProcessorNumber();
    }

    u32 SignalEvent(u32 EventHandle)
    {
        return bioSysCallSignalEvent(EventHandle);
    }

    u32 ClearEvent(u32 EventHandle)
    {
        return bioSysCallClearEvent(EventHandle);
    }

    u32 MapSharedMemory(u32 VarHandle, void *Address, size_t Size, u32 Permissions)
    {
        return bioSysCallMapSharedMemory(VarHandle, Address, Size, Permissions);
    }

    u32 UnmapSharedMemory(u32 VarHandle, void *Address, size_t Size)
    {
        return bioSysCallUnmapSharedMemory(VarHandle, Address, Size);
    }

    u32 CreateTransferMemory(u32 *Out_Handle, void *Address, size_t Size, u32 Permissions)
    {
        return bioSysCallCreateTransferMemory(Out_Handle, Address, Size, Permissions);
    }

    u32 CloseHandle(u32 VarHandle)
    {
        return bioSysCallCloseHandle(VarHandle);
    }

    u32 ResetSignal(u32 VarHandle)
    {
        return bioSysCallResetSignal(VarHandle);
    }

    u32 WaitSynchronization(s32 *Index, const u32 *Handles, s32 HandleCount, u64 Timeout)
    {
        return bioSysCallWaitSynchronization(Index, Handles, HandleCount, Timeout);
    }

    u32 CancelSynchronization(u32 ThreadHandle)
    {
        return bioSysCallCancelSynchronization(ThreadHandle);
    }

    u32 ArbitrateLock(u32 WaitTag, u32 *TagLocation, u32 SelfTag)
    {
        return bioSysCallArbitrateLock(WaitTag, TagLocation, SelfTag);
    }

    u32 ArbitrateUnlock(u32 *TagLocation)
    {
        return bioSysCallArbitrateUnlock(TagLocation);
    }

    u32 WaitProcessWideKeyAtomic(u32 *Key, u32 *TagLocation, u32 SelfTag, u64 Timeout)
    {
        return bioSysCallWaitProcessWideKeyAtomic(Key, TagLocation, SelfTag, Timeout);
    }

    u32 SignalProcessWideKey(u32 *Key, s32 Number)
    {
        return bioSysCallSignalProcessWideKey(Key, Number);
    }

    u64 GetSystemTick()
    {
        return bioSysCallGetSystemTick();
    }

    u32 ConnectToNamedPort(u32 *Out_IPCSession, const char *Name)
    {
        return bioSysCallConnectToNamedPort(Out_IPCSession, Name);
    }

    u32 SendSyncRequest(u32 IPCSession)
    {
        return bioSysCallSendSyncRequest(IPCSession);
    }

    u32 SendSyncRequestWithUserBuffer(void *UserBuffer, u64 Size, u32 IPCSession)
    {
        return bioSysCallSendSyncRequestWithUserBuffer(UserBuffer, Size, IPCSession);
    }

    u32 SendAsyncRequestWithUserBuffer(u32 *Handle, void *UserBuffer, u64 Size, u32 IPCSession)
    {
        return bioSysCallSendAsyncRequestWithUserBuffer(Handle, UserBuffer, Size, IPCSession);
    }

    u32 GetProcessId(u64 *Out_ProcessID, u32 VarHandle)
    {
        return bioSysCallGetProcessId(Out_ProcessID, VarHandle);
    }

    u32 GetThreadId(u64 *Out_ThreadID, u32 VarHandle)
    {
        return bioSysCallGetThreadId(Out_ThreadID, VarHandle);
    }

    u32 Break(u32 BreakReason, u64 Param1, u64 Param2)
    {
        return bioSysCallBreak(BreakReason, Param1, Param2);
    }

    u32 OutputDebugString(const char *String, u64 Size)
    {
        return bioSysCallOutputDebugString(String, Size);
    }

    void BIO_NORETURN ReturnFromException(u32 Code)
    {
        bioSysCallReturnFromException(Code);
    }

    u32 GetInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID)
    {
        return bioSysCallGetInfo(Out_Info, FirstID, VarHandle, SecondID);
    }

    u32 MapPhysicalMemory(void *Address, u64 Size)
    {
        return bioSysCallMapPhysicalMemory(Address, Size);
    }

    u32 UnmapPhysicalMemory(void *Address, u64 Size)
    {
        return bioSysCallUnmapPhysicalMemory(Address, Size);
    }

    u32 GetResourceLimitLimitValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which)
    {
        return bioSysCallGetResourceLimitLimitValue(Out_Value, ResourceLimit, Which);
    }

    u32 GetResourceLimitCurrentValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which)
    {
        return bioSysCallGetResourceLimitCurrentValue(Out_Value, ResourceLimit, Which);
    }

    u32 SetThreadActivity(u32 ThreadHandle, bool Pause)
    {
        return bioSysCallSetThreadActivity(ThreadHandle, Pause);
    }

    u32 GetThreadContext3(bio::arm::ThreadContext *Context, u32 ThreadHandle)
    {
        return bioSysCallGetThreadContext3(Context, ThreadHandle);
    }

    u32 CreateSession(u32 *ServerHandle, u32 *ClientHandle, u32 Unknown0, u64 Unknown1)
    {
        return bioSysCallCreateSession(ServerHandle, ClientHandle, Unknown0, Unknown1);
    }

    u32 AcceptSession(u32 *SessionHandle, u32 PortHandle)
    {
        return bioSysCallAcceptSession(SessionHandle, PortHandle);
    }

    u32 ReplyAndReceive(s32 *Index, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout)
    {
        return bioSysCallReplyAndReceive(Index, Handles, HandleCount, ReplyTarget, Timeout);
    }

    u32 ReplyAndReceiveWithUserBuffer(s32 *Index, void *UserBuffer, u64 Size, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout)
    {
        return bioSysCallReplyAndReceiveWithUserBuffer(Index, UserBuffer, Size, Handles, HandleCount, ReplyTarget, Timeout);
    }

    u32 CreateEvent(u32 *ServerHandle, u32 *ClientHandle)
    {
        return bioSysCallCreateEvent(ServerHandle, ClientHandle);
    }

    u32 MapPhysicalMemoryUnsafe(void *Address, u64 Size)
    {
       return bioSysCallMapPhysicalMemoryUnsafe(Address, Size); 
    }

    u32 UnmapPhysicalMemoryUnsafe(void *Address, u64 Size)
    {
        return bioSysCallUnmapPhysicalMemoryUnsafe(Address, Size);
    }

    u32 SetUnsafeLimit(u64 Size)
    {
        return bioSysCallSetUnsafeLimit(Size);
    }

    u32 CreateCodeMemory(u32 *Out_CodeHandle, void *SourceAddress, u64 Size)
    {
        return bioSysCallCreateCodeMemory(Out_CodeHandle, SourceAddress, Size);
    }

    u32 ControlCodeMemory(u32 CodeHandle, bio::CodeMapOperation MapOperation, void *DestAddress, u64 Size, u64 Permissions)
    {
        return bioSysCallControlCodeMemory(CodeHandle, MapOperation, DestAddress, Size, Permissions);
    }

    u32 ReadWriteRegister(u32 *Out_Value, u64 RegisterAddress, u32 ReadWriteMask, u32 Value)
    {
        return bioSysCallReadWriteRegister(Out_Value, RegisterAddress, ReadWriteMask, Value);
    }

    u32 CreateSharedMemory(u32 *Out_Handle, size_t Size, u32 LocalPermissions, u32 OtherPermissions)
    {
        return bioSysCallCreateSharedMemory(Out_Handle, Size, LocalPermissions, OtherPermissions);
    }

    u32 MapTransferMemory(u32 TransferMemory, void *Address, size_t Size, u32 Permissions)
    {
        return bioSysCallMapTransferMemory(TransferMemory, Address, Size, Permissions);
    }

    u32 UnmapTransferMemory(u32 TransferMemory, void *Address, size_t Size)
    {
        return bioSysCallUnmapTransferMemory(TransferMemory, Address, Size);
    }

    u32 CreateInterruptEvent(u32 *Out_Event, u64 IRQNumber, u32 Flags)
    {
        return bioSysCallCreateInterruptEvent(Out_Event, IRQNumber, Flags);
    }

    u32 QueryPhysicalAddress(u64 Out_Information[3], u64 VirtualAddress)
    {
        return bioSysCallQueryPhysicalAddress(Out_Information, VirtualAddress);
    }

    u32 QueryIoMapping(u64 *Out_VirtualAddress, u64 PhysicalAddress, u64 Size)
    {
        return bioSysCallQueryIoMapping(Out_VirtualAddress, PhysicalAddress, Size);
    }

    u32 CreateDeviceAddressSpace(u32 *Out_AddressSpace, u64 DevAddress, u64 DevSize)
    {
        return bioSysCallCreateDeviceAddressSpace(Out_AddressSpace, DevAddress, DevSize);
    }

    u32 AttachDeviceAddressSpace(u64 Device, u32 AddressSpace)
    {
        return bioSysCallAttachDeviceAddressSpace(Device, AddressSpace);
    }

    u32 DetachDeviceAddressSpace(u64 Device, u32 AddressSpace)
    {
        return bioSysCallDetachDeviceAddressSpace(Device, AddressSpace);
    }

    u32 MapDeviceAddressSpaceByForce(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions)
    {
        return bioSysCallMapDeviceAddressSpaceByForce(DeviceAddress, ProcessHandle, MapAddress, DevSize, DevAddress, Permissions);
    }

    u32 MapDeviceAddressSpaceAligned(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions)
    {
        return bioSysCallMapDeviceAddressSpaceAligned(DeviceAddress, ProcessHandle, MapAddress, DevSize, DevAddress, Permissions);
    }

    u32 UnmapDeviceAddressSpace(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 MapSize, u64 DevAddress)
    {
        return bioSysCallUnmapDeviceAddressSpace(DeviceAddress, ProcessHandle, MapAddress, MapSize, DevAddress);
    }

    u32 DebugActiveProcess(u32 *ProcessHandle, u64 ProcessID)
    {
        return bioSysCallDebugActiveProcess(ProcessHandle, ProcessID);
    }

    u32 BreakDebugProcess(u32 ProcessHandle)
    {
        return bioSysCallBreakDebugProcess(ProcessHandle);
    }

    u32 GetDebugEvent(u8 *Out_Event, u32 ProcessHandle)
    {
        return bioSysCallGetDebugEvent(Out_Event, ProcessHandle);
    }

    u32 ContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 *TitleIDs, u32 TitleIDCount)
    {
        return bioSysCallContinueDebugEvent(ProcessHandle, Flags, TitleIDs, TitleIDCount);
    }

    u32 LegacyContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 ThreadID)
    {
        return bioSysCallLegacyContinueDebugEvent(ProcessHandle, Flags, ThreadID);
    }

    u32 GetProcessList(u32 *Out_ProcessCount, u64 *Out_ProcessIDs, u32 MaxProcessIDs)
    {
        return bioSysCallGetProcessList(Out_ProcessCount, Out_ProcessIDs, MaxProcessIDs);
    }

    u32 GetThreadList(u32 *Out_ThreadCount, u64 *Out_TitleIDs, u32 MaxTitleIDs, u32 ProcessHandle)
    {
        return bioSysCallGetThreadList(Out_ThreadCount, Out_TitleIDs, MaxTitleIDs, ProcessHandle);
    }

    u32 GetDebugThreadContext(bio::arm::ThreadContext *Out_Context, u32 ProcessHandle, u64 ThreadID, u32 Flags)
    {
        return bioSysCallGetDebugThreadContext(Out_Context, ProcessHandle, ThreadID, Flags);
    }

    u32 SetDebugThreadContext(u32 ProcessHandle, u64 ThreadID, const bio::arm::ThreadContext *Out_Context, u32 Flags)
    {
        return bioSysCallSetDebugThreadContext(ProcessHandle, ThreadID, Out_Context, Flags);
    }

    u32 QueryDebugProcessMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u32 ProcessHandle, u64 Address)
    {
        return bioSysCallQueryDebugProcessMemory(MemInfo, PageInfo, ProcessHandle, Address);
    }

    u32 ReadDebugProcessMemory(void *Out_Buffer, u32 ProcessHandle, u64 Address, u64 Size)
    {
        return bioSysCallReadDebugProcessMemory(Out_Buffer, ProcessHandle, Address, Size);
    }

    u32 WriteDebugProcessMemory(u32 ProcessHandle, void *Buffer, u64 Address, u64 Size)
    {
        return bioSysCallWriteDebugProcessMemory(ProcessHandle, Buffer, Address, Size);
    }

    u32 GetDebugThreadParam(u64 *Out_64Bit, u32 *Out_32Bit, u32 ProcessHandle, u64 ThreadID, bio::DebugThreadParameter ThreadParam)
    {
        return bioSysCallGetDebugThreadParam(Out_64Bit, Out_32Bit, ProcessHandle, ThreadID, ThreadParam);
    }

    u32 GetSystemInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID)
    {
        return bioSysCallGetSystemInfo(Out_Info, FirstID, VarHandle, SecondID);
    }

    u32 CreatePort(u32 *PortHandle, u32 *ClientHandle, s32 MaxSessions, bool IsLight, const char *Name)
    {
        return bioSysCallCreatePort(PortHandle, ClientHandle, MaxSessions, IsLight, Name);
    }

    u32 ManageNamedPort(u32 *PortHandle, const char *Name, s32 MaxSessions)
    {
        return bioSysCallManageNamedPort(PortHandle, Name, MaxSessions);
    }

    u32 ConnectToPort(u32 *IPCSession, u32 PortHandle)
    {
        return bioSysCallConnectToPort(IPCSession, PortHandle);
    }

    u32 SetProcessMemoryPermission(u32 ProcessHandle, u64 Address, u64 Size, u32 Permissions)
    {
        return bioSysCallSetProcessMemoryPermission(ProcessHandle, Address, Size, Permissions);
    }

    u32 MapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size)
    {
        return bioSysCallMapProcessMemory(DestAddress, ProcessHandle, SourceAddress, Size);
    }

    u32 UnmapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size)
    {
        return bioSysCallUnmapProcessMemory(DestAddress, ProcessHandle, SourceAddress, Size);
    }

    u32 MapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size)
    {
        return bioSysCallMapProcessCodeMemory(ProcessHandle, DestAddress, SourceAddress, Size);
    }

    u32 UnmapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size)
    {
        return bioSysCallUnmapProcessCodeMemory(ProcessHandle, DestAddress, SourceAddress, Size);
    }

    u32 CreateProcess(u32 *Out_Process, void *ProcessInfo, u32 *Caps, u64 CapCount)
    {
        return bioSysCallCreateProcess(Out_Process, ProcessInfo, Caps, CapCount);
    }

    u32 StartProcess(u32 ProcessHandle, s32 MainPriority, s32 DefaultCPU, u32 StackSize)
    {
        return bioSysCallStartProcess(ProcessHandle, MainPriority, DefaultCPU, StackSize);
    }

    u32 TerminateProcess(u32 ProcessHandle)
    {
        return bioSysCallTerminateProcess(ProcessHandle);
    }
    u32 GetProcessInfo(u64 *Out_Value, u32 ProcessHandle, bio::ProcessInfo InfoType)
    {
        return bioSysCallGetProcessInfo(Out_Value, ProcessHandle, InfoType);
    }

    u32 CreateResourceLimit(u32 *ResourceLimit)
    {
        return bioSysCallCreateResourceLimit(ResourceLimit);
    }

    u32 SetResourceLimitLimitValue(u32 ResourceLimit, bio::LimitableResource Resource, u64 Value)
    {
        return bioSysCallSetResourceLimitLimitValue(ResourceLimit, Resource, Value);
    }

    u64 CallSecureMonitor(bio::SecureMonitorArgs *Args)
    {
        return bioSysCallCallSecureMonitor(Args);
    }
}