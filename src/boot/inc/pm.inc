; ÃèÊö·û
;usage: Descriptor Base, Limit, Attr
;       Base:   dd
;       Limit:  dd (low 20 bits available)
;       Attr:   dw (lower 4 bits of higher byte are always 0)

%macro Descriptor 3
    dw %2 & 0FFFFh
    dw %1 & 0FFFFh
    db (%1 >> 16) & 0FFh
    dw ((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)
    db (%1 >> 24) & 0FFh
%endmacro   ; total 8 bytes

%macro Gate 4
    dw (%2 & 0ffffh)
    dw %1
    dw (%3 & 1Fh) | ((%4 << 8) & 0ff00h)
    dw ((%2 >> 16) & 0ffffh)
%endmacro   ; total 8 bytes

DA_C        equ 098h
DA_CR       equ 098h
DA_DRW      equ 092h
DA_LDT      equ 082h
DA_386CGate equ 08ch
DA_386IGate equ 08eh
DA_386TSS   equ 089h

DA_32       equ 04000h
DA_LIMIT_4K equ 08000h
DA_64       equ 02000h

DA_DPL0 equ 000h
DA_DPL1 equ 020h
DA_DPL2 equ 040h
DA_DPL3 equ 060h


SA_TIL  equ 4
SA_RPL3 equ 3

PG_P    equ 1
PG_RWW  equ 2
PG_USS  equ 0
PG_USU  equ 4

DirectMemBase equ 0xffff880000000000