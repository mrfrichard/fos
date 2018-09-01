P_STACKBASE     equ 8
GSREG           equ P_STACKBASE
FSREG           equ GSREG           + 8
R15REG          equ FSREG           + 8
R14REG          equ R15REG          + 8
R13REG          equ R14REG          + 8
R12REG          equ R13REG          + 8
R11REG          equ R12REG          + 8
R10REG          equ R11REG          + 8
R9REG           equ R10REG          + 8
R8REG           equ R9REG           + 8
RDIREG          equ R8REG           + 8
RSIREG          equ RDIREG          + 8
RBPREG          equ RSIREG          + 8
RBXREG          equ RBPREG          + 8
RDXREG          equ RBXREG          + 8
RCXREG          equ RDXREG          + 8
RAXREG          equ RCXREG          + 8
RETADR          equ RAXREG          + 8
RIPREG          equ RETADR          + 8
CSREG           equ RIPREG          + 8
EFLAGSREG       equ CSREG           + 8
RSPREG          equ EFLAGSREG       + 8
SSREG           equ RSPREG          + 8
P_STACKTOP      equ SSREG           + 8
PML4EADDR       equ P_STACKTOP

SELECTOR_FLAT_C     equ 0x08
SELECTOR_FLAT_RW    equ 0x10
SELECTOR_VEDIO      equ 0x1b
SELECTOR_TSS        equ 0x30

SELECTOR_KERNEL_CS  equ SELECTOR_FLAT_C

TSS3_S_SP0          equ 4

INT_M_CTL       equ 0x20
INT_M_CTLMASK   equ 0x21
INT_S_CTL       equ 0xa0
INT_S_CTLMASK   equ 0xa1
EOI             equ 0x20

BaseOfKernelFilePhyAddr equ 0x80000