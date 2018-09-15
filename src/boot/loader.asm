[map symbols loader.map]
%include "load.inc"
org OffsetOfLoader ;0100h

BaseOfStack equ 0100h

jmp LABEL_START

_CursorPosition dw 0
_ExtendMemSize  dw 0
_DisplayPage    dw 0
_VideoParaments:
_VideoMode      db 0
_WindowsWidth   db 0
_VideoCardParaments:
_VideoMemSize   db 0
_DisplayState   db 0
_DisplayCardP   dw 0
_firstHd:    times 10h db 0
_secondHd:   times 10h db 0

%include "fat12hdr.inc"
%include "pm.inc"

LABEL_GDT:              Descriptor  0,          0,                  0               ; ��������
LABEL_DESC_FLAT_C:      Descriptor  0,          0fffffh,            DA_CR | DA_32 | DA_LIMIT_4K  ; 0-4G ��ִ�ж�
LABEL_DESC_FLAT_RW:     Descriptor  0,          0fffffh,            DA_DRW | DA_32 | DA_LIMIT_4K    ; 0-4G �ɶ�д��
LABEL_DESC_VIDEO:       Descriptor  0B8000h,    07fffh,             DA_DRW | DA_DPL3         ; �Դ��׵�ַ
GdtLen      equ $ - LABEL_GDT   ; GDT ����

LABEL_GDT64:            Descriptor  0,          0,                  0               ; ��������
LABEL_DESC64_FLAT_C:    Descriptor  0,          0fffffh,            DA_CR | DA_64 | DA_LIMIT_4K; 0-4G ��ִ�ж�
LABEL_DESC64_FLAT_RW:   Descriptor  0,          0fffffh,            DA_DRW | DA_64 | DA_LIMIT_4K; 0-4G �ɶ�д��
LABEL_DESC64_VIDEO:     Descriptor  0B8000h,    07fffh,             DA_DRW | DA_DPL3         ; �Դ��׵�ַ
LABEL_DESC64_USER_C:    Descriptor  0,          0fffffh,            DA_CR | DA_64 | DA_LIMIT_4K | DA_DPL3; 0-4G ��ִ�ж�
LABEL_DESC64_USER_RW:   Descriptor  0,          0fffffh,            DA_DRW | DA_64 | DA_LIMIT_4K | DA_DPL3; 0-4G �ɶ�д��
GdtLen64      equ $ - LABEL_GDT64   ;

GdtPtr      dw  GdtLen
            dd  BaseOfLoaderPhyAddr + LABEL_GDT
GdtPtr64    dw  GdtLen64
            dq  BaseOfLoaderPhyAddr + LABEL_GDT64

; GDT ѡ����
SelectorFlatC   equ LABEL_DESC_FLAT_C - LABEL_GDT
SelectorFlatRW  equ LABEL_DESC_FLAT_RW - LABEL_GDT
SelectorVideo   equ LABEL_DESC_VIDEO - LABEL_GDT + SA_RPL3
Selector64FlatC   equ LABEL_DESC64_FLAT_C - LABEL_GDT64
Selector64FlatRW  equ LABEL_DESC64_FLAT_RW - LABEL_GDT64
Selector64Video   equ LABEL_DESC64_VIDEO - LABEL_GDT64 + SA_RPL3

[SECTION .data1]
ALIGN   32
[BITS   32]
LABEL_DATA:

wRootDirSizeForLoop dw RootDirSectors
wSectorNo dw 0
bOdd db 0
dwKernelSize dd 0

KernelFileName db "KERNEL  BIN", 0
MessageLength equ 9
BootMessage:    db "Loading  "
Message1        db "Ready.   "
Message2        db "No Kernel"

_MemChkBuf: times 256 db 0
MemChkBuf equ BaseOfLoaderPhyAddr + _MemChkBuf

_dwMCRNumber: db 0
dwMCRNumber equ BaseOfLoaderPhyAddr + _dwMCRNumber

_szReturn: db 0x0a, 0
szReturn equ BaseOfLoaderPhyAddr + _szReturn

