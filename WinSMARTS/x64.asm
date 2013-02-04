.code

pushM macro
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
endm	push r15


popM macro
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
endm

contextSwitch PROC EXPORT
	pushM
	mov [rcx], rsp		; bacup stack pointer (32 bits) of outgoing task onto it's stack (before flags and general purpose registers)
	mov rsp, rdx		; load stack pointer (32 bits) of incoming task
	popM
	ret
contextSwitch ENDP

newTask PROC EXPORT
	mov rbx, rsp		; bacup stack pointer of 'newTask' caller (=constuctor of Task)
	mov rsp, r9	; load stack pointer of the new Task that is created (received parameter)
	push [rbx+28h]		; ?? retParam
	push rax			; ?? dummy value - DO NOT RETURN FROM RET
	push rdx		; ?? fnParam
	push r8		; ?? ret
	push rcx		; ?? fn
	pushM
	mov rax, rsp		; store stack pointer of the new Task that was created as return value
	mov rsp, rbx		; restore stack pointer of 'newTask' caller (=constuctor of Task)
	ret
newTask ENDP

doTimerAsm PROC EXPORT
	pushM
	mov rax, [rsp+80h]
	push [rsp+88h]
	call rax
	popM
	add rsp, 10h
	ret
doTimerAsm ENDP

end