; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

doTimerAsm PROC EXPORT
	pushfd             ; backup flags register (4 Bytes)
	pushad             ; bakcup general purpose registers (8*4 Bytes)
	mov eax, [esp+24h] ; load interrupt handler pointer
	push DWORD PTR [esp+28h]     ; push handler parameter
	call eax           ; invoke interrupt handler
	popad              ; after return from interrupt handler, restore general purpose registers
	popfd              ; restore flags register
	add esp, 08h       ; pop interrupt handler pointer and the parameter
	ret                ; resume
doTimerAsm ENDP

end