_dwDispPos: dd (80 * 6 + 0) * 2
dwDispPos equ BaseOfLoaderPhyAddr + _dwDispPos

_szMemChkTitle db "BaseAddrL BaseAddrH LengthLow LengthHigh Type", 0ah, 0
szMemChkTitle equ BaseOfLoaderPhyAddr + _szMemChkTitle

BPB_BytsPerSec  dw  0
BPB_SecPerTrk   dw  0
BS_DrvNum       db  0

DataLen equ $ - LABEL_DATA

[SECTION .gs]
ALIGN   32
[BITS   32]
LABEL_STACK:
StackSpace: times   1024 db 0;
TopOfStack equ BaseOfLoaderPhyAddr + $


[SECTION .s16]
[BITS   16]

LABEL_START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, BaseOfStack
    
    mov ah, 0x03
    xor bh, bh
    int 0x10
    mov [_CursorPosition], dx

    mov ah, 0x88
    int 0x15
    mov [_ExtendMemSize], ax

    mov ah, 0x0f
    int 0x10
    mov [_DisplayPage], bx
    mov [_VideoParaments], ax

    mov ah, 0x12
    mov bl, 0x10
    int 0x10
    mov [_VideoCardParaments], bx
    mov [_DisplayCardP], cx

    mov ax, 0
    mov ds, ax
    lds si, [4 * 0x46]
    mov di, _firstHd
    mov cx, 0x10
    rep
    movsb

    mov ah, 0x15
    mov dl, 0x81
    jc no_secondHd
    cmp ah, 0x03
    jne no_secondHd
    lds si, [4 * 0x46]
    mov di, _secondHd
    mov cx, 0x10
    rep
    movsb

no_secondHd:
    mov ax, cs
    mov ds, ax

    mov dh, 0
    call DispStr1
    ; soft driver reset
    xor ah, ah
    xor dl, dl
    int 13h

    ; Search Loader.bin in driver a
    pop es
    mov ax, 0
    mov es, ax
    mov ax, [es:0x7c0b]
    mov [BPB_BytsPerSec], ax
    mov ax, [es:0x7c18]
    mov [BPB_SecPerTrk], ax
    mov eax, [es:0x7c24]
    mov [BS_DrvNum], ax
    
    mov word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
    cmp word [wRootDirSizeForLoop], 0
    jz LABEL_NO_KERNELBIN
    dec word [wRootDirSizeForLoop]
    mov ax, BaseOfKernelFile
    mov es, ax
    mov bx, OffsetOfKernelFile

    mov ax, [wSectorNo]
    mov cl, 1
    call ReadSector

    mov si, KernelFileName
    mov di, OffsetOfKernelFile

    cld
    mov dx, 10h

LABEL_SEARCH_FOR_KERNELBIN:
    cmp dx, 0
    jz LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR
    dec dx
    mov cx, 11
LABEL_CMP_ILENAME:
    cmp cx, 0
    jz LABEL_FILENAME_FOUND
    dec cx
    lodsb
    cmp al, byte [es:di]
    jz LABEL_GO_ON
    jmp LABEL_DIFFERENT

LABEL_GO_ON:
    inc di
    jmp LABEL_CMP_ILENAME

LABEL_DIFFERENT:
    and di, 0ffe0h;
    add di, 20h;
    mov si, KernelFileName
    jmp LABEL_SEARCH_FOR_KERNELBIN

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
    add word [wSectorNo], 1
    jmp LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_KERNELBIN:
    mov dh, 2
    Call DispStr1
    jmp $

LABEL_FILENAME_FOUND:
    mov ax, RootDirSectors
    and di, 0fff0h

    push eax
    mov eax, [es:di + 01ch]
    mov dword [dwKernelSize], eax
    pop eax
    
    add di, 01ah;
    mov cx, word[es:di]
    push cx
    add cx, ax
    add cx, DeltaSectorNo
    mov ax, BaseOfKernelFile
    mov es, ax
    mov bx, OffsetOfKernelFile

    mov ax, cx
