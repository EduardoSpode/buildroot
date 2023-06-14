BUILDROOT_DIR := /workspace*/buildroot
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc

all:
	$(COMPILER) -o thread_runner thread_runner.c -lpthread -Wall
	cp thread_runner $(BUILDROOT_DIR)/output/target/bin
	
clean:
	rm -f *.o 
	rm -f thread_runner