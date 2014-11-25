/*
 * 10-cpuid.c
 *
 * Código de ejemplo, pensado para para detectar la presencia de SSE en
 * el hardware. Nota: necesitaríamos complementarlo con código UNIX 
 * pensado para detectar soporte SSE en el sistema operativo, ya que esto
 * no lo estamos haciendo acá.
 *
 * Ver también la sección 3.2 del "Intel (r) 64 and IA-32 architectures
 * software developer's manual, volume 2A: instruction set reference, A-M".
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdint.h>

#define IA32_CPUID_BASIC_INFO 0
#define IA32_CPUID_FEATURE    1

#define IA32_FEATURE_FPU  0
#define IA32_FEATURE_TSC  4
#define IA32_FEATURE_MMX  23
#define IA32_FEATURE_SSE  25
#define IA32_FEATURE_SSE2 26
#define IA32_FEATURE_SSE3 9
#define IA32_FEATURE_HTT  28

static int has_cpuid(void);

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

int 
main(int argc, char * const argv[])
{
	uint32_t regs[4];

	if (!has_cpuid()) {
		printf("has_cpuid(): no.\n");
	} else {
		printf("has_cpuid(): yes.\n");
		do_cpuid(IA32_CPUID_BASIC_INFO, regs);
		printf("Highest cpuid operand: %u.\n", regs[0]);
		printf("IA32 Basic information.\n");
		printf("\tEbx: %c%c%c%c.\n", 
		       (regs[1] >> 0) & 0xff,
		       (regs[1] >> 8) & 0xff,
		       (regs[1] >> 16) & 0xff,
		       (regs[1] >> 24) & 0xff);
		printf("\tEcx: %c%c%c%c.\n", 
		       (regs[2] >> 0) & 0xff,
		       (regs[2] >> 8) & 0xff,
		       (regs[2] >> 16) & 0xff,
		       (regs[2] >> 24) & 0xff);
		printf("\tEdx: %c%c%c%c.\n", 
		       (regs[3] >> 0) & 0xff,
		       (regs[3] >> 8) & 0xff,
		       (regs[3] >> 16) & 0xff,
		       (regs[3] >> 24) & 0xff);
		if (regs[0] >= 1) {
#define BIT(r, i) ((r) & (1U << (i)) ? 1 : 0)
#define YESNO(b) ((b) ? "yes" : "no")
#define HAS_FEATURE(r, f) (YESNO(BIT((r), (f))))
			do_cpuid(IA32_CPUID_FEATURE, regs);
			printf("IA32 Feature information.\n");
			printf("\tEcx: 0x%08x.\n", regs[2]);
			printf("\tEdx: 0x%08x.\n", regs[3]);
			printf("\tFPU: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_FPU));
			printf("\tTSC: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_TSC));
			printf("\tMMX: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_MMX));
			printf("\tSSE: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_SSE));
			printf("\tSSE2: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_SSE2));
			printf("\tSSE3: %s.\n", 
			       HAS_FEATURE(regs[2], IA32_FEATURE_SSE3));
			printf("\tHTT: %s.\n", 
			       HAS_FEATURE(regs[3], IA32_FEATURE_HTT));
		}
	}

	return 0;
}
