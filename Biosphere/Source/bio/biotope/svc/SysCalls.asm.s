.macro BIO_ASM_SVC_Begin name
	.section .text.\name, "ax", %progbits
	.global \name
	.type \name, %function
	.align 2
	.cfi_startproc
\name:
.endm

.macro BIO_ASM_SVC_End
	.cfi_endproc
.endm

BIO_ASM_SVC_Begin SVC_SetHeapSize
	str x0, [sp, #-16]!
	svc 0x1
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetMemoryPermission
	svc 0x2
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetMemoryAttribute
	svc 0x3
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapMemory
	svc 0x4
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapMemory
	svc 0x5
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_QueryMemory
	str x1, [sp, #-16]!
	svc 0x6
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ExitProcess
	svc 0x7
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateThread
	str x0, [sp, #-16]!
	svc 0x8
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_StartThread
	svc 0x9
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ExitThread
	svc 0xA
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SleepThread
	svc 0xB
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetThreadPriority
	str x0, [sp, #-16]!
	svc 0xC
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetThreadPriority
	svc 0xD
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetThreadCoreMask
	stp x0, x1, [sp, #-16]!
	svc 0xE
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetThreadCoreMask
	svc 0xF
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetCurrentProcessorNumber
	svc 0x10
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SignalEvent
	svc 0x11
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ClearEvent
	svc 0x12
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapSharedMemory
	svc 0x13
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapSharedMemory
	svc 0x14
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateTransferMemory
	str x0, [sp, #-16]!
	svc 0x15
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CloseHandle
	svc 0x16
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ResetSignal
	svc 0x17
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_WaitSynchronization
	str x0, [sp, #-16]!
	svc 0x18
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CancelSynchronization
	svc 0x19
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ArbitrateLock
	svc 0x1A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ArbitrateUnlock
	svc 0x1B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_WaitProcessWideKeyAtomic
	svc 0x1C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SignalProcessWideKey
	svc 0x1D
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetSystemTick
	svc 0x1E
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ConnectToNamedPort
	str x0, [sp, #-16]!
	svc 0x1F
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SendSyncRequest
	svc 0x21
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SendSyncRequestWithUserBuffer
	svc 0x22
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SendAsyncRequestWithUserBuffer
	str x0, [sp, #-16]!
	svc 0x23
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetProcessId
	str x0, [sp, #-16]!
	svc 0x24
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetThreadId
	str x0, [sp, #-16]!
	svc 0x25
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_Break
	svc 0x26
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_OutputDebugString
	svc 0x27
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ReturnFromException
	svc 0x28
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetInfo
	str x0, [sp, #-16]!
	svc 0x29
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapPhysicalMemory
	svc 0x2A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapPhysicalMemory
	svc 0x2B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetResourceLimitLimitValue
	str x0, [sp, #-16]!
	svc 0x30
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetResourceLimitCurrentValue
	str x0, [sp, #-16]!
	svc 0x31
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetThreadActivity
	svc 0x32
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetThreadContext3
	svc 0x33
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateSession
	stp x0, x1, [sp, #-16]!
	svc 0x40
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_AcceptSession
	str x0, [sp, #-16]!
	svc 0x41
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ReplyAndReceive
	str x0, [sp, #-16]!
	svc 0x43
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ReplyAndReceiveWithUserBuffer
	str x0, [sp, #-16]!
	svc 0x44
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateEvent
	stp x0, x1, [sp, #-16]!
	svc 0x45
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapPhysicalMemoryUnsafe
	svc 0x48
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapPhysicalMemoryUnsafe
	svc 0x49
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetUnsafeLimit
	svc 0x4A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateCodeMemory
	str x0, [sp, #-16]!
	svc 0x4B
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ControlCodeMemory
	svc 0x4C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ReadWriteRegister
	str x0, [sp, #-16]!
	svc 0x4E
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateSharedMemory
	str x0, [sp, #-16]!
	svc 0x50
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapTransferMemory
	svc 0x51
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapTransferMemory
	svc 0x52
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateInterruptEvent
	str x0, [sp, #-16]!
	svc 0x53
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_QueryPhysicalAddress
	str x0, [sp, #-16]!
	svc 0x54
	ldr x4, [sp], #16
	stp x1, x2, [x4]
	str x3, [x4, #16]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_QueryIoMapping
	str x0, [sp, #-16]!
	svc 0x55
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateDeviceAddressSpace
	str x0, [sp, #-16]!
	svc 0x56
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_AttachDeviceAddressSpace
	svc 0x57
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_DetachDeviceAddressSpace
	svc 0x58
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapDeviceAddressSpaceByForce
	svc 0x59
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapDeviceAddressSpaceAligned
	svc 0x5A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapDeviceAddressSpace
	svc 0x5C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_DebugActiveProcess
	str x0, [sp, #-16]!
	svc 0x60
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_BreakDebugProcess
	svc 0x61
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetDebugEvent
	svc 0x63
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_LegacyContinueDebugEvent
	svc 0x64
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ContinueDebugEvent
	svc 0x64
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetProcessList
	str x0, [sp, #-16]!
	svc 0x65
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetThreadList
	str x0, [sp, #-16]!
	svc 0x66
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetDebugThreadContext
	svc 0x67
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetDebugThreadContext
	svc 0x68
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_QueryDebugProcessMemory
	str x1, [sp, #-16]!
	svc 0x69
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ReadDebugProcessMemory
	svc 0x6A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_WriteDebugProcessMemory
	svc 0x6B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetDebugThreadParam
	stp x0, x1, [sp, #-16]!
	svc 0x6D
	ldp x3, x4, [sp], #16
	str x1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetSystemInfo
	str x0, [sp, #-16]!
	svc 0x6F
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreatePort
	stp x0, x1, [sp, #-16]!
	svc 0x70
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ManageNamedPort
	str x0, [sp, #-16]!
	svc 0x71
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_ConnectToPort
	str x0, [sp, #-16]!
	svc 0x72
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetProcessMemoryPermission
	svc 0x73
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapProcessMemory
	svc 0x74
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapProcessMemory
	svc 0x75
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_MapProcessCodeMemory
	svc 0x77
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_UnmapProcessCodeMemory
	svc 0x78
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateProcess
	str x0, [sp, #-16]!
	svc 0x79
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_StartProcess
	svc 0x7A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_TerminateProcess
	svc 0x7B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_GetProcessInfo
	str x0, [sp, #-16]!
	svc 0x7C
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CreateResourceLimit
	str x0, [sp, #-16]!
	svc 0x7D
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_SetResourceLimitLimitValue
	svc 0x7E
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin SVC_CallSecureMonitor
	str x0, [sp, #-16]!
	mov x8, x0
	ldp x0, x1, [x8]
	ldp x2, x3, [x8, #0x10]
	ldp x4, x5, [x8, #0x20]
	ldp x6, x7, [x8, #0x30]
	svc 0x7F
	ldr x8, [sp], #16
	stp x0, x1, [x8]
	stp x2, x3, [x8, #0x10]
	stp x4, x5, [x8, #0x20]
	stp x6, x7, [x8, #0x30]
	ret
BIO_ASM_SVC_End