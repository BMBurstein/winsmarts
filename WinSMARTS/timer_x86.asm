; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

doTimerAsm PROC EXPORT
	push ebp
	mov ebp, esp
	pushfd             ; backup flags register (4 Bytes)
	pushad             ; bakcup general purpose registers (8*4 Bytes)
	mov eax, [ebp+08h] ; load interrupt handler pointer
	push DWORD PTR [ebp+0Ch]     ; push handler parameter
	call eax           ; invoke interrupt handler
	popad              ; after return from interrupt handler, restore general purpose registers
	popfd              ; restore flags register
	mov esp, ebp  
	pop ebp  
	ret 8
doTimerAsm ENDP

end