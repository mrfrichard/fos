# Makefile for Tinix

# Entry point of Tinix
# It must be as same as 'KernalEntryPointPhyAddr' in load.inc
ENTRYPOINT		= 0xffff880000010000
BSS				= 0xffff880000020000
DATA			= 0xffff880000050000

#Offset of entry point in kernel file
#It depends on ENTRYPOINT
ENTRYOFFSET		= 0x400

# Programs, flags, ect.
ASM			= nasm
DASM		= ndisasm
CC			= gcc
LD			= ld
ASMBFLAGS	= -I boot/inc/
ASMKFLAGS	= -I include/ -f elf64
CFLAGS		= -I include -Wall -c -fno-builtin -fno-stack-protector -fstrength-reduce \
              -fomit-frame-pointer -finline-functions -nostdinc -static -O -mcmodel=large # -DNDEBUG
#LDFLAGS		= -n -m elf_x86_64 -s -x -Ttext $(ENTRYPOINT) -Tbss $(BSS) -Tdata $(DATA) -Map=kernel.map -dn
LDFLAGS		= -n -m elf_x86_64 -s -x -Tld.lds -Map=kernel.map
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

# Img
IMG 		= fos.img
WORKIMG		= /root/$(IMG)
#WORKIMG		= ./$(IMG)
LOOPDEV		= $(shell losetup -f)
MOUNTDIR	= /mnt/a

# This Program 
FOSBoot		= boot.bin
FOSLoader	= loader.bin
FOSKernel	= kernel.bin

OBJS			= kernel/kernel.o kernel/global.o kernel/start.o kernel/i8259.o kernel/protect.o lib/klib.o lib/string.o lib/klibc.o \
                kernel/main.o kernel/proc.o kernel/clock.o kernel/mm.o #kernel/syscall.o  kernel/keyboard.o kernel/tty.o kernel/console.o \
DASMOUTPUT		= kernel.bin.asm

# All Phony Targets
.PHONY : everything final image img clean realclean disasm all buildimg

# Default starting position
everything : $(FOSBoot) $(FOSLoader) $(FOSKernel)
loader:  $(FOSLoader) $(FOSKernel)

all : clean everything

final : all clean

image : final createimg buildimg

img : loader buildimg

bimg : boot.bin createimg

clean :
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(TINIXBoot) $(TINIXKernel)

disasm :
	$(DASM) $(DASMFLAGS) $(TINIXKernel) > $(DASMOUTPUT)

createimg:
	dd if=/dev/zero of=$(WORKIMG) bs=512 count=2880; \
	losetup $(LOOPDEV) $(WORKIMG); \
	mkfs.msdos $(LOOPDEV); \
	losetup -d $(LOOPDEV); \
	dd if=boot.bin of=$(WORKIMG) bs=512 count=1 conv=notrunc
	
buildimg:
	mount $(WORKIMG) /mnt/a -o loop; \
	cp -f loader.bin /mnt/a; \
	cp -f kernel.bin /mnt/a; \
	umount /mnt/a; \
	cp $(WORKIMG) .

$(FOSBoot) : boot/boot.asm boot/inc/load.inc boot/inc/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(FOSLoader) : boot/loader.asm boot/inc/load.inc boot/inc/fat12hdr.inc boot/inc/pm.inc boot/inc/lib.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(FOSKernel) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(FOSKernel) $(OBJS)

kernel/kernel.o : kernel/kernel.asm # include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o : kernel/start.c include/type.h include/protect.h include/proto.h include/string.h 
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o : kernel/i8259.c include/i8259.h include/type.h include/protect.h include/proto.h include/global.h include/const.h include/string.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o : kernel/protect.c include/protect.h include/type.h include/global.h include/const.h include/proto.h include/mm.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o : kernel/global.c include/global.h include/type.h include/protect.h include/const.h include/main.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o : kernel/main.c include/main.h include/type.h include/const.h include/proto.h include/clock.h include/proc.h include/global.h include/string.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o : kernel/proc.c include/proc.h include/type.h include/const.h include/protect.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o : kernel/clock.c include/clock.h include/type.h include/const.h include/protect.h include/global.h include/proto.h  include/i8259.h include/proc.h 
	$(CC) $(CFLAGS) -o $@ $<

kernel/mm.o : kernel/mm.c include/mm.h include/type.h include/const.h include/proto.h
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o : lib/klib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $< 

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/klibc.o : lib/klib.c include/type.h include/proto.h include/const.h include/string.h
	$(CC) $(CFLAGS) -o $@ $<