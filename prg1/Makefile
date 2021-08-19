obj-m:= chardriver.o
KDIR= /lib/modules/$(shell uname -r)/build
PWD:= $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o  Application1 Application2 Application1.c Application2.c

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm Application1 Application2

install:
	make -C $(KDIR) M=$(PWD) modules_install
