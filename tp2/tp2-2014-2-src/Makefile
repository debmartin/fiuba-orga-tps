<<<<<<< .mine
# Do not edit -- this file documents how this program was built for your machine.
SYSTYPE	= LINUX3
SYSLIBS	= -lpthread -msse
CC	= gcc $(WARN) -g -DUSE_SSE_ASSEMBLY -msse
OPT	= -O
DEBUG	= -g
EXPORT	= AUXLIBS='' CCARGS='-g -DUSE_SSE_ASSEMBLY -msse' OPT='-O' DEBUG='-g'
WARN	= -Wall
SHELL	= /bin/sh
DEFS    = -I. -D$(SYSTYPE)
CFLAGS  = $(DEBUG) $(OPT) $(DEFS)
OPTS	= 'CC=$(CC)'
HDRS    = debug.h defs.h mygetopt.h param4.h
SRCS	= generic_plot4.c sse_plot4.c main4.c mygetopt_long.c
PROG	= tp2-2014-2-bin
AUXLIBS = 

$(PROG): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(SYSLIBS)

update: $(PROG)
	:

makefiles Makefiles:
	(echo "# Do not edit -- this file documents how this program was built for your machine."; $(SHELL) makedefs) >makedefs.tmp
	set +e;                                \
	if cmp makedefs.tmp makedefs.out; then \
	    rm makedefs.tmp;                   \
	else                                   \
	    mv makedefs.tmp makedefs.out;      \
	fi >/dev/null 2>/dev/null
	rm -f Makefile; (cat makedefs.out Makefile.in) >Makefile

clean:
	rm -f Makefile
	cp Makefile.init Makefile
	rm -f bin/[!CRS]* lib/[!CRS]* include/[!CRS]* libexec/[!CRS]*     \
	      junk *core .nfs* .pure *.out *.out.* *.tmp *.a *~ *- *.orig \
	      *.t *.o *.bak make.err *.gmon $(PROG)
=======
# Do not edit -- this file documents how this program was built for your machine.
SYSTYPE	= LINUX3
SYSLIBS	= -lc_p -lpthread
CC	= gcc $(WARN) -pg
OPT	= -O
DEBUG	= -g
EXPORT	= AUXLIBS='-lc_p' CCARGS='-pg' OPT='-O' DEBUG='-g'
WARN	= -Wall
SHELL	= /bin/sh
DEFS    = -I. -D$(SYSTYPE)
CFLAGS  = $(DEBUG) $(OPT) $(DEFS)
OPTS	= 'CC=$(CC)'
HDRS    = debug.h defs.h mygetopt.h param.h
SRCS	= generic_plot.c sse_plot.c main.c mygetopt_long.c
PROG	= tp2-2014-2-bin
AUXLIBS = 

$(PROG): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(SYSLIBS)

update: $(PROG)
	:

makefiles Makefiles:
	(echo "# Do not edit -- this file documents how this program was built for your machine."; $(SHELL) makedefs) >makedefs.tmp
	set +e;                                \
	if cmp makedefs.tmp makedefs.out; then \
	    rm makedefs.tmp;                   \
	else                                   \
	    mv makedefs.tmp makedefs.out;      \
	fi >/dev/null 2>/dev/null
	rm -f Makefile; (cat makedefs.out Makefile.in) >Makefile

clean:
	rm -f Makefile
	cp Makefile.init Makefile
	rm -f bin/[!CRS]* lib/[!CRS]* include/[!CRS]* libexec/[!CRS]*     \
	      junk *core .nfs* .pure *.out *.out.* *.tmp *.a *~ *- *.orig \
	      *.t *.o *.bak make.err *.gmon $(PROG)
>>>>>>> .r98
