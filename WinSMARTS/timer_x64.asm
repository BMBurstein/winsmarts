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
	pushM               ; backup flags register and general purpose registers (16*8 Bytes)
	mov rax, [rsp+80h]  ; load interrupt handler pointer
	mov rcx, [rsp+88h]  ; push interrupt handler parameter

	sub rsp, 10h
	push r8
	mov r8, 15
	and r8, rsp
	mov [rsp+8h], r8
	mov [rsp+10h], r8
	cmp r8, 0
	pop r8
	je lable1
	add rsp,8h
	lable1:

	sub rsp,20h         ; stack preparation before calling to function
	call rax            ; invoke interrupt handler
	add rsp,20h         ; stack restore after calling to function
	
	push r8
	mov r8, [rsp+8h]
	cmp r8, 0
	pop r8
	jne lable2
	add rsp, 8h
	lable2:
	add rsp, 8h

	popM                ; after return from interrupt handler, restore general purpose registers and flags register
	add rsp, 10h        ; pop interrupt handler pointer and the parameter
	ret                 ; resume
doTimerAsm ENDP

end