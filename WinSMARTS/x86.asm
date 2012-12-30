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
	push [ebx+14h]		; ?? retParam
	push eax			; ?? dummy value - DO NOT RETURN FROM RET
	push [ebx+08h]		; ?? fnParam
	push [ebx+10h]		; ?? ret
	push [ebx+04h]		; ?? fn
	pushfd				; flags register - meanwhile garbage for further backup operations
	pushad				; general purpose registers - meanwhile garbage for further backup operations
	mov eax, esp		; store stack pointer of the new Task that was created as return value
	mov esp, ebx		; restore stack pointer of 'newTask' caller (=constuctor of Task)
	ret
newTask ENDP

doTimerAsm PROC EXPORT
	pushfd
	pushad
	mov eax, [esp+24h]
	push [esp+28h]
	call eax
	popad
	popfd
	add esp, 08h
	ret
doTimerAsm ENDP
end