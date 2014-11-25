/*
 * 03-eflags3.c
 *
 * Similar a eflags1.c y eflags2.c; en este caso también mostramos
 * el valor del bit de paridad, y recibimos un sólo argumento para
 * evaluarlo con testl.
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int
main(int argc, char * const argv[])
{
	int i;
	int j;
	char c;
	uint32_t r;

	if (argc != 3)
		exit(1);

	if (sscanf(argv[1], "%d%c", &i, &c) != 1
	    || sscanf(argv[2], "%d%c", &j, &c) != 1)
		exit(1);

	__asm__ volatile (
		"testl  %1, %2 \n\t"
		"pushfl        \n\t"
		"popl   %0     \n\t"
	: "=g" (r)
	: "r" (i), "r" (j)
	: "cc"
	);

#define BIT(r, i) ((r) & (1 << (i)) ? 1 : 0)
	printf("eflags: 0x%08x\n", r);
	printf("carry: %d\n", BIT(r, 0));
	printf("1: %d\n", BIT(r, 1));
	printf("parity: %d\n", BIT(r, 2));
	printf("0: %d\n", BIT(r, 3));
	printf("0: %d\n", BIT(r, 5));
	printf("zero: %d\n", BIT(r, 6));
	printf("sign: %d\n", BIT(r, 7));
	printf("interrupt enable: %d\n", BIT(r, 9));
	printf("i/o privilege level: %d\n", (r >> 12) & 3);
	printf("virtual-8086 mode: %d\n", BIT(r, 17));
	printf("alignment check: %d\n", BIT(r, 18));
	printf("id: %d\n", BIT(r, 21));
	
	return 0;
}
