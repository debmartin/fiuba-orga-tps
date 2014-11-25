#include <debug.h>
#include <stdio.h>
#include <defs.h>
#include <stdint.h>
#include <param4.h>

void
generic_plot(param_t *parms)
{
	uint8_t *u8;
	float cr, ci;
	float zr, zi;
	float sr, si;
	float absz;
	int x, y;
	int c;

	/* 
	 * Barremos la región rectangular del plano complejo comprendida 
	 * entre (parms->UL_re, parms->UL_im) y (parms->LR_re, parms->LR_im).
	 * El parámetro de iteración es el punto (cr, ci).
	 */
	
	ci = parms->UL_im - parms->y0 * parms->d_im;
	u8 = parms->bitmap + parms->y0 * parms->x_res + parms->x0;

	for (y = parms->y0; y < parms->y1; ++y) {
		cr = parms->UL_re + parms->x0 * parms->d_re;

		for (x = parms->x0; x < parms->x1; ++x) {
			zr = cr;
			zi = ci;

			/*
			 * Determinamos el nivel de brillo asociado al punto
			 * (cr, ci), usando la fórmula compleja recurrente 
			 * f = f^3 + c.
			 */
			for (c = 0; c < parms->shades; ++c) {
				if ((absz = zr*zr + zi*zi) >= 4.0f)
					break;
				sr = zr * zr * zr
				   - 3 * zr * zi * zi
				   + cr;
				si = 3 * zr * zr * zi
				   - zi * zi * zi
				   + ci;
				zr = sr;
				zi = si;
			}

			/* Escribimos la intensidad del px. */
			*u8++ = (uint8_t)c;

			/* Calculamos la siguiente parte real. */
			cr += parms->d_re;
		}
		/* Calculamos el lugar en el bitmap de la proxima linea. */
		u8 += (parms->x_res - (parms->x1 - parms->x0));

		/* Calculamos la siguiente parte compleja. */
		ci -= parms->d_im;
	}
}
