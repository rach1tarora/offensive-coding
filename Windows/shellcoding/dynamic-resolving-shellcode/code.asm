BITS 32

global _start

section .text

_start:
	xor eax, eax
	mov eax, [fs:eax + 0x30]
	mov eax, [eax + 0xc]
	mov esi, [eax + 0x14]
	lodsd
	xchg eax, esi
	lodsd
	mov eax, [eax + 0x10]
	
	mov ebx, [eax + 0x3c]
	add ebx, eax
	mov ebx, [ebx + 0x78]
	add ebx, eax
	mov esi, [ebx + 0x20]
	add esi, eax
	xor ecx, ecx
	mov edx, eax
	push edx
	push esi
	
	push 0x00059ba3
	call _find_addr
	call _get_addr
	push edi
	
	mov eax, [esp + 12]
	mov esi, [esp + 8]
	
	xor ecx, ecx
	push 0x0015bdfd
	call _find_addr
	call _get_addr
	push edi
	jmp _do_main

_find_addr:
	inc ecx
	lodsd
	add eax, edx
	call _calculate_hash
	cmp edi, [esp + 4]
	jnz _find_addr
	ret
	
_get_addr:	
	mov esi,  [ebx + 0x24]
	add esi, edx
	mov cx, WORD [esi + ecx * 2]
	dec ecx
	mov esi,  [ebx + 0x1c]
	add esi, edx
	mov edi,  [esi + ecx * 4]
	add edi, edx
	ret
	
_calculate_hash:
	push eax
	push ebx
	push ecx
	push edx
	
	xor ecx, ecx
	mov ebx, ecx
	mov edi, ebx
	mov edx, ebx
	_loop:
		shl edi, 1
		mov dl, BYTE [eax + ecx]
		add edi, edx
		inc ecx
		cmp byte [eax + ecx], 0x0
		jne _loop
	
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret
	
_do_main:
	mov edi, [esp + 8]
	push "ll"
	push "32.d"
	push "user"
	push esp
	call edi
	
	push "oxA"
	push "ageB"
	push "Mess"
	push esp
	push eax
	mov edi, [esp + 32]
	call edi
	
	push "o!"
	push "hell"
	mov esi, esp

	xor ebx, ebx
	push ebx
	push esi
	push esi
	push ebx
	call eax
