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
	push r15
endm  


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


doTimerAsm PROC EXPORT
	pushM
	mov rax, [rsp+80h]
	;push QWORD PTR [rsp+88h]
	mov rcx, [rsp+88h]
	sub rsp,20h
	call rax
	add rsp,20h
	popM
	add rsp, 10h
	ret
doTimerAsm ENDP

end