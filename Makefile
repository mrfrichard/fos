# Makefile for Tinix

# Entry point of Tinix
# It must be as same as 'KernalEntryPointPhyAddr' in load.inc
ENTRYPOINT		= 0xffff880000010000
BSS			= 0xffff880000020000
DATA			= 0xffff880000030000

#Offset of entry point in kernel file
#It depends on ENTRYPOINT
ENTRYOFFSET		= 0x400

# Programs, flags, ect.
ASM			= nasm
DASM		= ndisasm
CC			= gcc
LD			= ld
ASMBFLAGS	= -I src/boot/inc/
ASMKFLAGS	= -I src/include/ -f elf64
CFLAGS		= -I src/include -Wall -c -fno-builtin -fno-stack-protector -fstrength-reduce \
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
FOSBoot		= release/bin/boot.bin
FOSLoader	= release/bin/loader.bin
FOSKernel	= release/bin/kernel.bin

OBJS			= o/kernel.o o/global.o o/start.o o/i8259.o o/protect.o o/klib.o o/string.o o/klibc.o \
                o/main.o o/proc.o o/clock.o o/mm.o #kernel/syscall.o  kernel/keyboard.o kernel/tty.o kernel/console.o \
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
	rm -f $(OBJS) $(FOSBoot) $(FOSLoader) $(FOSKernel)

disasm :
	$(DASM) $(DASMFLAGS) $(FOSKernel) > $(DASMOUTPUT)

createimg:
	dd if=/dev/zero of=$(WORKIMG) bs=512 count=2880; \
	losetup $(LOOPDEV) $(WORKIMG); \
	mkfs.msdos $(LOOPDEV); \
	losetup -d $(LOOPDEV); \
	dd if=$(FOSBoot) of=$(WORKIMG) bs=512 count=1 conv=notrunc
	
buildimg:
	mount $(WORKIMG) /mnt/a -o loop; \
	cp -f $(FOSLoader) /mnt/a; \
	cp -f $(FOSKernel) /mnt/a; \
	umount /mnt/a; \
	cp $(WORKIMG) ./release/img

$(FOSBoot) : src/boot/boot.asm src/boot/inc/load.inc src/boot/inc/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(FOSLoader) : src/boot/loader.asm src/boot/inc/load.inc src/boot/inc/fat12hdr.inc src/boot/inc/pm.inc src/boot/inc/lib.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(FOSKernel) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(FOSKernel) $(OBJS)

o/kernel.o : src/kernel/kernel.asm src/include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

o/start.o : src/kernel/start.c src/include/type.h src/include/protect.h src/include/proto.h src/include/string.h src/include/mm.h src/include/proto.h src/include/string.h
	$(CC) $(CFLAGS) -o $@ $<

o/i8259.o : src/kernel/i8259.c src/include/i8259.h src/include/type.h src/include/protect.h src/include/proto.h src/include/global.h src/include/const.h src/include/string.h
	$(CC) $(CFLAGS) -o $@ $<

o/protect.o : src/kernel/protect.c src/include/protect.h src/include/type.h src/include/global.h src/include/const.h src/include/proto.h src/include/mm.h
	$(CC) $(CFLAGS) -o $@ $<

o/global.o : src/kernel/global.c src/include/global.h src/include/type.h src/include/protect.h src/include/const.h src/include/main.h src/include/proc.h
	$(CC) $(CFLAGS) -o $@ $<

o/main.o : src/kernel/main.c src/include/main.h src/include/type.h src/include/const.h src/include/proto.h src/include/clock.h src/include/proc.h src/include/global.h \
				src/include/string.h src/include/mm.h
	$(CC) $(CFLAGS) -o $@ $<

o/proc.o : src/kernel/proc.c src/include/proc.h src/include/type.h src/include/const.h src/include/protect.h src/include/global.h src/include/mm.h src/include/link.h \
				src/include/proto.h src/include/string.h
	$(CC) $(CFLAGS) -o $@ $<

o/clock.o : src/kernel/clock.c src/include/clock.h src/include/type.h src/include/const.h src/include/protect.h src/include/global.h src/include/proto.h  src/include/i8259.h src/include/proc.h 
	$(CC) $(CFLAGS) -o $@ $<

o/mm.o : src/kernel/mm.c src/include/mm.h src/include/type.h src/include/const.h src/include/proto.h src/include/proc.h src/include/string.h
	$(CC) $(CFLAGS) -o $@ $<

o/klib.o : src/lib/klib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $< 

o/string.o : src/lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

o/klibc.o : src/lib/klib.c src/include/type.h src/include/proto.h src/include/const.h src/include/string.h
	$(CC) $(CFLAGS) -o $@ $<