LABEL_GOON_LOADING_FILE:
    push ax
    push bx
    mov ah, 0eh

    mov al, '.'
    mov bl, 0fh
    int 10h
    pop bx
    pop ax
    mov cl, 1
    call ReadSector
    pop ax
    call GetFATEntry
    cmp ax, 0fffh
    jz LABEL_FILE_LOADED
    push ax
    mov dx, RootDirSectors;
    add ax, dx
    add ax, DeltaSectorNo
    add bx, [BPB_BytsPerSec]
    jmp LABEL_GOON_LOADING_FILE
LABEL_FILE_LOADED:
    mov dx, 03f2h
    mov al, 0
    out dx, al
    mov dh, 1
    call DispStr1

    ; �õ��ڴ���
    mov ebx, 0
    mov di, _MemChkBuf
.loop:
    mov eax, 0e820h
    mov ecx, 20
    mov edx, 0534d4150h
    int 15h
    jc  LABEL_MEM_CHK_FAIL
    add di, 20
    inc dword [_dwMCRNumber]
    cmp ebx, 0
    jne .loop
    jmp LABEL_MEM_CHK_OK
LABEL_MEM_CHK_FAIL:
    mov dword [_dwMCRNumber], 0
LABEL_MEM_CHK_OK:

    ; ����gdtr
    lgdt [GdtPtr]

    ; ���ж�
    cli

    ; �򿪵�ַ��A20
    in al, 92h
    or al, 00000010b
    out 92h, al

    ; ׼���л�������ģʽ
    mov eax, cr0
    or  eax, 1
    mov cr0, eax

    ; �������뱣��ģʽ
    jmp dword SelectorFlatC:(BaseOfLoaderPhyAddr + LABEL_PM_START)

ReadSector:
    push bx
    mov bp, sp
    sub esp, 2
    mov byte[bp - 2], cl
    push bx
    mov bl, [BPB_SecPerTrk]
    div bl
    inc ah
    mov cl, ah
    mov dh, al
    shr al, 1
    mov ch, al
    and dh, 1
    pop bx
    mov dl, [BS_DrvNum]
.GoOnReading:
    mov ah, 2
    mov al, byte [bp - 2]
    int 13h
    jc .GoOnReading
    add esp, 2
    pop bp

    ret

DispStr1:
    mov ax, MessageLength
    mul dh
    add ax, BootMessage
    mov bp, ax
    mov ax, ds
    mov es, ax
    mov cx, MessageLength
    mov ax, 01301h
    mov bx, 0007h
    add dh, 3
    mov dl, 0
    int 10h
    ret

GetFATEntry:
    push es
    push bx
    push ax

    mov ax, BaseOfKernelFile
    sub ax, 0100h
    mov es, ax

    pop ax
    mov byte [bOdd], 0
    mov bx, 3
    mul bx
    mov bx, 2
    div bx
    cmp dx, 0
    jz LABEL_EVEN
    mov byte [bOdd], 1

LABEL_EVEN:
    xor dx, dx
    mov bx, [BPB_BytsPerSec]
    div bx
    push dx
    mov bx, 0
    add ax, SectorNoOfFAT1;
    mov cl, 2
    call ReadSector
    pop dx
    add bx, dx
    mov ax, [es:bx]
    cmp byte [bOdd], 1
    jnz LABEL_EVEN_2
    shr ax, 4
LABEL_EVEN_2:
    and ax, 0fffh
LABEL_GET_FAT_ENRY_OK:
    pop bx
    pop es
    ret

[SECTION .s32]
ALIGN 32
[BITS 32]

LABEL_PM_START:
    mov ax, SelectorVideo
    mov gs, ax
    mov ax, SelectorFlatRW
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, TopOfStack

