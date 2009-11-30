#
# Makefile for the Linux Coda filesystem routines.
#


obj-$(CONFIG_CODA_FS) += coda.o

coda-objs := psdev.o cache.o cnode.o inode.o dir.o file.o upcall.o \
	     coda_linux.o symlink.o pioctl.o sysctl.o

#
# This makes the makefile function outside of the kernel
# tree when building a standalone module
#

KVER ?= $(shell uname -r)
KSRC ?= /lib/modules/${KVER}/source
KBLD ?= /lib/modules/${KVER}/build
pwd  := $(shell pwd)

coda_makeflags := CONFIG_CODA_FS=m \
	NOSTDINC_FLAGS="-nostdinc -isystem $(shell $(CC) -print-file-name=include) -I${pwd}" \
	INSTALL_MOD_DIR=kernel/fs/coda -C ${KBLD} M=${pwd}

all: ${KSRC}
	make ${coda_makeflags}

install: ${KSRC}
	make ${coda_makeflags} modules_install
	-/sbin/depmod -a

clean:
	-rm -rf *.o .*.o.cmd *.ko .*.ko.cmd *.mod.c .tmp_versions

${KSRC}:
	@echo ===
	@echo === Cannot find the source directory for your ${KVER} kernel.
	@echo ===
	@echo === I was expecting to find a link at ${KBLD}
	@echo === pointing at the source tree. You might have to install a
	@echo === kernel-headers and/or kernel-kbuild package from your
	@echo === distribution that matches this kernel.
	@echo ===
	@exit 1

