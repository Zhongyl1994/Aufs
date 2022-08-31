#
# File            : makefile
# Author          : ZhangLe
# CreateTime      : 2022-08-27 20:07:11
# LastModified    : 2022-08-31 10:02:18
# Vim             : ts=4, sw=4
#

#obj-$(CONFIG_HELLO_FS) += hello.o
obj-m += hello.o
EXTRA_CFLAGS += -g

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
