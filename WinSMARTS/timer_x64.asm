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
	push QWORD PTR [rsp+88h]
	call rax
	popM
	add rsp, 10h
	ret
doTimerAsm ENDP

;doTimerAsm PROC EXPORT
;	pushfd             ; backup flags register (4 Bytes)
;	pushad             ; bakcup general purpose registers (8*4 Bytes)
;	mov eax, [esp+24h] ; load interrupt handler pointer
;	push DWORD PTR [esp+28h]     ; push handler parameter
;	call eax           ; invoke interrupt handler
;	popad              ; after return from interrupt handler, restore general purpose registers
;	popfd              ; restore flags register
;	add esp, 08h       ; pop interrupt handler pointer and the parameter
;	ret                ; resume
;doTimerAsm ENDP

end