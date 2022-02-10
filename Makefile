all:	
	nasm -fbin src/x86_64/boot/bootloader.asm -o bin/bootloader.bin
	nasm -felf64 src/x86_64/kernel/kernel.asm -o objres/kasm.o
	nasm -felf64 src/x86_64/kernel/cpu/impl/TSS.asm -o objres/TSSASM.o
	gcc -c -m64 src/x86_64/kernel/kmain.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o objres/kmain.o
	gcc -c -m64 src/x86_64/kernel/cpu/impl/TSS.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o objres/tss.o
	gcc -c -m64 src/x86_64/kernel/util/impl/strings.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/strings.o
	gcc -c -m64 src/x86_64/kernel/drivers/impl/VGA.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/vga.o
	gcc -c -m64 src/x86_64/kernel/drivers/impl/mouse.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/mouse.o
	gcc -c -m64 src/x86_64/kernel/drivers/impl/IO.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/io.o
	gcc -c -m64 src/x86_64/kernel/drivers/impl/PIT.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/pit.o
	gcc -c -m64 src/x86_64/kernel/interrupts/impl/exceptions.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/excp.o
	gcc -c -m64 src/x86_64/kernel/interrupts/impl/IDT.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/idt.o
	gcc -c -m64 src/x86_64/kernel/memory/impl/pmm.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/pmm.o
	gcc -c -m64 src/x86_64/kernel/memory/impl/heap.c -ffreestanding -fno-pie -fstack-protector -mgeneral-regs-only -o obj/heap.o
	nasm -felf64 src/x86_64/kernel/cpu/impl/switchutils.asm -o obj/lma.o
	ld -melf_x86_64 -Tlink.ld objres/*.o obj/*.o --oformat binary -o bin/kernel.bin
	cat bin/bootloader.bin bin/kernel.bin > bin/NekoOS.bin
	@ # Prepare the image.
	sudo dd if=/dev/zero of=NekoOS.img bs=1024 count=1440
	@ # Put the OS stuff in the image.
	sudo dd if=bin/NekoOS.bin of=NekoOS.img

burn_usb:
	sudo dd if=NekoOS.img of=/dev/sdb

danger:
	make all burn_usb run


run:
	sudo qemu-system-x86_64 -hda /dev/sdb -monitor stdio -d int -no-reboot -D logfile.txt -M smm=off
