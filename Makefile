default:
	mkdir -p obj
	mkdir -p objres
	mkdir -p bin
	nasm -fbin src/x86_64/boot/stage1.asm -o bin/bootloader1.bin
	nasm -felf32 src/x86_64/boot/stage2.asm -o objres/bootloader2.o
	gcc -c -m64 src/x86_64/kernel/kmain.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/kmain.o
	ld -melf_x86_64 obj/*.o --oformat binary -o bin/kernel.bin
	ld -melf_i386 -e _stage2 -Ttext 0x1000 objres/*.o --oformat binary -o bin/stage2.bin
	cat bin/bootloader1.bin bin/stage2.bin bin/kernel.bin > bin/NekoOS.bin
	sudo dd if=/dev/zero of=NekoOS.img bs=1024 count=1440
	@ # Put the OS stuff in the image.
	sudo dd if=bin/NekoOS.bin of=NekoOS.img

all:
	make; make burnusb; make runusb

burnusb:
	sudo dd if=NekoOS.img of=/dev/sdb

runusb:
	sudo qemu-system-x86_64 -hda /dev/sdb -monitor stdio -d int -no-reboot -D logfile.txt -M smm=off # -soundhw pcspk

run:
	sudo qemu-system-x86_64 -fda NekoOS.img -monitor stdio
