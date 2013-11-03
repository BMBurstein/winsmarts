; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

contextSwitch PROC EXPORT
	pushfd        ; bacup flags register (4 Bytes) of outgoing task
	pushad        ; bacup general purpose registers (8*4 Bytes) of outgoing task
	mov eax, [esp+28h] ; oldContext (received parameter)
	mov ebx, [esp+2ch] ; newContext (received parameter)
	mov [eax], esp    ; bacup stack pointer (4 Bytes) of outgoing task onto it's stack (before flags and general purpose registers)
	mov esp, ebx    ; load stack pointer (4 Bytes) of incoming task
	popad              ; restore general purpose registers of incoming task
	popfd              ; restore flags register of incoming task
	ret
contextSwitch ENDP

newTask PROC EXPORT
	mov ecx, esp       ; backup stack pointer of 'newTask' caller (=constuctor of Task)
	mov esp, [esp+0ch] ; load stack pointer of the new Task that is created (received parameter)
	push DWORD PTR [ecx+14h]     ; 'retParam' - parameter (pointer to SMART instance) for taskEnd function of anonymous namespace
	push eax           ; DUMMY VALUE (suppose to be instruction pointer) - NEVER return from taskEnd function
	push DWORD PTR [ecx+08h]     ; 'fnParam' - parameter (pointer to SMART instance) that passed to main function of the task
	push DWORD PTR [ecx+10h]     ; 'ret' - reutrn address from main function of the task (taskEnd of anonymous namespace)
	push DWORD PTR [ecx+04h]     ; 'fn' - first instruction of the task (main function)
	pushfd             ; flags register - meanwhile garbage for further backup operations
	pushad             ; general purpose registers - meanwhile garbage for further backup operations
	mov eax, esp       ; store stack pointer of the new Task that was created as return value
	mov esp, ecx       ; restore stack pointer of 'newTask' caller (=constuctor of Task)
	ret
newTask ENDP
end