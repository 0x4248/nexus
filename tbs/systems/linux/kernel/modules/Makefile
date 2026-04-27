obj-m := proc.o
demo-objs := proc.o
demo-objs += $(patsubst %.c,%.o,$(wildcard lib/*.c))

KDIR := /usr/lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
