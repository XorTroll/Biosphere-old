#include <bio/biotope/svc/SysCalls.hpp>

extern "C"
{
    u32 SVC_SetHeapSize(void **OutAddress, u64 Size);
    u32 SVC_SetMemoryPermission(void *StartAddress, u64 Size, u32 Permissions);
    u32 SVC_SetMemoryAttribute(void *StartAddress, u64 Size, u32 State0, u32 State1);
    u32 SVC_MapMemory(void *DestAddress, void *SourceAddress, u64 Size);
    u32 SVC_UnmapMemory(void *DestAddress, void *SourceAddress, u64 Size);
    u32 SVC_QueryMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u64 Address);
    void BIO_NORETURN SVC_ExitProcess(void);
    u32 SVC_CreateThread(u32 *Out_ThreadHandle, void *EntryPoint, void *Arguments, void *StackTop, int Priority, int CPUID);
    u32 SVC_StartThread(u32 ThreadHandle);
    void BIO_NORETURN SVC_ExitThread(void);
    u32 SVC_SleepThread(u64 NanoSeconds);
    u32 SVC_GetThreadPriority(u32 *Out_Priority, u32 ThreadHandle);
    u32 SVC_SetThreadPriority(u32 ThreadHandle, u32 Priority);
    u32 SVC_GetThreadCoreMask(s32 *Out_PreferredCore, u32 *Out_AffinityMask, u32 ThreadHandle);
    u32 SVC_SetThreadCoreMask(u32 ThreadHandle, s32 PreferredCore, u32 AffinityMask);
    u32 SVC_GetCurrentProcessorNumber(void);
    u32 SVC_SignalEvent(u32 EventHandle);
    u32 SVC_ClearEvent(u32 EventHandle);
    u32 SVC_MapSharedMemory(u32 VarHandle, void *Address, size_t Size, u32 Permissions);
    u32 SVC_UnmapSharedMemory(u32 VarHandle, void *Address, size_t Size);
    u32 SVC_CreateTransferMemory(u32 *Out_Handle, void *Address, size_t Size, u32 Permissions);
    u32 SVC_CloseHandle(u32 VarHandle);
    u32 SVC_ResetSignal(u32 VarHandle);
    u32 SVC_WaitSynchronization(s32 *Index, const u32 *Handles, s32 HandleCount, u64 Timeout);
    u32 SVC_CancelSynchronization(u32 ThreadHandle);
    u32 SVC_ArbitrateLock(u32 WaitTag, u32 *TagLocation, u32 SelfTag);
    u32 SVC_ArbitrateUnlock(u32 *TagLocation);
    u32 SVC_WaitProcessWideKeyAtomic(u32 *Key, u32 *TagLocation, u32 SelfTag, u64 Timeout);
    u32 SVC_SignalProcessWideKey(u32 *Key, s32 Number);
    u64 SVC_GetSystemTick(void);
    u32 SVC_ConnectToNamedPort(u32 *Out_IPCSession, const char *Name);
    u32 SVC_SendSyncRequest(u32 IPCSession);
    u32 SVC_SendSyncRequestWithUserBuffer(void *UserBuffer, u64 Size, u32 IPCSession);
    u32 SVC_SendAsyncRequestWithUserBuffer(u32 *Handle, void *UserBuffer, u64 Size, u32 IPCSession);
    u32 SVC_GetProcessId(u64 *Out_ProcessID, u32 VarHandle);
    u32 SVC_GetThreadId(u64 *Out_ThreadID, u32 VarHandle);
    u32 SVC_Break(u32 BreakReason, u64 Param1, u64 Param2);
    u32 SVC_OutputDebugString(const char *String, u64 Size);
    void BIO_NORETURN SVC_ReturnFromException(u32 Code);
    u32 SVC_GetInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID);
    u32 SVC_MapPhysicalMemory(void *Address, u64 Size);
    u32 SVC_UnmapPhysicalMemory(void *Address, u64 Size);
    u32 SVC_GetResourceLimitLimitValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which);
    u32 SVC_GetResourceLimitCurrentValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which);
    u32 SVC_SetThreadActivity(u32 ThreadHandle, bool Pause);
    u32 SVC_GetThreadContext3(bio::arm::ThreadContext *Context, u32 ThreadHandle);
    u32 SVC_CreateSession(u32 *ServerHandle, u32 *ClientHandle, u32 Unknown0, u64 Unknown1);
    u32 SVC_AcceptSession(u32 *SessionHandle, u32 PortHandle);
    u32 SVC_ReplyAndReceive(s32 *Index, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout);
    u32 SVC_ReplyAndReceiveWithUserBuffer(s32 *Index, void *UserBuffer, u64 Size, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout);
    u32 SVC_CreateEvent(u32 *ServerHandle, u32 *ClientHandle);
    u32 SVC_MapPhysicalMemoryUnsafe(void *Address, u64 Size);
    u32 SVC_UnmapPhysicalMemoryUnsafe(void *Address, u64 Size);
    u32 SVC_SetUnsafeLimit(u64 Size);
    u32 SVC_CreateCodeMemory(u32 *Out_CodeHandle, void *SourceAddress, u64 Size);
    u32 SVC_ControlCodeMemory(u32 CodeHandle, bio::CodeMapOperation MapOperation, void *DestAddress, u64 Size, u64 Permissions);
    u32 SVC_ReadWriteRegister(u32 *Out_Value, u64 RegisterAddress, u32 ReadWriteMask, u32 Value);
    u32 SVC_CreateSharedMemory(u32 *Out_Handle, size_t Size, u32 LocalPermissions, u32 OtherPermissions);
    u32 SVC_MapTransferMemory(u32 TransferMemory, void *Address, size_t Size, u32 Permissions);
    u32 SVC_UnmapTransferMemory(u32 TransferMemory, void *Address, size_t Size);
    u32 SVC_CreateInterruptEvent(u32 *Out_Event, u64 IRQNumber, u32 Flags);
    u32 SVC_QueryPhysicalAddress(u64 Out_Information[3], u64 VirtualAddress);
    u32 SVC_QueryIoMapping(u64 *Out_VirtualAddress, u64 PhysicalAddress, u64 Size);
    u32 SVC_CreateDeviceAddressSpace(u32 *Out_AddressSpace, u64 DevAddress, u64 DevSize);
    u32 SVC_AttachDeviceAddressSpace(u64 Device, u32 AddressSpace);
    u32 SVC_DetachDeviceAddressSpace(u64 Device, u32 AddressSpace);
    u32 SVC_MapDeviceAddressSpaceByForce(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions);
    u32 SVC_MapDeviceAddressSpaceAligned(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions);
    u32 SVC_UnmapDeviceAddressSpace(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 MapSize, u64 DevAddress);
    u32 SVC_DebugActiveProcess(u32 *ProcessHandle, u64 ProcessID);
    u32 SVC_BreakDebugProcess(u32 ProcessHandle);
    u32 SVC_GetDebugEvent(u8 *Out_Event, u32 ProcessHandle);
    u32 SVC_ContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 *TitleIDs, u32 TitleIDCount);
    u32 SVC_LegacyContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 ThreadID);
    u32 SVC_GetProcessList(u32 *Out_ProcessCount, u64 *Out_ProcessIDs, u32 MaxProcessIDs);
    u32 SVC_GetThreadList(u32 *Out_ThreadCount, u64 *Out_TitleIDs, u32 MaxTitleIDs, u32 ProcessHandle);
    u32 SVC_GetDebugThreadContext(bio::arm::ThreadContext *Out_Context, u32 ProcessHandle, u64 ThreadID, u32 Flags);
    u32 SVC_SetDebugThreadContext(u32 ProcessHandle, u64 ThreadID, const bio::arm::ThreadContext *Out_Context, u32 Flags);
    u32 SVC_QueryDebugProcessMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u32 ProcessHandle, u64 Address);
    u32 SVC_ReadDebugProcessMemory(void *Out_Buffer, u32 ProcessHandle, u64 Address, u64 Size);
    u32 SVC_WriteDebugProcessMemory(u32 ProcessHandle, void *Buffer, u64 Address, u64 Size);
    u32 SVC_GetDebugThreadParam(u64 *Out_64Bit, u32 *Out_32Bit, u32 ProcessHandle, u64 ThreadID, bio::DebugThreadParameter ThreadParam);
    u32 SVC_GetSystemInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID);
    u32 SVC_CreatePort(u32 *PortHandle, u32 *ClientHandle, s32 MaxSessions, bool IsLight, const char *Name);
    u32 SVC_ManageNamedPort(u32 *PortHandle, const char *Name, s32 MaxSessions);
    u32 SVC_ConnectToPort(u32 *IPCSession, u32 PortHandle);
    u32 SVC_SetProcessMemoryPermission(u32 ProcessHandle, u64 Address, u64 Size, u32 Permissions);
    u32 SVC_MapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size);
    u32 SVC_UnmapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size);
    u32 SVC_MapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size);
    u32 SVC_UnmapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size);
    u32 SVC_CreateProcess(u32 *Out_Process, void *ProcessInfo, u32 *Caps, u64 CapCount);
    u32 SVC_StartProcess(u32 ProcessHandle, s32 MainPriority, s32 DefaultCPU, u32 StackSize);
    u32 SVC_TerminateProcess(u32 ProcessHandle);
    u32 SVC_GetProcessInfo(u64 *Out_Value, u32 ProcessHandle, bio::ProcessInfo InfoType);
    u32 SVC_CreateResourceLimit(u32 *ResourceLimit);
    u32 SVC_SetResourceLimitLimitValue(u32 ResourceLimit, bio::LimitableResource Resource, u64 Value);
    u64 SVC_CallSecureMonitor(bio::SecureMonitorArgs *Args);
}

