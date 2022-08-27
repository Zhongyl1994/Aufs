#
# File            : makefile
# Author          : ZhangLe
# CreateTime      : 2022-08-27 20:07:11
# LastModified    : 2022-08-27 21:03:33
# Vim             : ts=4, sw=4
#

obj-m += aufs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uanme -r)/build M=$(PWD) clean
