#
# Top-level makefile for Coda kernel modules
#

KVER ?= $(shell uname -r)
UTS_SHORT := $(shell echo ${KVER} | cut -d. -f-2)
export KVER

all install clean:
	make -C linux$(UTS_SHORT) $@

distclean:
	make -C linux2.6 clean
	make -C linux2.4 clean
	#make -C linux2.2 clean
	#make -C linux2.0 clean

