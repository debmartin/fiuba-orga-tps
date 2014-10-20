#include <mips/regdef.h>
#include <sys/syscall.h>
.abicalls

.text
.align 2

.global mips32_plot
.entity mips32_plot

mips32_plot:

#define FRAME_SPACE 56
.frame $fp, FRAME_SPACE, ra
subu sp, sp, FRAME_SPACE # Pongo el stack pointer al final de mi frame.
.cprestore 40		# El gp en 40(sp)
sw $fp, 44(sp)
sw ra, 48(sp)

#Pongo el frame pointer al final de mi frame.
or $fp, sp, zero


#Offset dentro de la estructura param.
#define OFF_UPPER_LEFT_REAL 0			#float: 1 registro f
#define OFF_UPPER_LEFT_IMAG 4 			#float: 1 registro f
#define OFF_LOWER_RIGHT_REAL 8 			#float: 1 registro f
#define OFF_LOWER_RIGHT_IMAG 12 		#float: 1 registro f
#define OFF_SCALE_REAL 16 				#float: 1 registro f
#define OFF_SCALE_IMAG 20 				#float: 1 registro f
#define OFF_RES_REAL 24 				#size_t = unsigned: 1 registros
#define OFF_RES_IMAG 32 				#size_t = unsigned: 1 registros
#define OFF_SHADES 40 					#size_t = unsigned: 1 registros
#define OFF_OUTPUT_FILE 48 				#puntero: 1 registro
#Offset del numero de archivo dentro de la estructura _sFile
#define OFF_FILE_NUMBER 112 			#numero: 1 registro


sw a0, FRAME_SPACE($fp)

l.s f0, OFF_UPPER_LEFT_IMAG(a0) # F0 para upper_left_im
l.s f1, OFF_SCALE_IMAG(a0) # F1 para d_im

lw f3, OFF_UPPER_LEFT_REAL(a0) # F3 para upper_left_re
l.s f4, OFF_SCALE_REAL(a0) # F4 para d_re

la t6, OFF_OUTPUT_FILE(a0)
lw t6, OFF_FILE_NUMBER(t0) # T6 para el file descriptor.

imprimir_p2:
	li v0, sys_WRITE
	move a0, t6
	la a1, p2
	li a2, LARGO_P2
	syscall
	# Si me devolvió un número negativo en v0, hubo error.
	bne a3, zero, imprimir_io_error

imprimir_res_x:
	lw t8, OFF_RES_REAL(a0)
	li t9, imprimir_res_y
	j imprimir_numero
	
imprimir_res_y:
	lw t8, OFF_RES_IMAG(a0)
	li t9, imprimir_max_gray
	j imprimir_numero
	
imprimir_max_gray:
	lw t8, OFF_SHADES(a0)
	li t9, empezar
	j imprimir_numero

empezar:
	li t0, 0 				# T0 es y
	lw t1, OFF_RES_IMAG(a0) # T1 res_y
	mov.s f2, f0 			# F2 copia del up_im para ir modificando (ci)
	li t2, 0 				# T2 para x
	lw t3, OFF_RES_REAL(a0) # T3 para res_x
	mov.s f5, f3 			# F5 copia del up_re (cr)
	li t4, 0 				# T4 para la velocidad de escape.
	lw t5, OFF_SHADES(a0)	# T5 para el máximo de grises

