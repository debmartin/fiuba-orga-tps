#include <mips/regdef.h>
#include <sys/syscall.h>
.abicalls

.text
.align 2

.global mips32_plot
.ent mips32_plot

mips32_plot:

FRAME_SPACE = 72

# Información de datos
LARGO_NUM = 1
LARGO_P2 = 3
LARGO_ENTER = 1
LARGO_IO_ERROR = 11

# Offset dentro de la estructura param:
OFF_UPPER_LEFT_REAL = 0				#float: 1 registro f
OFF_UPPER_LEFT_IMAG = 4 			#float: 1 registro f
OFF_LOWER_RIGHT_REAL = 8 			#float: 1 registro f
OFF_LOWER_RIGHT_IMAG = 12 			#float: 1 registro f
OFF_SCALE_REAL = 16 				#float: 1 registro f
OFF_SCALE_IMAG = 20 				#float: 1 registro f
OFF_RES_REAL = 24 					#size_t = unsigned: 1 registros
OFF_RES_IMAG = 28 					#size_t = unsigned: 1 registros
OFF_SHADES = 32 					#size_t = unsigned: 1 registros
OFF_OUTPUT_FILE = 36 				#puntero: 1 registro
# Offset del numero de archivo dentro de la estructura _sFile
OFF_FILE_NUMBER = 14 				#short int: 1/2 registro

# Offset dentro del frame:
OFF_FRAME_FILE_NO = 0
OFF_FRAME_Y = 4
OFF_FRAME_RES_Y = 8
OFF_FRAME_X = 12
OFF_FRAME_RES_X = 16
OFF_FRAME_VEL = 20
OFF_FRAME_D_IM = 24
OFF_FRAME_CI = 28
OFF_FRAME_D_RE = 32
OFF_FRAME_CR = 36
OFF_FRAME_ZI = 40
OFF_FRAME_ZR = 44
OFF_FRAME_DIV = 48
OFF_FRAME_RESTO = 52
OFF_GP = 56
OFF_FP = 60
OFF_RA = 64


.frame $fp, FRAME_SPACE, ra
subu sp, sp, FRAME_SPACE # Pongo el stack pointer al final de mi frame.
.cprestore OFF_GP		# El gp en 40(sp)
sw $fp, OFF_FP(sp)
sw ra, OFF_RA(sp)

#Pongo el frame pointer al final de mi frame.
move $fp, sp

sw a0, FRAME_SPACE($fp)

lw t6, OFF_OUTPUT_FILE(a0)
lh t6, OFF_FILE_NUMBER(t6) 				# T6 para el file descriptor.
sh t6, OFF_FRAME_FILE_NO($fp)

imprimir_p2:
	li v0, SYS_write
	move a0, t6
	la a1, p2
	li a2, LARGO_P2
	
	syscall	
	# Si me devolvió un número no nulo en a3, hubo error.
	bne a3, zero, imprimir_io_error
	li a2, LARGO_P2
  	blt v0, a2, imprimir_io_error
	

imprimir_res_x:
	lw a0, FRAME_SPACE($fp) # Recargo a0 con la estructura porque en el imprimir se pisa
	lw t8, OFF_RES_REAL(a0)
	jal imprimir_numero
	
imprimir_res_y:
	lw a0, FRAME_SPACE($fp)
	lw t8, OFF_RES_IMAG(a0)
	jal imprimir_numero
	
imprimir_max_gray:
	lw a0, FRAME_SPACE($fp)
	lw t8, OFF_SHADES(a0)
	jal imprimir_numero

empezar:
	lw a0, FRAME_SPACE($fp)
	li t0, 0 							# T0 es y
	lw t1, OFF_RES_IMAG(a0) 			# T1 res_y
	
	l.s $f0, OFF_UPPER_LEFT_IMAG(a0) 	# F0 para upper_left_im
	l.s $f2, OFF_SCALE_IMAG(a0) 		# F2 para d_im
	
	l.s $f6, OFF_UPPER_LEFT_REAL(a0)	# F6 para upper_left_re
	l.s $f8, OFF_SCALE_REAL(a0) 		# F8 para d_re

	mov.s $f4, $f0 						# F4 copia del up_im para ir modificando (ci)
	mov.s $f10, $f6 					# F10 copia del up_re (cr)

