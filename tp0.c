#define N 256

 typedef struct num_complejo{
	float real;
	float imaginario;
 } complejo;

 complejo* crear_complejo(float real, float imaginaria){
 	complejo* num = malloc(sizeof(complejo));
 	if (num == NULL) return NULL;
 	num->real = real;
 	num->imaginario = imaginaria;
 	return num;
 }

 void destruir_complejo(complejo* num){
 	free(num);
 }

 typedef struct pixel{
	complejo numero_complejo;
 } pixel_t;
 
 complejo* crear_pixel(float real, float imaginaria){
 	pixel_t* pixel = malloc(sizeof(pixel));
 	if (pixel == NULL) return NULL;
 	complejo* num = crear_complejo(float real, float imaginaria);
 	if (num == NULL){
 		free(pixel);
 		return NULL;
 	}
 	pixel->numero_complejo = num;
 	return pixel;
 }

 void destruir_pixel(pixel_t* pixel){
 	destruir_complejo(pixel->numero_complejo);
 	free(pixel);
 }

 unsigned int modulo_al_2(complejo* num){
	return ((num2->real)**2 + (num2->imaginario)**2);
 }
 
 complejo* sumar(complejo* num1, complejo* num2){
	float real = num1->real + num2->real;
	float imaginario = num1->imaginario + num2->imaginario;
	return crear_complejo(real, imaginario);
 }
 
 complejo* al_2(complejo* num){
	float real = num->real**2 - num->imaginario**2;
	float imaginario = 2*num->real*num->imaginario;
	return crear_complejo(real, imaginario);
 }
 
 int velocidad_de_escape(pixel_t* pixel){
	complejo* num = pixel->numero_complejo;
	complejo* num2 = crear_complejo(num->real, num->imaginario);
	int i;
	for (i = 0; i < (N-1); i++){
		if ( modulo_al_2(num2) > 4) break;
		num2 = sumar(al_2(num2), num);
	}
	return i;
 }