loop_vertical:
	beq t0, t1, terminar # Si y llegó a ser el último pixel, termino.
	loop_horizontal:
		beq t2, t3, aumento_loop_vert

		mov.s f6, f2 # F6 guarda la copia del ci (zi)
		mov.s f7, f5 # F7 guarda la copia del cr (zr)

		loop_mandelbrot:
			beq t4, t5, imprimir_brillo

			# Calculo el módulo
			mul.s f8, f6, f6 # F8 guarda zi*zi
			mul.s f9, f7, f7 # F9 guarda zr*zr
			add.s f10, f8, f9 # F10 guarda zi*zi+zr*zr
			li.s f11, 4.0 # F11 guarda 4
			c.lt.s f10, f11 # Si F10<F11 pone el flag en true.
			bclf imprimir_brillo # Va a imprimir brillo si el flag es false, es decir, si el módulo es mayor a 4.

			mul.s f10, f8, f6 # F10 guarda zi*zi*zi
			mul.s f11, f9, f7 # F11 guarda zr*zr*zr
			mul.s f12, f8, f7 # F12 guarda zi*zi*zr
			mul.s f13, f9, f6 # F13 guarda zr*zr*zi
			li.s f14, 3.0 # F14 guarda 3
			mul.s f12, f14, f12 # F12 guarda 3*zi*zi*zr
			mul.s f13, f14, f13 # F13 guarda 3*zr*zr*zi
			sub.s f11, f11, f12 # F11 guarda zr*zr*zr - 3*zi*zi*zr
			add.s f11, f11, f5 # F11 guarda sr
			sub.s f13, f13, f10 # F13 guarda 3*zr*zr*zi - zi*zi*zi
			add.s f13, f13, f2 # F13 guarda si
			mov.s f6, f13
			mov.s f7, f11

			add t4, t4, 1 # Aumento la velocidad de escape en 1
			j loop_mandelbrot

		imprimir_brillo:
	#define OFF_FRAME_Y 16
	#define OFF_FRAME_RES_Y 20
	#define OFF_FRAME_X 24
	#define OFF_FRAME_RES_X 28
	#define OFF_FRAME_VEL 32
		guardar:	
			sw t0, OFF_FRAME_Y($fp)
			sw t1, OFF_FRAME_RES_Y($fp)
			sw t2, OFF_FRAME_X($fp)
			sw t3, OFF_FRAME_RES_X($fp)
			sw t4, OFF_FRAME_VEL($fp)
			
			move t8, t4
			li t9, recargar
			j imprimir_numero
		
		recargar:
			lw t0, OFF_FRAME_Y($fp)
			lw t1, OFF_FRAME_RES_Y($fp)
			lw t2, OFF_FRAME_X($fp)
			lw t3, OFF_FRAME_RES_X($fp)
			lw t4, OFF_FRAME_VEL($fp)
		
		imprimir_enter:
			li v0, sys_WRITE
			move a0, t6
			la a1, enter
			li a2, LARGO_ENTER
			syscall
			# Si me devolvió un número negativo en v0, hubo error.
			bne a3, zero, imprimir_io_error

	aumento_loop_hor:
		add t2, t2, 1 		# Aumento x en 1.
		add.s f5, f5, d_re 	# Aumento cr según la escala.
		j loop_horizontal
aumento_loop_vert:
	add t0, t0, 1 		# Aumento y en 1.
	sub.s f2, f2, f1 	# A ci le resto según la escala.
	j loop_vertical


terminar:
	lw ra, 32($fp)
	lw gp, 24($fp)
	lw $fp, 28($fp)
	addu sp, sp, FRAME_SPACE
	jr ra

#define MAX_DIV 100		# Es el máximo divisor de acuerdo a la cantidad máxima de cifras que puede tener el número a imprimir.

imprimir_numero:	# Recibe registros T6, T8 (y lo cambia) y T9; usa registros T0, T1, T2, T3 y T4
# T6 es el archivo donde debo imprimir.
# T8 tiene el número de 3 cifras máximo.
# T9 tiene a donde debo volver para seguir ejecutando el programa.
	li t0, MAX_DIV		# T0 para el divisor.
	imp_cifra:
		be t0, 0, volver	# Si ya imprimió todo, vuelve al programa.
		divu t1, t8, t0		# T1 para el cociente de num/div
		rem t2, t8, t0		# T2 para el resto de num/div
	proximo:
		move t8, t2
		divu t0, t0, 10		# Para testear la siguiente cifra.
		j imp_cifra
	escribir_num:
		# El número a imprimir está en T1
		la t3, numeros		# T3 para la dirección del arreglo.
		# La posición en el arreglo es número por 4 (ya que cada palabra son 4 bytes).
		sll t4, t1, 2		# T4 tiene la poscición del número en el arreglo.
		addu t4, t3, t4		# T4 tiene la dirección donde está la dirección de la etiqueta del número.
		lw t4, 0(t4)		# T4 tiene dirección de la etiqueta del número.
		
		li v0, sys_WRITE
		li a0, t6
		lw a1, 0(t4)
		li a2, LARGO_NUM
		syscall
		# Si me devolvió un número negativo en v0, hubo error.
		bne a3, zero, imprimir_io_error
		j proximo
	volver:
		jr t9


		
imprimir_io_error:
	li v0, 15
	li a0, STD_ERR
	la a1, error_io
	li a2, LARGO_IO_ERROR
	syscall
	ba exit

exit:
	li v0, SYS_exit
	syscall


.end mips32_plot


.rdata
#define LARGO_P2 3
p2: .asciiz "P2\n"

.align 2
#define LARGO_NUM 1
numeros: .word cero, uno, dos, tres, cuatro cinco, seis, siete, ocho, nueve
cero: .asciiz "0"
uno: .asciiz "1"
dos: .asciiz "2"
tres: .asciiz "3"
cuatro: .asciiz "4"
cinco: .asciiz "5"
seis: .asciiz "6"
siete: .asciiz "7"
ocho: .asciiz "8"
nueve: .asciiz "9"

#define LARGO_ENTER
enter: .asciiz "\n"

#define LARGO_IO_ERROR 11
error_io: .asciiz "i/o error.\n"








# Para imprimir Strings:
#li v0, sys_WRITE
###Supongo que en t0 está el file descriptor
#or a0, t0, zero
#la a1, dir_del_string
#li a2, largo_en_caracteres_del_string
#syscall
# Si me devolvió un número negativo en v0, hubo error.
#bne a3, zero, exit
