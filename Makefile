ifneq ($(KERNELRELEASE),)
	obj-m := hallo.o
else
KDIR := /lib/modules/$$(uname -r)/build/ 
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif

clean:
	rm -f *.o modules.* Module.* *.ko *.mod.c
