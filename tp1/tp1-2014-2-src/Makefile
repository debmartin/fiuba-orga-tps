# Usage: 
#	make makefiles [CC=compiler] [OPT=compiler-flags] [DEBUG=debug-flags]
#
# The defaults are: CC=gcc, OPT=-O, and DEBUG=-g. Examples:
#
#	make makefiles
#	make makefiles CC="purify cc"
#	make makefiles CC=cc OPT=
#
SHELL   = /bin/sh

default: update

update depend clean: Makefiles
	$(MAKE) MAKELEVEL= $@

makefiles Makefiles:
	$(MAKE) -f Makefile.in MAKELEVEL= Makefiles
# DO NOT DELETE
