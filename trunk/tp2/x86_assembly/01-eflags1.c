/*
 * 01-eflags1.c
 *
 * Imprime algunos campos del registro EFLAGS.
 *
 * $Date: 2008/06/10 18:50:00 $
 */

#include <stdio.h>
#include <stdint.h>

int
main(int argc, char * const argv[])
{
	uint32_t r;

	__asm__ volatile (
		"pushfl    \n\t"
		"popl   %0 \n\t"
	: "=g" (r)
	:
	);

#define BIT(r, i) ((r) & (1 << (i)) ? 1 : 0)
	printf("eflags: 0x%08x\n", r);
	printf("carry: %d\n", BIT(r, 0));
	printf("1: %d\n", BIT(r, 1));
	printf("0: %d\n", BIT(r, 3));
	printf("0: %d\n", BIT(r, 5));
	printf("interrupt enable: %d\n", BIT(r, 9));
	printf("i/o privilege level: %d\n", (r >> 12) & 3);
	printf("virtual-8086 mode: %d\n", BIT(r, 17));
	printf("alignment check: %d\n", BIT(r, 18));
	printf("id: %d\n", BIT(r, 21));
	
	return 0;
}
