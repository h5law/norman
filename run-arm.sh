#!/bin/sh -

qemu="qemu-system-arm"

elf=build/demo.elf

case $1 in
    *.elf)
	elf="$1"
	shift;
	;;
esac

# Set the target machine
machine=mps2-an385,accel=tcg
cpu=cortex-m3

$qemu -semihosting-config enable=on -monitor none -serial none -nographic -machine $machine -kernel "$elf" "$@"
