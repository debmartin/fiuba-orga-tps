#ifndef VELOCIDAD_ESCAPE_H_INCLUDED
#define VELOCIDAD_ESCAPE_H_INCLUDED

typedef struct pixel pixel_t;

int velocidad_de_escape(pixel_t* pixel);

pixel_t* crear_pixel(float real, float imaginaria);

void destruir_pixel(pixel_t* pixel);

#endif // VELOCIDAD_ESCAPE_H_INCLUDED
