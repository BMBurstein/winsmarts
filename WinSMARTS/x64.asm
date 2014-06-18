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

contextSwitch PROC EXPORT
	pushM
	mov [rcx], rsp    ; backup stack pointer (32 bits) of outgoing task onto its stack (before flags and general purpose registers)
	mov rsp, rdx    ; load stack pointer (32 bits) of incoming task
	popM
	mov rcx, [rsp+10h] 
	ret
contextSwitch ENDP

newTask PROC EXPORT
	push rbx
	mov rbx, rsp              ; backup stack pointer of 'newTask' caller (=constructor of Task)
	mov rsp, r8               ; load stack pointer of the new Task that is created (received parameter)
	push QWORD PTR [rbx+30h]  ; ?? retParam
	push rax                  ; ?? dummy value - DO NOT RETURN FROM RET
	push r9                   ; ?? ret
	sub rsp, 18h              ; ??shadow
	push rdx                  ; ?? fnParam

	lea rdx, taskEndAsm
	push rdx	

	push rcx                  ; ?? fn
	pushM
	mov rax, rsp    ; store stack pointer of the new Task that was created as return value
	mov rsp, rbx    ; restore stack pointer of 'newTask' caller (=constructor of Task)
	pop rbx
	ret
newTask ENDP

taskEndAsm PROC EXPORT
	add rsp, 20h
	mov rcx, [rsp+10h]
	ret
taskEndAsm ENDP

end