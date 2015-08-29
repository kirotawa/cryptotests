EXTRA_CFLAGS := -I$(src)/include
obj-m += test.o 

test-objs := cryptotest.o algorithms/sha.o

DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(DIR) M=$(PWD) clean
