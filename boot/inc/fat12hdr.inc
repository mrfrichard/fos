; ----------------------------------------------------------
; const define base on FAT12
; ----------------------------------------------------------
FATSz                   equ 9
RootDirSectors          equ 14
SectorNoOfRootDirectory equ 19
SectorNoOfFAT1          equ 1
DeltaSectorNo           equ 17

BytsPerSec  equ  512
SecPerTrk   equ  18
DrvNum      equ  0

LoadFileLoc equ 0xa8
LoadFileFun equ 0x7c00 + LoadFileLoc