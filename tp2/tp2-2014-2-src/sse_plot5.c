#ifdef USE_SSE_ASSEMBLY
/*
 * System libraries.
 */

#include <stdio.h>
#include <unistd.h>

/*
 * Application libraries.
 */

#include <defs.h>
#include <debug.h>
#include <param5.h>

void sse_plot_unitario(param_t *parms);

void
sse_plot(void* args)
{
	void** arg = (void**) args;
	param_t* parms = *((param_t**) arg[0]);
	size_t size = *((size_t*) arg[1]);
	size_t i;
	for (i = 0; i < size; i++){
		sse_plot_unitario(&parms[i]);
	}
}



/* Useful constants. */
static float FFX4[4] __attribute__ ((aligned (16))) 
	= {255.0f, 255.0f, 255.0f, 255.0f};
static float FOUR[4] __attribute__ ((aligned (16))) 
	= {4.0f, 4.0f, 4.0f, 4.0f};
static float UNIT[4] __attribute__ ((aligned (16))) 
	= {1.0f, 1.0f, 1.0f, 1.0f};

#define INIT4(vec, val) \
	(vec)[0] = (vec)[1] = (vec)[2] = (vec)[3] = (val)

void
sse_plot_unitario(param_t *parms)
{
	uint8_t *u8;
	float CR0[4] __attribute__ ((aligned (16)));
	float CI0[4] __attribute__ ((aligned (16)));
	float CR[4] __attribute__ ((aligned (16)));
	float CI[4] __attribute__ ((aligned (16)));
	float ZR[4] __attribute__ ((aligned (16)));
	float ZI[4] __attribute__ ((aligned (16)));
	float DX[4] __attribute__ ((aligned (16)));
	float DY[4] __attribute__ ((aligned (16)));
	int ITER[4] __attribute__ ((aligned (16)));
	int x;
	int y;

	/* 
	 * Barremos la región rectangular del plano complejo comprendida 
	 * entre (parms->UL_re, parms->UL_im) y (parms->LR_re, parms->LR_im),
	 * calculando el color de cada punto de acuerdo a la fórmula de la
	 * recurrencia Z' = Z^3 + C
	 * 
	 * Nomenclatura:
	 * 
	 * o DX, DY representan los valores de las distancias entre pixels, 
	 *   medidas en unidades complejas.
	 * 
	 * o CR0, CI0 son las coordenadas complejas del pixel superior izq.
	 *
	 * o ZR, ZI son valores complejos usados para calcular el fractal 
	 *   descripto por la recurrencia de más arriba, Z = (ZR, ZI).
	 *
	 * o ITER contiene la cantidad de iteraciones asociadas a los pixels
	 *   cuyas coordenadas son (CR, CI).
	 * 
	 * o El parámetro de iteración son los puntos (CR, CI).
	 */

	INIT4(DX, 4.0 * parms->d_re);
	INIT4(DY, 1.0 * parms->d_im);

	/*
	 * Inicialización de (CR0, CI0). Notar que las coordenadas de este
	 * punto se encuentran desplazadas en 0.5 pixel, de forma tal de
	 * hacer el cálculo con el valor complejo del punto medio.
	 *
	 */

	CR0[0] = parms->UL_re + (parms->x0 + 0.5f) * parms->d_re;
	CR0[1] = parms->UL_re + (parms->x0 + 1.5f) * parms->d_re;
	CR0[2] = parms->UL_re + (parms->x0 + 2.5f) * parms->d_re;
	CR0[3] = parms->UL_re + (parms->x0 + 3.5f) * parms->d_re;
	INIT4(CI0, parms->UL_im - (parms->y0 + 0.5f) * parms->d_im);

	/* 
	 * u8 initialmente apunta al primer byte de la región de memoria en
	 * donde vamos a almacenar los tonos de los pixels calculados en esta
	 * corrida de la función.
	 */
	u8 = parms->bitmap + parms->y0 * parms->x_res + parms->x0;

	__asm__ volatile (
	"movaps    %1, %%xmm0 \n\t"
	"movaps    %%xmm0, %0 \n\t"
	: "=m" (CI)  /* %0 */
	: "m" (*CI0) /* %1 */
	: "xmm0"
	);

	for (y = parms->y0; y < parms->y1; ++y) {
		__asm__ volatile (
		"CR_eq_CR0:              \n\t"
		"movaps   %1, %%xmm0     \n\t"
		"movaps   %%xmm0, %0     \n\t"
		: "=m" (CR)  /* %0 */
		: "m" (*CR0) /* %1 */
		: "xmm0"
		);

		for (x = parms->x0; x < parms->x1; x += 4) {
			__asm__ volatile (
			"initialize:             \n\t"
			"xorps    %%xmm0, %%xmm0 \n\t" /* xmm0: ITER */
			"movaps   %9, %%xmm1     \n\t" /* xmm1: UNIT */
			"movaps   %3, %%xmm2     \n\t" /* xmm2: ZR = CR */
			"movaps   %4, %%xmm3     \n\t" /* xmm3: ZI = CI */

			"loop:                   \n\t"
			"movaps   %%xmm2, %%xmm4 \n\t" /* xmm4: ZR */
			"movaps   %%xmm3, %%xmm5 \n\t" /* xmm5: ZI */
			"mulps    %%xmm4, %%xmm4 \n\t" /* xmm4: ZR^2 */
			"mulps    %%xmm5, %%xmm5 \n\t" /* xmm5: ZI^2 */
			"movaps   %%xmm4, %%xmm6 \n\t" /* xmm6: ZR^2 */
			"addps    %%xmm5, %%xmm6 \n\t" /* xmm6: ZR^2 + ZI^2 */
			"cmpltps  %6, %%xmm6     \n\t" /* xmm6: abs(Z) <=> 2 */
			"movmskps %%xmm6, %%eax  \n\t" /* eax : abs(Z) <=> 2 */
			"andps    %%xmm6, %%xmm1 \n\t" /* xmm1: new INCR */
			"testl    %%eax, %%eax   \n\t"
			"jz       cast_output    \n\t"

			"check:                  \n\t"
			"movaps   %%xmm0, %%xmm6 \n\t" /* xmm6: ITER */
			"cmpeqps  %8, %%xmm6     \n\t" /* xmm6: ITER == 255 */
			"movmskps %%xmm6, %%eax  \n\t" /* eax : ITER == 255 */
			"testl    %%eax, %%eax   \n\t"
			"jnz      cast_output    \n\t"

			"increment:              \n\t"
			"addps    %%xmm1, %%xmm0 \n\t" /* xmm0: ITER */

			/* Calculate Z = Z^3 + C. */
			"Z_eq_Z3_plus_C:         \n\t"
			"movaps   %6, %%xmm6     \n\t" /* xmm6: FOUR */
			"subps    %%xmm1, %%xmm6 \n\t" /* xmm6: THREE */
			
			"movaps   %%xmm5, %%xmm7 \n\t" /* xmm7: ZI^2 */
			"mulps    %%xmm2, %%xmm7 \n\t" /* xmm7: ZR * ZI^2 */
			"mulps    %%xmm6, %%xmm7 \n\t" /* xmm7: 3 * ZR * ZI^2 */
			"mulps    %%xmm4, %%xmm2 \n\t" /* xmm2: ZR^3 */
			"subps    %%xmm7, %%xmm2 \n\t" /* xmm2: ZR^3 - 3 * ZR * ZI^2 */
			
			"mulps    %%xmm4, %%xmm6 \n\t" /* xmm6: 3 * ZR^2 */
			"mulps    %%xmm3, %%xmm6 \n\t" /* xmm6: 3 * ZR^2 * ZI */
			"mulps    %%xmm3, %%xmm5 \n\t" /* xmm5: ZI^3 */
			"subps    %%xmm5, %%xmm6 \n\t" /* xmm6:  3 * ZR^2 * ZI - ZI^3 */

			"addps    %3, %%xmm2     \n\t" /* xmm2: += CR */
			"addps    %4, %%xmm6     \n\t" /* xmm6: += CI */
										   /* xmm2: new ZR */
			"movaps   %%xmm6, %%xmm3 \n\t" /* xmm3: new ZI */

			"jmp      loop           \n\t"

			/*
			 * Escribimos los tonos de los pixels en la región de
			 * memoria correspondiente.
			 * 
			 * En este momento, la cantidad de iteraciones de cada
			 * pixel se encuentra en un registro de punto flotante
			 * XMM. Necesitamos convertir estos vamores a entero y
			 * luego almacenarlos en memoria.
			 */
			"cast_output:            \n\t"
			"movaps   %%xmm0, %%xmm1 \n\t" /* xmm1: final ITER */
			"shufps   $0x4e, %%xmm1, %%xmm1 \n\t" /* 0100 1110 */
			"cvtps2pi %%xmm0, %%mm0  \n\t"
			"cvtps2pi %%xmm1, %%mm1  \n\t"
			
			"write_px:               \n\t"
			"movd     %%mm0, %%eax   \n\t"
			"movb     %%al, (%0)     \n\t"

			"punpckhdq %%mm0, %%mm0  \n\t"
			"movd     %%mm0, %%eax   \n\t"
			"movb     %%al, 1(%0)    \n\t"

			"movd     %%mm1, %%eax   \n\t"
			"movb     %%al, 2(%0)    \n\t"

			"punpckhdq %%mm1, %%mm1  \n\t"
			"movd     %%mm1, %%eax   \n\t"
			"movb     %%al, 3(%0)    \n\t"

			"fin:                    \n\t"
#if _LP64
			"addq     $4, %0         \n\t"
#else
			"addl     $4, %0         \n\t"
#endif

			: "=r" (u8),      /* %0 */
			  "=m" (ZR),      /* %1 */
			  "=m" (ZI)       /* %2 */
			: "m" (*CR),      /* %3 */
			  "m" (*CI),      /* %4 */
			  "m" (*ITER),    /* %5 */
			  "m" (*FOUR),    /* %6 */
			  "0" (u8),       /* %7 */
			  "m" (*FFX4),    /* %8 */
			  "m" (*UNIT)     /* %9 */
			: "xmm0", "xmm1", "xmm2", "xmm3", 
			  "xmm4", "xmm5", "xmm6", "xmm7",
			  "mm0", "mm1", "eax", "ebx",
			  "cc", "memory"
			);

			__asm__ volatile (
			"CR_eq_CR_plus_DX:       \n\t"
			"movaps   %0, %%xmm0     \n\t"
			"addps    %1, %%xmm0     \n\t"
			"movaps   %%xmm0, %0     \n\t"
			: "=m" (CR)    /* %0 */
			: "m" (*DX)    /* %1 */
			: "xmm0"
			);
		}

		__asm__ volatile (
		"CI_eq_CI_minus_DY:      \n\t"
		"movaps   %0, %%xmm0     \n\t"
		"subps    %1, %%xmm0     \n\t"
		"movaps   %%xmm0, %0    \n\t" /* CI -= DY */
		: "=m" (CI)    /* %0 */
		: "m" (*DY)    /* %1 */
		: "xmm0"
		);
		
		size_t suma_u8 = parms->x_res - (parms->x1 - parms->x0);
		
		__asm__ volatile (
		"U8_avanza_una_linea:    \n\t"
#if _LP64
			"addq     %2, %0         \n\t"
#else
			"addl     %2, %0         \n\t"
#endif		
		: "=r" (u8)		/* %0 */
		: "0" (u8),		/* %1 */
		  "m" (suma_u8)/* %2 */
		: "cc"
		);

		__asm__ volatile (
		"call_emms:              \n\t"
		"emms                    \n\t"
		:
		:
		: "cc"
		);
	}
}
#endif
