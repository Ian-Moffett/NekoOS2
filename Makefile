all:	
	nasm -fbin src/x86_64/boot/bootloader.asm -o bin/bootloader.bin
	nasm -felf64 src/x86_64/kernel/kernel.asm -o objres/kasm.o
	gcc -c -m64 src/x86_64/kernel/kmain.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/kmain.o
	ld -melf_x86_64 -Tlink.ld objres/*.o obj/*.o --oformat binary -o bin/kernel.bin
	cat bin/bootloader.bin bin/kernel.bin > bin/NekoOS.bin
	@ # Prepare the image.
	sudo dd if=/dev/zero of=NekoOS.img bs=1024 count=1440
	@ # Put the OS stuff in the image.
	sudo dd if=bin/NekoOS.bin of=NekoOS.img

burn_usb:
	sudo dd if=NekoOS.img of=/dev/sdb

danger:
	make; make burn_usb run


run:
	sudo qemu-system-x86_64 -hda /dev/sdb -monitor stdio -d int -no-reboot -D logfile.txt -M smm=off