loop_vertical:
	beq t0, t1, terminar 			# Si y llegó a ser el último pixel, terminó.
	
	lw a0, FRAME_SPACE($fp)
	li t2, 0		 				# T2 para x
	lw t3, OFF_RES_REAL(a0)			# T3 para res_x

	loop_horizontal:
		beq t2, t3, aumento_loop_vert

		mov.s $f12, $f4 			# F12 guarda la copia del ci (zi)
		mov.s $f14, $f10 			# F14 guarda la copia del cr (zr)

		lw a0, FRAME_SPACE($fp)
		li t4, 0 					# T4 para la velocidad de escape
		lw t5, OFF_SHADES(a0)		# T5 para el máximo de grises

		loop_mandelbrot:
			beq t4, t5, imprimir_brillo

			# Calculo el módulo
			mul.s $f16, $f12, $f12 	# F16 guarda zi*zi
			mul.s $f18, $f14, $f14 	# F18 guarda zr*zr
			add.s $f20, $f16, $f18 	# F20 guarda zi*zi+zr*zr

			li.s $f22, 4.0 			# F22 guarda 4
			c.lt.s $f22, $f20 		# Si F22<F20 pone el flag en true.
			bc1t imprimir_brillo 	# Va a imprimir brillo si el flag es false, es decir, si el módulo es mayor a 4.

			mul.s $f16, $f12, $f12 	# F16 guarda zi*zi
			mul.s $f18, $f14, $f14 	# F18 guarda zr*zr
			mul.s $f20, $f12, $f16 	# F20 guarda zi*zi*zi
			mul.s $f22, $f14, $f18 	# F22 guarda zr*zr*zr
			mul.s $f24, $f16, $f14 	# F24 guarda zi*zi*zr
			mul.s $f26, $f18, $f12 	# F26 guarda zr*zr*zi
			li.s $f28, 3.0 			# F28 guarda 3
			mul.s $f24, $f28, $f24 	# F24 guarda 3*zi*zi*zr
			mul.s $f26, $f28, $f26 	# F26 guarda 3*zr*zr*zi
			sub.s $f22, $f22, $f24 	# F22 guarda zr*zr*zr - 3*zi*zi*zr
			add.s $f22, $f22, $f4 	# F22 guarda sr
			sub.s $f26, $f26, $f20 	# F26 guarda 3*zr*zr*zi - zi*zi*zi
			add.s $f26, $f26, $f10 	# F26 guarda si
			mov.s $f12, $f26
			mov.s $f14, $f22
			
			addi t4, t4, 1
			j loop_mandelbrot

		imprimir_brillo:

			guardar:	
				sw t0, OFF_FRAME_Y($fp)
				sw t1, OFF_FRAME_RES_Y($fp)
				sw t2, OFF_FRAME_X($fp)
				sw t3, OFF_FRAME_RES_X($fp)
				sw t4, OFF_FRAME_VEL($fp)
				
				s.s $f2, OFF_FRAME_D_IM($fp)
				s.s $f4, OFF_FRAME_CI($fp)
				s.s $f8, OFF_FRAME_D_RE($fp)
				s.s $f10, OFF_FRAME_CR($fp)
				s.s $f12, OFF_FRAME_ZI($fp)
				s.s $f14, OFF_FRAME_ZR($fp)						
				
				move t8, t4
				jal imprimir_numero
			
			recargar:
				lw t0, OFF_FRAME_Y($fp)
				lw t1, OFF_FRAME_RES_Y($fp)
				lw t2, OFF_FRAME_X($fp)
				lw t3, OFF_FRAME_RES_X($fp)
				lw t4, OFF_FRAME_VEL($fp)
				
				l.s $f2, OFF_FRAME_D_IM($fp)
				l.s $f4, OFF_FRAME_CI($fp)
				l.s $f8, OFF_FRAME_D_RE($fp)
				l.s $f10, OFF_FRAME_CR($fp)
				l.s $f12, OFF_FRAME_ZI($fp)
				l.s $f14, OFF_FRAME_ZR($fp)

	aumento_loop_hor:
		addi t2, t2, 1 				# Aumento x en 1.
		add.s $f10, $f10, $f8 		# Aumento cr según la escala.
		j loop_horizontal
