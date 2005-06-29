#
# Makefile for the Linux Coda filesystem routines.
#

O_TARGET := coda.o

obj-y := psdev.o cache.o cnode.o inode.o dir.o file.o upcall.o coda_linux.o \
	 symlink.o pioctl.o sysctl.o 
obj-m := $(O_TARGET)

#
# This makes the makefile function outside of the kernel
# tree when building a standalone module
#

KVER ?= $(shell uname -r)
KMOD ?= /lib/modules/${KVER}
KSRC ?= ${KMOD}/build
pwd  := $(shell pwd)

coda_makeflags := CONFIG_CODA_FS=m \
		  CPPFLAGS="-D__KERNEL__ -I${pwd} -Iinclude -I${KSRC}/include" \
		  KERNELRELEASE=${KVER} MOD_DESTDIR=fs/coda \
		  -C ${KSRC} SUBDIRS=${pwd}

all: ${KSRC}
	make ${coda_makeflags} modules

install: all
	# we can't use the kernel's make modules_install as it removes
	# all the existing modules.
	/bin/cp coda.o /lib/modules/${KVER}/kernel/fs/coda
	/sbin/depmod -ae

clean:
	rm -f *.o .*.o.flags

${KSRC}/.config:
	@echo ===
	@echo === Cannot find the source directory for your ${KVER} kernel.
	@echo ===
	@echo === I was expecting to find a link at ${KSRC}
	@echo === pointing at the source tree. You might have to install a
	@echo === kernel-headers-${KVER} and/or kernel-kbuild package from
	@echo === your distribution that matches this kernel.
	@echo ===
	@exit 1

ifdef TOPDIR
include ${TOPDIR}/Rules.make
endif