SetupPaging:
; setup PML4E
    mov ax, SelectorFlatRW
    mov es, ax
    mov edi, PML4E_Base
    mov eax, PDPTE_Base | PG_P | PG_USU | PG_RWW	
    stosd
    mov ecx, 271 * 2 + 1
    mov eax, 0
    rep stosd
    mov eax, PDPTE_Base | PG_P | PG_USU | PG_RWW
    stosd
    mov ecx, 239 * 2 + 1
    mov eax, 0
    rep stosd

; setup PDPTE
    mov     edi, PDPTE_Base
    mov     eax, PDE_Base | PG_P | PG_USU | PG_RWW
    stosd
    mov ecx, 1023
    mov eax, 0
    rep stosd

; setup PDE
    mov edi, PDE_Base
    mov eax, PTE_Base | PG_P | PG_USU | PG_RWW
    stosd
    mov ecx, 1023
    mov eax, 0
    rep stosd

; setup PTE
    mov edi, PTE_Base
    mov eax, PG_P | PG_USU | PG_RWW
    mov ecx, 0x200
.1:
    stosd
    push eax
    mov eax, 0
    stosd
    pop eax
    add eax, 4096
    loop .1
    
    mov     eax, PML4E_Base
    mov     cr3, eax

    mov eax, cr4
    or eax, 30h
    mov cr4, eax

    mov ecx, 0c0000080h
    rdmsr
    bts eax, 8
    wrmsr

    mov     eax, cr0
    or      eax, 80000000h
    mov     cr0, eax
    jmp .ret
.ret:
    jmp Selector64FlatC:dword(to_long_mod + BaseOfLoaderPhyAddr)

%include "lib.inc"

[BITS   64]
to_long_mod:
    lgdt [GdtPtr64 + BaseOfLoaderPhyAddr]

    mov rax, Selector64FlatRW
    mov ds, rax
    mov es, rax
    mov ss, rax
    mov fs, rax
    mov rax, Selector64Video
    mov gs, rax

    push qword Selector64FlatRW
    push rsp
    pushfq

    push qword Selector64FlatC
    push qword bit642 + BaseOfLoaderPhyAddr
    iretq

bit642:
;InitKernel
    xor rsi, rsi
    mov cx, word [BaseOfKernelFilePhyAddr + 38h]    ; Elf64_Half    (4 bytes)   e_phnum         程序头表中的项数。生成的 e_phentsize 和 e_phnum 指定了表的大小（以字节为单位）。如果文件没有程序头表，则 e_phnum 值为零。
    movzx rcx, cx;
    mov r8w, [BaseOfKernelFilePhyAddr + 36h]        ; Elf64_Half    (4 bytes)   e_phentsize     文件的程序头表中某一项的大小（以字节为单位）。所有项的大小都相同。
    movzx r8, r8w
    mov r10, [BaseOfKernelFilePhyAddr + 20h]        ; Elf64_Off     (16 bytes)  e_shoff         节头表的文件偏移（以字节为单位）。如果文件没有节头表，则此成员值为零。
    add r10, BaseOfKernelFilePhyAddr
.Begin:
    mov eax, [r10 + 0]                              ; Elf64_Word    p_type      此数组元素描述的段类型或解释此数组元素的信息的方式。表 7–25 中指定了类型值及其含义。
    cmp eax, 0                                      ; p_type = PT_NULL(0)       未使用。没有定义成员值。使用此类型，程序头表可以包含忽略的项。
    jz .NoAction
    mov r9, rcx
    mov rcx, [r10 + 020h]                           ; Elf64_Xword   p_filesz    段的文件映像中的字节数，可以为零。
    mov rdi, [r10 + 10h]                            ; Elf64_Addr    p_vaddr     段的第一个字节在内存中的虚拟地址
    mov rax, [r10 + 08h]                            ; Elf64_Off     p_offset    相对段的第一个字节所在文件的起始位置的偏移
    add rax, BaseOfKernelFilePhyAddr
    mov rsi, rax
    rep movsb
    mov rcx, r9
    
.NoAction:
    add r10, r8
    dec rcx
    jnz .Begin
    
    mov rax, [BaseOfKernelFilePhyAddr + 18h]
    jmp rax