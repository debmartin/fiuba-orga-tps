/*
 * 11-rdtsc.c
 *
 * Ejemplo de cómo usar el contador TSC.
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define IA32_CPUID_BASIC_INFO 0
#define IA32_CPUID_FEATURE    1

#define IA32_FEATURE_FPU  0
#define IA32_FEATURE_TSC  4
#define IA32_FEATURE_MMX  23
#define IA32_FEATURE_SSE  25
#define IA32_FEATURE_SSE2 26
#define IA32_FEATURE_SSE3 9
#define IA32_FEATURE_HTT  28

static void do_cpuid(uint32_t, uint32_t [4]);
static int has_cpuid(void);
static int has_rdtsc(void);

int
main(int argc, char * const argv[])
{
	uint64_t tsc;
	struct timeval tv;
	struct timeval tv0;

	if (!has_rdtsc()) {
		fprintf(stderr, "No TSC detected.\n");
		exit(1);
	}

#define rdtsc(u64) \
	__asm__ __volatile__ ("rdtsc" : "=A" (u64))

	gettimeofday(&tv0, 0);
	while (1) {
		rdtsc(tsc);
		gettimeofday(&tv, 0);

#define DS(x, y)      \
	((x).tv_sec - (y).tv_sec)
#define DU(x, y)      \
	((DS(tv, tv0) ? 1000000L : 0) + (x).tv_usec - (y).tv_usec)
#define DELTA_S(x, y) \
	(DU(x, y) / 1000000L + DS(x, y))
#define DELTA_U(x, y) \
	(DU(x, y) % 1000000L)

		printf("%3lu s %06lu us %llu clocks.\n", 
		       DELTA_S(tv, tv0), 
		       DELTA_U(tv, tv0), 
		       tsc);
		sleep(1);
	}

	return 0;
}

static void
do_cpuid(uint32_t what, uint32_t regs[4])
{
	__asm__ volatile (
		"cpuid           \n\t"
	: "=a" (regs[0]),
	  "=b" (regs[1]),
	  "=c" (regs[2]),
	  "=d" (regs[3])
	: "0" (what)
	);
}

static int
has_cpuid(void)
{
	uint32_t x;
	uint32_t y;

	/*
	 * Intel IA-32 Software Developer's Manual. Volume 1: Basic
	 * architecture.
	 *
	 * 14.1 Using the cpuid instruction.
	 *
	 * ...
	 *
	 * Use the CPUID instruction for processor identification in the
	 * Pentium M processor family, Pentium 4 processor family, Intel
	 * Xeon processor family, P6 family, Pentium processor, and later
	 * Intel486 processors.
	 *
	 * ...
	 *
	 * The ID flag (bit 21) in the EFLAGS register indicates support for
	 * the CPUID instruction. If a software procedure can set and clear
	 * this flag, the processor executing the procedure supports the 
	 * CPUID instruction. The CPUID instruction will cause the invalid
	 * opcode exception (#UD) if executed on a processor that does not
	 * support it.
	 * 
	 * To obtain processor identification information, a source operand
	 * value is placed in the EAX register to select the type of 
	 * information to be returned. When the CPUID instruction is executed,
	 * selected information is returned in the EAX, EBX, ECX, and EDX 
	 * registers.
	 * 
	 * ...
	 *
	 */
	__asm__ volatile (
		"pushf                 \n\t"
		"popl  %0              \n\t" /* eax: eflags */
		"movl  %0, %1          \n\t" /* ebx: eflags */
		"xorl  $0x00200000, %0 \n\t" /* eax: toggle bit 21 */
		"pushl %0              \n\t"
		"popf                  \n\t" /* set new eflags */
		"pushf                 \n\t" /* get new eflags */
		"popl  %0              \n\t" /* eax: new eflags */
	: "=a" (x), "=b" (y)
	:
	: "cc"
	);

	return x != y ? 1 : 0;
}

static int
has_rdtsc(void)
{
	uint32_t regs[4];

	if (has_cpuid() != 0) {
		do_cpuid(IA32_CPUID_BASIC_INFO, regs);
		if (regs[0] >= 1) {
#define BIT(r, i) ((r) & (1U << (i)) ? 1 : 0)
#define YESNO(b) ((b) ? "yes" : "no")
#define HAS_FEATURE(r, f) (YESNO(BIT((r), (f))))
			if (HAS_FEATURE(regs[3], IA32_FEATURE_TSC))
				return 1;
		}
	}

	return 0;
}
