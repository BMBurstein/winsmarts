; Copyright (c) Microsoft Corporation.  All rights reserved.

.686
.model flat, c

.code

contextSwitch PROC EXPORT
	pushfd
	pushad
	mov eax, [esp+28h]
	mov ebx, [esp+2ch]
	mov [eax], esp
	mov esp, ebx
	popad
	popfd
	ret
contextSwitch ENDP

newTask PROC EXPORT
	mov ebx, esp
	mov esp, [esp+0ch]
	push [ebx+14h] ; retParam
	push eax       ; dummy value - DO NOT RETURN FROM RET
	push [ebx+08h] ; fnParam
	push [ebx+10h] ; ret
	push [ebx+04h] ; fn
	pushfd
	pushad
	mov eax, esp
	mov esp, ebx
	ret
newTask ENDP

popReg PROC EXPORT
	pop edi
	pop esi
	pop ebp
	pop edx
	pop ecx
	pop ebx
	pop eax
	popfd
	ret
popReg ENDP
end