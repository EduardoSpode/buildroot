obj-m := disk-test.o
BUILDROOT_DIR := /workspaces/buildroot
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc
all:
	$(COMPILER) -o disk-test disk-test.c
	cp disk-test $(BUILDROOT_DIR)/output/target/bin