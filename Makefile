obj-m += test.o 

test-objs := cryptotest.o sha.o
CFLAGS_test.o := -DDEBUG

DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(DIR) M=$(PWD) clean