aumento_loop_vert:
	addi t0, t0, 1 			# Aumento y en 1.
	add.s $f4, $f4, $f2 	# A ci le sumo según la escala.
	j loop_vertical


terminar:
	lw ra, OFF_RA($fp)
	lw gp, OFF_GP($fp)
	lw $fp, OFF_FP($fp)
	addu sp, sp, FRAME_SPACE
	jr ra

	
MAX_DIV = 100		# Es el máximo divisor de acuerdo a la cantidad máxima de cifras que puede tener el número a imprimir.
imprimir_numero:	# Recibe registros T8 (tiene el número de 3 cifras máximo, y lo cambia); usa registros T0, T1, T2, T3 y T4.
	li t0, MAX_DIV			# T0 para el divisor.
	imp_cifra:
		beq t0, 0, volver	# Si ya imprimió todo, vuelve al programa.
		divu t1, t8, t0		# T1 para el cociente de num/div
		rem t2, t8, t0		# T2 para el resto de num/div
	escribir_num:
		# El número a imprimir está en T1
		la t3, numeros		# T3 para la dirección del arreglo.
		# La posición en el arreglo es número por 4 (ya que cada palabra son 4 bytes).
		sll t4, t1, 2		# T4 tiene la poscición del número en el arreglo.
		addu t4, t3, t4		# T4 tiene la dirección donde está la dirección de la etiqueta del número.
		lw t4, 0(t4)		# T4 tiene dirección de la etiqueta del número.
		
		li v0, SYS_write
		lh a0, OFF_FRAME_FILE_NO($fp)
		move a1, t4
		li a2, LARGO_NUM
		
		sw t0, OFF_FRAME_DIV($fp)
		sw t2, OFF_FRAME_RESTO($fp)
		
		syscall
		# Si me devolvió un número no nulo en a3, hubo error.
		bne a3, zero, imprimir_io_error
		li a2, LARGO_NUM
		blt v0, a2, imprimir_io_error
		
		lw t0, OFF_FRAME_DIV($fp)
		lw t2, OFF_FRAME_RESTO($fp)
		
	proximo:
		move t8, t2
		divu t0, t0, 10		# Para testear la siguiente cifra.
		j imp_cifra
	
	volver:
    # Imprimo un enter.
    li v0, SYS_write
	lw a0, OFF_FRAME_FILE_NO($fp)
	la a1, enter
	li a2, LARGO_ENTER
	syscall
	# Si me devolvió un número no nulo en a3, hubo error.
	bne a3, zero, imprimir_io_error
    blt v0, a2, imprimir_io_error
    jr ra


STD_ERR = 2		
imprimir_io_error:
	li v0, SYS_write
	li a0, STD_ERR
	la a1, error_io
	li a2, LARGO_IO_ERROR
	syscall
	j exit

exit:
	li v0, SYS_exit
	syscall


.end mips32_plot


.rdata

p2: .asciiz "P2\n"

.align 2

numeros: .word cero, uno, dos, tres, cuatro, cinco, seis, siete, ocho, nueve
cero: .ascii "0"
uno: .ascii "1"
dos: .ascii "2"
tres: .ascii "3"
cuatro: .ascii "4"
cinco: .ascii "5"
seis: .ascii "6"
siete: .ascii "7"
ocho: .ascii "8"
nueve: .ascii "9"


enter: .asciiz "\n"

error_io: .asciiz "i/o error.\n"
