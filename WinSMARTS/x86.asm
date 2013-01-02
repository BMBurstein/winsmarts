; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

contextSwitch PROC EXPORT
	pushfd				; bacup flags register (32 bits) of outgoing task
	pushad				; bacup general purpose registers (8*8 bits) of outgoing task
	mov eax, [esp+28h]	; oldContext (received parameter)
	mov ebx, [esp+2ch]	; newContext (received parameter)
	mov [eax], esp		; bacup stack pointer (32 bits) of outgoing task onto it's stack (before flags and general purpose registers)
	mov esp, ebx		; load stack pointer (32 bits) of incoming task
	popad				; restore general purpose registers of incoming task
	popfd				; restore flags register of incoming task
	ret
contextSwitch ENDP

newTask PROC EXPORT
	mov ebx, esp		; bacup stack pointer of 'newTask' caller (=constuctor of Task)
	mov esp, [esp+0ch]	; load stack pointer of the new Task that is created (received parameter)
	push [ebx+14h]		; 'retParam' - parameter (pointer to SMART instance) for taskEnd function of anonymous namespace
	push eax			; DUMMY VALUE (suppose to be instruction pointer) - never return from taskEnd function
	push [ebx+08h]		; 'fnParam' - parameter (pointer to SMART instance) that passed to main function of the task
	push [ebx+10h]		; 'ret' - reutrn address from main function of the task (taskEnd of anonymous namespace)
	push [ebx+04h]		; 'fn' - first instruction of the task (main function)
	pushfd				; flags register - meanwhile garbage for further backup operations
	pushad				; general purpose registers - meanwhile garbage for further backup operations
	mov eax, esp		; store stack pointer of the new Task that was created as return value
	mov esp, ebx		; restore stack pointer of 'newTask' caller (=constuctor of Task)
	ret
newTask ENDP

doTimerAsm PROC EXPORT
	pushfd				; bacup flags register (32 bits)
	pushad              ; bacup general purpose registers (8*8 bits)
	mov eax, [esp+24h]  ; load interrupt handler pointer
	push [esp+28h]      ; push pointer to SMART instance again into stack head- required parameter for interrupt handler
	call eax			; invoke interrupt handler (with SMART instance at parameter)
	popad               ; after return from interrupt handler, restore general purpose registers
	popfd               ; restore flags register
	add esp, 08h        ; pop interrupt handler pointer and the parameter
	ret                 ; resume with instruction of task which we stoped
doTimerAsm ENDP
end