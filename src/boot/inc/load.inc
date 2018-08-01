BaseOfLoader equ 09000h
OffsetOfLoader equ 0100h

BaseOfLoaderPhyAddr equ BaseOfLoader * 10h

BaseOfKernelFile equ 08000h
OffsetOfKernelFile equ 0

BaseOfKernelFilePhyAddr equ BaseOfKernelFile * 10h

KernelEntryPointPhyAddr equ 010000h

PML4E_Base      equ 00h
PDPTE_Base      equ 1000h
PDE_Base        equ 2000h
PTE_Base        equ 3000h

BaseMemSize     equ 1000000h
PDESize         equ 200000h
