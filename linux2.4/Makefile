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

coda_cppflags := -D__KERNEL__ -I${pwd} -Iinclude -I${KSRC}/include
coda_makeflags := CONFIG_CODA_FS=m \
		  CPPFLAGS="${coda_cppflags}" \
		  KERNELRELEASE=${KVER} MOD_DESTDIR=fs/coda \
		  -C ${KSRC} SUBDIRS=${pwd}

all:
	[ -d ${KSRC} ] || make error
	[ -f ${KSRC}/.config ] || make INCLUDES_ONLY=${KSRC} modules
	[ ! -f ${KSRC}/.config ] || make ${coda_makeflags} modules

ifdef INCLUDES_ONLY
modules: ${O_TARGET}
${O_TARGET}: ${obj-y}
	ld -m "`ld --help | awk '/supported emulations/ {print $$4}'`" -r -o $@ $^
.c.o:
	cc $(shell ./kconfig) ${coda_cppflags} -DMODULE -c $<
endif

install: all
	# we can't use the kernel's make modules_install as it removes
	# all the existing modules.
	/bin/cp coda.o /lib/modules/${KVER}/kernel/fs/coda
	/sbin/depmod -ae

clean:
	rm -f *.o .*.o.flags modversions.h

error:
	@echo ===
	@echo === Cannot find the source directory for your ${KVER} kernel.
	@echo ===
	@echo === I was expecting to find the source or a link to the sources
	@echo === at ${KSRC}. You might have to install a
	@echo === kernel-headers-${KVER} and/or kernel-kbuild package from
	@echo === your distribution that matches this kernel.
	@echo ===
	@echo === You can also set the KSRC environment variable to point at
	@echo ===  your kernel source tree.
	@echo ===
	@exit 1

ifdef TOPDIR
include ${TOPDIR}/Rules.make
endif