namespace bio::svc
{
    u32 SetHeapSize(void **OutAddress, u64 Size)
    {
        return SVC_SetHeapSize(OutAddress, Size);
    }

    u32 SetMemoryPermission(void *StartAddress, u64 Size, u32 Permissions)
    {
        return SVC_SetMemoryPermission(StartAddress, Size, Permissions);
    }

    u32 SetMemoryAttribute(void *StartAddress, u64 Size, u32 State0, u32 State1)
    {
        return SVC_SetMemoryAttribute(StartAddress, Size, State0, State1);
    }

    u32 MapMemory(void *DestAddress, void *SourceAddress, u64 Size)
    {
        return SVC_MapMemory(DestAddress, SourceAddress, Size);
    }
    u32 UnmapMemory(void *DestAddress, void *SourceAddress, u64 Size)
    {
        return SVC_UnmapMemory(DestAddress, SourceAddress, Size);
    }

    u32 QueryMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u64 Address)
    {
        return SVC_QueryMemory(MemInfo, PageInfo, Address);
    }
    void BIO_NORETURN ExitProcess()
    {
        SVC_ExitProcess();
    }

    u32 CreateThread(u32 *Out_ThreadHandle, void *EntryPoint, void *Arguments, void *StackTop, int Priority, int CPUID)
    {
        return SVC_CreateThread(Out_ThreadHandle, EntryPoint, Arguments, StackTop, Priority, CPUID);
    }

    u32 StartThread(u32 ThreadHandle)
    {
        return SVC_StartThread(ThreadHandle);
    }

    void BIO_NORETURN ExitThread()
    {
        SVC_ExitThread();
    }

    u32 SleepThread(u64 NanoSeconds)
    {
        return SVC_SleepThread(NanoSeconds);
    }

    u32 GetThreadPriority(u32 *Out_Priority, u32 ThreadHandle)
    {
        return SVC_GetThreadPriority(Out_Priority, ThreadHandle);
    }

    u32 SetThreadPriority(u32 ThreadHandle, u32 Priority)
    {
        return SVC_SetThreadPriority(ThreadHandle, Priority);
    }

    u32 GetThreadCoreMask(s32 *Out_PreferredCore, u32 *Out_AffinityMask, u32 ThreadHandle)
    {
        return SVC_GetThreadCoreMask(Out_PreferredCore, Out_AffinityMask, ThreadHandle);
    }

    u32 SetThreadCoreMask(u32 ThreadHandle, s32 PreferredCore, u32 AffinityMask)
    {
        return SVC_SetThreadCoreMask(ThreadHandle, PreferredCore, AffinityMask);
    }

    u32 GetCurrentProcessorNumber()
    {
        return SVC_GetCurrentProcessorNumber();
    }

    u32 SignalEvent(u32 EventHandle)
    {
        return SVC_SignalEvent(EventHandle);
    }

    u32 ClearEvent(u32 EventHandle)
    {
        return SVC_ClearEvent(EventHandle);
    }

    u32 MapSharedMemory(u32 VarHandle, void *Address, size_t Size, u32 Permissions)
    {
        return SVC_MapSharedMemory(VarHandle, Address, Size, Permissions);
    }

    u32 UnmapSharedMemory(u32 VarHandle, void *Address, size_t Size)
    {
        return SVC_UnmapSharedMemory(VarHandle, Address, Size);
    }

    u32 CreateTransferMemory(u32 *Out_Handle, void *Address, size_t Size, u32 Permissions)
    {
        return SVC_CreateTransferMemory(Out_Handle, Address, Size, Permissions);
    }

    u32 CloseHandle(u32 VarHandle)
    {
        return SVC_CloseHandle(VarHandle);
    }

    u32 ResetSignal(u32 VarHandle)
    {
        return SVC_ResetSignal(VarHandle);
    }

    u32 WaitSynchronization(s32 *Index, const u32 *Handles, s32 HandleCount, u64 Timeout)
    {
        return SVC_WaitSynchronization(Index, Handles, HandleCount, Timeout);
    }

    u32 CancelSynchronization(u32 ThreadHandle)
    {
        return SVC_CancelSynchronization(ThreadHandle);
    }

    u32 ArbitrateLock(u32 WaitTag, u32 *TagLocation, u32 SelfTag)
    {
        return SVC_ArbitrateLock(WaitTag, TagLocation, SelfTag);
    }

    u32 ArbitrateUnlock(u32 *TagLocation)
    {
        return SVC_ArbitrateUnlock(TagLocation);
    }

    u32 WaitProcessWideKeyAtomic(u32 *Key, u32 *TagLocation, u32 SelfTag, u64 Timeout)
    {
        return SVC_WaitProcessWideKeyAtomic(Key, TagLocation, SelfTag, Timeout);
    }

    u32 SignalProcessWideKey(u32 *Key, s32 Number)
    {
        return SVC_SignalProcessWideKey(Key, Number);
    }

    u64 GetSystemTick()
    {
        return SVC_GetSystemTick();
    }

    u32 ConnectToNamedPort(u32 *Out_IPCSession, const char *Name)
    {
        return SVC_ConnectToNamedPort(Out_IPCSession, Name);
    }

    u32 SendSyncRequest(u32 IPCSession)
    {
        return SVC_SendSyncRequest(IPCSession);
    }

    u32 SendSyncRequestWithUserBuffer(void *UserBuffer, u64 Size, u32 IPCSession)
    {
        return SVC_SendSyncRequestWithUserBuffer(UserBuffer, Size, IPCSession);
    }

    u32 SendAsyncRequestWithUserBuffer(u32 *Handle, void *UserBuffer, u64 Size, u32 IPCSession)
    {
        return SVC_SendAsyncRequestWithUserBuffer(Handle, UserBuffer, Size, IPCSession);
    }

    u32 GetProcessId(u64 *Out_ProcessID, u32 VarHandle)
    {
        return SVC_GetProcessId(Out_ProcessID, VarHandle);
    }

    u32 GetThreadId(u64 *Out_ThreadID, u32 VarHandle)
    {
        return SVC_GetThreadId(Out_ThreadID, VarHandle);
    }

    u32 Break(u32 BreakReason, u64 Param1, u64 Param2)
    {
        return SVC_Break(BreakReason, Param1, Param2);
    }

    u32 OutputDebugString(const char *String, u64 Size)
    {
        return SVC_OutputDebugString(String, Size);
    }

    void BIO_NORETURN ReturnFromException(u32 Code)
    {
        SVC_ReturnFromException(Code);
    }

    u32 GetInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID)
    {
        return SVC_GetInfo(Out_Info, FirstID, VarHandle, SecondID);
    }

    u32 MapPhysicalMemory(void *Address, u64 Size)
    {
        return SVC_MapPhysicalMemory(Address, Size);
    }

    u32 UnmapPhysicalMemory(void *Address, u64 Size)
    {
        return SVC_UnmapPhysicalMemory(Address, Size);
    }

    u32 GetResourceLimitLimitValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which)
    {
        return SVC_GetResourceLimitLimitValue(Out_Value, ResourceLimit, Which);
    }

    u32 GetResourceLimitCurrentValue(u64 *Out_Value, u32 ResourceLimit, bio::LimitableResource Which)
    {
        return SVC_GetResourceLimitCurrentValue(Out_Value, ResourceLimit, Which);
    }

    u32 SetThreadActivity(u32 ThreadHandle, bool Pause)
    {
        return SVC_SetThreadActivity(ThreadHandle, Pause);
    }

    u32 GetThreadContext3(bio::arm::ThreadContext *Context, u32 ThreadHandle)
    {
        return SVC_GetThreadContext3(Context, ThreadHandle);
    }

    u32 CreateSession(u32 *ServerHandle, u32 *ClientHandle, u32 Unknown0, u64 Unknown1)
    {
        return SVC_CreateSession(ServerHandle, ClientHandle, Unknown0, Unknown1);
    }

    u32 AcceptSession(u32 *SessionHandle, u32 PortHandle)
    {
        return SVC_AcceptSession(SessionHandle, PortHandle);
    }

    u32 ReplyAndReceive(s32 *Index, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout)
    {
        return SVC_ReplyAndReceive(Index, Handles, HandleCount, ReplyTarget, Timeout);
    }

    u32 ReplyAndReceiveWithUserBuffer(s32 *Index, void *UserBuffer, u64 Size, const u32 *Handles, s32 HandleCount, u32 ReplyTarget, u64 Timeout)
    {
        return SVC_ReplyAndReceiveWithUserBuffer(Index, UserBuffer, Size, Handles, HandleCount, ReplyTarget, Timeout);
    }

    u32 CreateEvent(u32 *ServerHandle, u32 *ClientHandle)
    {
        return SVC_CreateEvent(ServerHandle, ClientHandle);
    }

    u32 MapPhysicalMemoryUnsafe(void *Address, u64 Size)
    {
       return SVC_MapPhysicalMemoryUnsafe(Address, Size); 
    }

    u32 UnmapPhysicalMemoryUnsafe(void *Address, u64 Size)
    {
        return SVC_UnmapPhysicalMemoryUnsafe(Address, Size);
    }

    u32 SetUnsafeLimit(u64 Size)
    {
        return SVC_SetUnsafeLimit(Size);
    }

    u32 CreateCodeMemory(u32 *Out_CodeHandle, void *SourceAddress, u64 Size)
    {
        return SVC_CreateCodeMemory(Out_CodeHandle, SourceAddress, Size);
    }

    u32 ControlCodeMemory(u32 CodeHandle, bio::CodeMapOperation MapOperation, void *DestAddress, u64 Size, u64 Permissions)
    {
        return SVC_ControlCodeMemory(CodeHandle, MapOperation, DestAddress, Size, Permissions);
    }

    u32 ReadWriteRegister(u32 *Out_Value, u64 RegisterAddress, u32 ReadWriteMask, u32 Value)
    {
        return SVC_ReadWriteRegister(Out_Value, RegisterAddress, ReadWriteMask, Value);
    }

    u32 CreateSharedMemory(u32 *Out_Handle, size_t Size, u32 LocalPermissions, u32 OtherPermissions)
    {
        return SVC_CreateSharedMemory(Out_Handle, Size, LocalPermissions, OtherPermissions);
    }

    u32 MapTransferMemory(u32 TransferMemory, void *Address, size_t Size, u32 Permissions)
    {
        return SVC_MapTransferMemory(TransferMemory, Address, Size, Permissions);
    }

    u32 UnmapTransferMemory(u32 TransferMemory, void *Address, size_t Size)
    {
        return SVC_UnmapTransferMemory(TransferMemory, Address, Size);
    }

    u32 CreateInterruptEvent(u32 *Out_Event, u64 IRQNumber, u32 Flags)
    {
        return SVC_CreateInterruptEvent(Out_Event, IRQNumber, Flags);
    }

    u32 QueryPhysicalAddress(u64 Out_Information[3], u64 VirtualAddress)
    {
        return SVC_QueryPhysicalAddress(Out_Information, VirtualAddress);
    }

    u32 QueryIoMapping(u64 *Out_VirtualAddress, u64 PhysicalAddress, u64 Size)
    {
        return SVC_QueryIoMapping(Out_VirtualAddress, PhysicalAddress, Size);
    }

    u32 CreateDeviceAddressSpace(u32 *Out_AddressSpace, u64 DevAddress, u64 DevSize)
    {
        return SVC_CreateDeviceAddressSpace(Out_AddressSpace, DevAddress, DevSize);
    }

    u32 AttachDeviceAddressSpace(u64 Device, u32 AddressSpace)
    {
        return SVC_AttachDeviceAddressSpace(Device, AddressSpace);
    }

    u32 DetachDeviceAddressSpace(u64 Device, u32 AddressSpace)
    {
        return SVC_DetachDeviceAddressSpace(Device, AddressSpace);
    }

    u32 MapDeviceAddressSpaceByForce(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions)
    {
        return SVC_MapDeviceAddressSpaceByForce(DeviceAddress, ProcessHandle, MapAddress, DevSize, DevAddress, Permissions);
    }

    u32 MapDeviceAddressSpaceAligned(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 DevSize, u64 DevAddress, u32 Permissions)
    {
        return SVC_MapDeviceAddressSpaceAligned(DeviceAddress, ProcessHandle, MapAddress, DevSize, DevAddress, Permissions);
    }

    u32 UnmapDeviceAddressSpace(u32 DeviceAddress, u32 ProcessHandle, u64 MapAddress, u64 MapSize, u64 DevAddress)
    {
        return SVC_UnmapDeviceAddressSpace(DeviceAddress, ProcessHandle, MapAddress, MapSize, DevAddress);
    }

    u32 DebugActiveProcess(u32 *ProcessHandle, u64 ProcessID)
    {
        return SVC_DebugActiveProcess(ProcessHandle, ProcessID);
    }

    u32 BreakDebugProcess(u32 ProcessHandle)
    {
        return SVC_BreakDebugProcess(ProcessHandle);
    }

    u32 GetDebugEvent(u8 *Out_Event, u32 ProcessHandle)
    {
        return SVC_GetDebugEvent(Out_Event, ProcessHandle);
    }

    u32 ContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 *TitleIDs, u32 TitleIDCount)
    {
        return SVC_ContinueDebugEvent(ProcessHandle, Flags, TitleIDs, TitleIDCount);
    }

    u32 LegacyContinueDebugEvent(u32 ProcessHandle, u32 Flags, u64 ThreadID)
    {
        return SVC_LegacyContinueDebugEvent(ProcessHandle, Flags, ThreadID);
    }

    u32 GetProcessList(u32 *Out_ProcessCount, u64 *Out_ProcessIDs, u32 MaxProcessIDs)
    {
        return SVC_GetProcessList(Out_ProcessCount, Out_ProcessIDs, MaxProcessIDs);
    }

    u32 GetThreadList(u32 *Out_ThreadCount, u64 *Out_TitleIDs, u32 MaxTitleIDs, u32 ProcessHandle)
    {
        return SVC_GetThreadList(Out_ThreadCount, Out_TitleIDs, MaxTitleIDs, ProcessHandle);
    }

    u32 GetDebugThreadContext(bio::arm::ThreadContext *Out_Context, u32 ProcessHandle, u64 ThreadID, u32 Flags)
    {
        return SVC_GetDebugThreadContext(Out_Context, ProcessHandle, ThreadID, Flags);
    }

    u32 SetDebugThreadContext(u32 ProcessHandle, u64 ThreadID, const bio::arm::ThreadContext *Out_Context, u32 Flags)
    {
        return SVC_SetDebugThreadContext(ProcessHandle, ThreadID, Out_Context, Flags);
    }

    u32 QueryDebugProcessMemory(bio::MemoryInfo *MemInfo, u32 *PageInfo, u32 ProcessHandle, u64 Address)
    {
        return SVC_QueryDebugProcessMemory(MemInfo, PageInfo, ProcessHandle, Address);
    }

    u32 ReadDebugProcessMemory(void *Out_Buffer, u32 ProcessHandle, u64 Address, u64 Size)
    {
        return SVC_ReadDebugProcessMemory(Out_Buffer, ProcessHandle, Address, Size);
    }

    u32 WriteDebugProcessMemory(u32 ProcessHandle, void *Buffer, u64 Address, u64 Size)
    {
        return SVC_WriteDebugProcessMemory(ProcessHandle, Buffer, Address, Size);
    }

    u32 GetDebugThreadParam(u64 *Out_64Bit, u32 *Out_32Bit, u32 ProcessHandle, u64 ThreadID, bio::DebugThreadParameter ThreadParam)
    {
        return SVC_GetDebugThreadParam(Out_64Bit, Out_32Bit, ProcessHandle, ThreadID, ThreadParam);
    }

    u32 GetSystemInfo(u64 *Out_Info, u64 FirstID, u32 VarHandle, u64 SecondID)
    {
        return SVC_GetSystemInfo(Out_Info, FirstID, VarHandle, SecondID);
    }

    u32 CreatePort(u32 *PortHandle, u32 *ClientHandle, s32 MaxSessions, bool IsLight, const char *Name)
    {
        return SVC_CreatePort(PortHandle, ClientHandle, MaxSessions, IsLight, Name);
    }

    u32 ManageNamedPort(u32 *PortHandle, const char *Name, s32 MaxSessions)
    {
        return SVC_ManageNamedPort(PortHandle, Name, MaxSessions);
    }

    u32 ConnectToPort(u32 *IPCSession, u32 PortHandle)
    {
        return SVC_ConnectToPort(IPCSession, PortHandle);
    }

    u32 SetProcessMemoryPermission(u32 ProcessHandle, u64 Address, u64 Size, u32 Permissions)
    {
        return SVC_SetProcessMemoryPermission(ProcessHandle, Address, Size, Permissions);
    }

    u32 MapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size)
    {
        return SVC_MapProcessMemory(DestAddress, ProcessHandle, SourceAddress, Size);
    }

    u32 UnmapProcessMemory(void *DestAddress, u32 ProcessHandle, u64 SourceAddress, u64 Size)
    {
        return SVC_UnmapProcessMemory(DestAddress, ProcessHandle, SourceAddress, Size);
    }

    u32 MapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size)
    {
        return SVC_MapProcessCodeMemory(ProcessHandle, DestAddress, SourceAddress, Size);
    }

    u32 UnmapProcessCodeMemory(u32 ProcessHandle, u64 DestAddress, u64 SourceAddress, u64 Size)
    {
        return SVC_UnmapProcessCodeMemory(ProcessHandle, DestAddress, SourceAddress, Size);
    }

    u32 CreateProcess(u32 *Out_Process, void *ProcessInfo, u32 *Caps, u64 CapCount)
    {
        return SVC_CreateProcess(Out_Process, ProcessInfo, Caps, CapCount);
    }

    u32 StartProcess(u32 ProcessHandle, s32 MainPriority, s32 DefaultCPU, u32 StackSize)
    {
        return SVC_StartProcess(ProcessHandle, MainPriority, DefaultCPU, StackSize);
    }

    u32 TerminateProcess(u32 ProcessHandle)
    {
        return SVC_TerminateProcess(ProcessHandle);
    }
    u32 GetProcessInfo(u64 *Out_Value, u32 ProcessHandle, bio::ProcessInfo InfoType)
    {
        return SVC_GetProcessInfo(Out_Value, ProcessHandle, InfoType);
    }

    u32 CreateResourceLimit(u32 *ResourceLimit)
    {
        return SVC_CreateResourceLimit(ResourceLimit);
    }

    u32 SetResourceLimitLimitValue(u32 ResourceLimit, bio::LimitableResource Resource, u64 Value)
    {
        return SVC_SetResourceLimitLimitValue(ResourceLimit, Resource, Value);
    }

    u64 CallSecureMonitor(bio::SecureMonitorArgs *Args)
    {
        return SVC_CallSecureMonitor(Args);
    }
}