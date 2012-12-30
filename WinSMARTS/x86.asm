; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

contextSwitch PROC EXPORT
	pushfd				;??bacup flags register (32 bits) of outgoing task
	pushad				;??bacup general purpose registers (8*8 bits)	of outgoing task
	mov eax, [esp+28h]	;?? bacup instruction pointer
	mov ebx, [esp+2ch]	;?? bacup stack pointer 
	mov [eax], esp		;??
	mov esp, ebx		;??
	popad				;??restore general purpose registers of incoming task
	popfd				;??restore EFLAGS register of incoming task
	ret
contextSwitch ENDP

newTask PROC EXPORT
	mov ebx, esp		;?? bacup stack pointer of newTask caller (constuctor of Task)
	mov esp, [esp+0ch]	;?? load stack pointer of new Task (parameter received)
	push [ebx+14h]		; retParam
	push eax			; dummy value - DO NOT RETURN FROM RET
	push [ebx+08h]		; fnParam
	push [ebx+10h]		; ret
	push [ebx+04h]		; fn
	pushfd
	pushad
	mov eax, esp
	mov esp, ebx
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