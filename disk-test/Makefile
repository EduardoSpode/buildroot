obj-m := test.o
BUILDROOT_DIR := /workspaces/buildroot
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc

all:
	$(COMPILER) -o test test.c
	cp test $(BUILDROOT_DIR)/output/target/  bin