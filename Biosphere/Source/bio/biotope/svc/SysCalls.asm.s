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

BIO_ASM_SVC_Begin bioSysCallSetHeapSize
	str x0, [sp, #-16]!
	svc 0x1
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetMemoryPermission
	svc 0x2
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetMemoryAttribute
	svc 0x3
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapMemory
	svc 0x4
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapMemory
	svc 0x5
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallQueryMemory
	str x1, [sp, #-16]!
	svc 0x6
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallExitProcess
	svc 0x7
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateThread
	str x0, [sp, #-16]!
	svc 0x8
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallStartThread
	svc 0x9
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallExitThread
	svc 0xA
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSleepThread
	svc 0xB
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetThreadPriority
	str x0, [sp, #-16]!
	svc 0xC
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetThreadPriority
	svc 0xD
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetThreadCoreMask
	stp x0, x1, [sp, #-16]!
	svc 0xE
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetThreadCoreMask
	svc 0xF
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetCurrentProcessorNumber
	svc 0x10
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSignalEvent
	svc 0x11
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallClearEvent
	svc 0x12
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapSharedMemory
	svc 0x13
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapSharedMemory
	svc 0x14
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateTransferMemory
	str x0, [sp, #-16]!
	svc 0x15
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCloseHandle
	svc 0x16
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallResetSignal
	svc 0x17
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallWaitSynchronization
	str x0, [sp, #-16]!
	svc 0x18
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCancelSynchronization
	svc 0x19
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallArbitrateLock
	svc 0x1A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallArbitrateUnlock
	svc 0x1B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallWaitProcessWideKeyAtomic
	svc 0x1C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSignalProcessWideKey
	svc 0x1D
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetSystemTick
	svc 0x1E
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallConnectToNamedPort
	str x0, [sp, #-16]!
	svc 0x1F
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSendSyncRequest
	svc 0x21
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSendSyncRequestWithUserBuffer
	svc 0x22
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSendAsyncRequestWithUserBuffer
	str x0, [sp, #-16]!
	svc 0x23
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetProcessId
	str x0, [sp, #-16]!
	svc 0x24
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetThreadId
	str x0, [sp, #-16]!
	svc 0x25
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallBreak
	svc 0x26
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallOutputDebugString
	svc 0x27
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallReturnFromException
	svc 0x28
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetInfo
	str x0, [sp, #-16]!
	svc 0x29
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapPhysicalMemory
	svc 0x2A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapPhysicalMemory
	svc 0x2B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetResourceLimitLimitValue
	str x0, [sp, #-16]!
	svc 0x30
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetResourceLimitCurrentValue
	str x0, [sp, #-16]!
	svc 0x31
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetThreadActivity
	svc 0x32
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetThreadContext3
	svc 0x33
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateSession
	stp x0, x1, [sp, #-16]!
	svc 0x40
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallAcceptSession
	str x0, [sp, #-16]!
	svc 0x41
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallReplyAndReceive
	str x0, [sp, #-16]!
	svc 0x43
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallReplyAndReceiveWithUserBuffer
	str x0, [sp, #-16]!
	svc 0x44
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateEvent
	stp x0, x1, [sp, #-16]!
	svc 0x45
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapPhysicalMemoryUnsafe
	svc 0x48
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapPhysicalMemoryUnsafe
	svc 0x49
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetUnsafeLimit
	svc 0x4A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateCodeMemory
	str x0, [sp, #-16]!
	svc 0x4B
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallControlCodeMemory
	svc 0x4C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallReadWriteRegister
	str x0, [sp, #-16]!
	svc 0x4E
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateSharedMemory
	str x0, [sp, #-16]!
	svc 0x50
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapTransferMemory
	svc 0x51
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapTransferMemory
	svc 0x52
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateInterruptEvent
	str x0, [sp, #-16]!
	svc 0x53
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallQueryPhysicalAddress
	str x0, [sp, #-16]!
	svc 0x54
	ldr x4, [sp], #16
	stp x1, x2, [x4]
	str x3, [x4, #16]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallQueryIoMapping
	str x0, [sp, #-16]!
	svc 0x55
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateDeviceAddressSpace
	str x0, [sp, #-16]!
	svc 0x56
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallAttachDeviceAddressSpace
	svc 0x57
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallDetachDeviceAddressSpace
	svc 0x58
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapDeviceAddressSpaceByForce
	svc 0x59
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapDeviceAddressSpaceAligned
	svc 0x5A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapDeviceAddressSpace
	svc 0x5C
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallDebugActiveProcess
	str x0, [sp, #-16]!
	svc 0x60
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallBreakDebugProcess
	svc 0x61
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetDebugEvent
	svc 0x63
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallLegacyContinueDebugEvent
	svc 0x64
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallContinueDebugEvent
	svc 0x64
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetProcessList
	str x0, [sp, #-16]!
	svc 0x65
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetThreadList
	str x0, [sp, #-16]!
	svc 0x66
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetDebugThreadContext
	svc 0x67
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetDebugThreadContext
	svc 0x68
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallQueryDebugProcessMemory
	str x1, [sp, #-16]!
	svc 0x69
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallReadDebugProcessMemory
	svc 0x6A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallWriteDebugProcessMemory
	svc 0x6B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetDebugThreadParam
	stp x0, x1, [sp, #-16]!
	svc 0x6D
	ldp x3, x4, [sp], #16
	str x1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetSystemInfo
	str x0, [sp, #-16]!
	svc 0x6F
	ldr x2, [sp], #16
	str x1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreatePort
	stp x0, x1, [sp, #-16]!
	svc 0x70
	ldp x3, x4, [sp], #16
	str w1, [x3]
	str w2, [x4]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallManageNamedPort
	str x0, [sp, #-16]!
	svc 0x71
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallConnectToPort
	str x0, [sp, #-16]!
	svc 0x72
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetProcessMemoryPermission
	svc 0x73
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapProcessMemory
	svc 0x74
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapProcessMemory
	svc 0x75
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallMapProcessCodeMemory
	svc 0x77
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallUnmapProcessCodeMemory
	svc 0x78
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateProcess
	str x0, [sp, #-16]!
	svc 0x79
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallStartProcess
	svc 0x7A
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallTerminateProcess
	svc 0x7B
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallGetProcessInfo
	str x0, [sp, #-16]!
	svc 0x7C
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCreateResourceLimit
	str x0, [sp, #-16]!
	svc 0x7D
	ldr x2, [sp], #16
	str w1, [x2]
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallSetResourceLimitLimitValue
	svc 0x7E
	ret
BIO_ASM_SVC_End

BIO_ASM_SVC_Begin bioSysCallCallSecureMonitor
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