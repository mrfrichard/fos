; 
;----------------------------------------------------------------------------
DispAL:
    push    ecx
    push    edx
    push    edi
    mov edi, [dwDispPos]
    mov     ah, 0ch
    mov     dl, al
    shr     al, 4
    mov     ecx, 2
.begin:
    and     al, 01111b
    cmp     al, 9
    ja      .11
    add     al, '0'
    jmp     .12
.11:
    sub     al, 0ah
    add     al, 'A'
.12:
    mov     [gs:edi], ax
    add     edi, 2
    mov     al, dl
    loop    .begin

    add     edi, 2
    mov     [dwDispPos], edi
    pop     edi
    pop     edx
    pop     ecx

    ret

; 
;----------------------------------------------------------------------------
DispInt:
    mov eax, [esp + 4]
    shr eax, 24
    call DispAL

    mov eax, [esp + 4]
    shr eax, 16
    call DispAL

    mov eax, [esp + 4]
    shr eax, 8
    call DispAL

    mov eax, [esp + 4]
    call DispAL

    mov ah, 07h
    mov al, 'h'
    push edi
    mov edi, [dwDispPos]
    mov [gs:edi], ax
    add edi, 4
    mov [dwDispPos], edi
    pop edi

    ret

; 
;----------------------------------------------------------------------------
DispInt64:
    mov eax, [esp + 4]
    shr eax, 24
    call DispAL

    mov eax, [esp + 4]
    shr eax, 16
    call DispAL

    mov eax, [esp + 4]
    shr eax, 8
    call DispAL

    mov eax, [esp + 4]
    call DispAL

    mov eax, [esp + 8]
    shr eax, 24
    call DispAL

    mov eax, [esp + 8]
    shr eax, 16
    call DispAL

    mov eax, [esp + 8]
    shr eax, 8
    call DispAL

    mov eax, [esp + 8]
    call DispAL

    mov ah, 07h
    mov al, 'h'
    push edi
    mov edi, [dwDispPos]
    mov [gs:edi], ax
    add edi, 4
    mov [dwDispPos], edi
    pop edi
    ret

; 
;----------------------------------------------------------------------------
DispStr:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi
    mov esi, [ebp + 8]
    mov edi, [dwDispPos]
    mov ah, 0fh

.21:
    lodsb
    test al, al
    jz  .22
    cmp al, 0Ah;
    jnz .23
    push eax
    mov eax, edi
    mov bl, 160
    div bl
    and eax, 0FFh
    inc eax
    mov bl, 160
    mul bl
    mov edi, eax
    pop eax
    jmp .21
.23:
    mov [gs:edi], ax
    add edi, 2
    jmp .21

.22:
    mov [dwDispPos], edi
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
; 
;----------------------------------------------------------------------------
DispReturn:
    push szReturn
    call DispStr
    add esp, 4
    ret
