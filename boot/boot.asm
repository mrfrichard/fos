%include "fat12hdr.inc"
%include "load.inc"
BaseOfStack equ  07c00h

org 07c00h

jmp short LABEL_START
nop

; =========================================================
; FAT12 disk a header
; ---------------------------------------------------------
BS_OEMName      db  'FutuStar'
BPB_BytsPerSec  dw  BytsPerSec
BPB_SecPerClus  db  1
BPB_RsvdSecCnt  dw  1
BPB_NUmFATs     db  2
BPB_RootEntCnt  dw  224
BPB_TotSec16    dw  2880
BPB_Media       db  0f0h
BPB_FATSz       dw  9
BPB_SecPerTrk   dw  SecPerTrk
BPB_NumHeads    dw  2
BPB_HiddSec     dd  0
BPB_TotSec32    dd  0
BS_DrvNum       db  DrvNum
BS_Reserved1    db  0
BS_BootSig      db  029h
BS_VolID        dd  0
BS_VolLab       db  'Future Star'
BS_FileSysType  db  'FAT12   '


wRootDirSizeForLoop dw RootDirSectors
wSectorNo dw 0
bOdd db 0

LoaderFileName db "LOADER  BIN", 0
MessageLength equ 9
BootMessage:    db "Booting  "
Message1        db "BootReady"
Message2        db "No LOADER"

LABEL_START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, BaseOfStack

    ; clear screen
    mov ax, 0600h   ; ah = 6; al=0
    mov bx, 0700h   ; �ڵװ��֣�bh=07h��
    mov cx, 0       ; ���Ͻǣ�0, 0��
    mov dx, 0184fh   ; ���½ǣ�80, 50��
    int 10h

    mov dh, 0
    call DispStr

    ; soft driver reset
    xor ah, ah
    xor dl, dl
    int 13h

    mov ax, BaseOfLoader
    mov bx, OffsetOfLoader
    mov si, LoaderFileName

    ; Search Loader.bin in driver a
    mov word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
    cmp word [wRootDirSizeForLoop], 0
    jz LABEL_NO_LOADERBIN
    dec word [wRootDirSizeForLoop]
    mov es, ax
    

    mov ax, [wSectorNo]
    mov cl, 1
    call ReadSector
    mov di, bx
    cld
    mov dx, 10h

LABEL_SEARCH_FOR_LOADERBIN:
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
    mov si, LoaderFileName
    jmp LABEL_SEARCH_FOR_LOADERBIN

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
    add word [wSectorNo], 1
    jmp LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
    mov dh, 2
    Call DispStr
    jmp $

LABEL_FILENAME_FOUND:
    mov ax, RootDirSectors
    and di, 0ffe0h
    add di, 01ah;
    mov cx, word[es:di]
    push cx
    add cx, ax
    add cx, DeltaSectorNo
    mov ax, BaseOfLoader
    mov es, ax
    mov bx, OffsetOfLoader

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
    mov dh, 1
    call DispStr
    jmp BaseOfLoader:OffsetOfLoader

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

DispStr:
    mov ax, MessageLength
    mul dh
    add ax, BootMessage
    mov bp, ax
    mov ax, ds
    mov es, ax
    mov cx, MessageLength
    mov ax, 01301h
    mov bx, 0007h
    mov dl, 0
    int 10h
    ret

GetFATEntry:
    push es
    push bx
    push ax

    mov ax, BaseOfLoader
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

    times 510-($-$$) db 0
    dw 0xaa55