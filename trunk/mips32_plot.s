#include <mips/regdef.h>
#include <sys/syscall.h>
.abicalls
.text
.align 2
.global mips32_plot
.entity mips32_plot
mips32_plot:
#define FRAME_SPACE ...
.frame $fp, FRAME_SPACE, ra
#.cpload t9
.cprestore FRAME_SPACE-Padding-$(del ra)-4(del fp)
sw $fp, FRAME_SPACE-Padding-4(del ra)
sw ra, FRAME_SPACE-Padding
#Pongo el stack pointer al final de mi frame.
subu sp, sp, FRAME_SPACE
#Pongo el frame pointer al final de mi frame.
add $fp, sp, zero
sw a0, FRAME_SPACE($fp)

#Offset dentro de la estructura param.
#define OFF_UPPER_LEFT_REAL 0		#float: 1 registro f
#define OFF_UPPER_LEFT_IMAG 4		#float: 1 registro f
#define OFF_LOWER_RIGHT_REAL 8		#float: 1 registro f
#define OFF_LOWER_RIGHT_IMAG 12		#float: 1 registro f
#define OFF_SCALE_REAL 16			#float: 1 registro f
#define OFF_SCALE_IMAG 20      		#float: 1 registro f
#define OFF_RES_REAL 24      		#size_t = unsigned: 1 registros
#define OFF_RES_IMAG 32      		#size_t = unsigned: 1 registros
#define OFF_SHADES 40      			#size_t = unsigned: 1 registros
#define OFF_OUTPUT_FILE 48      	#puntero: 1 registro
#Offset del numero de archivo dentro de la estructura _sFile
#define OFF_FILE_NUMBER 112      	#numero: 1 registro



li t0, 0							# T0 es y
lw t1, OFF_RES_IMAG(a0)				# T1 res_y
l.s f0, OFF_UPPER_LEFT_IMAG(a0)		# F0 para upper_left_im
l.s f1, OFF_SCALE_IMAG(a0)			# F1 para d_im
mov.s f2, f0						# F2 copia del up_im para ir modificando (ci)
li t2, 0							# T2 para x	
lw t3, OFF_RES_REAL(a0)				# T3 para res_x
lw f3, OFF_UPPER_LEFT_REAL(a0)		# F3 para upper_left_re
l.s f4, OFF_SCALE_REAL(a0)			# F4 para d_re
mov.s f5, f3						# F5 copia del up_re (cr)
li t4, 0							# T4 para la velocidad de escape.
lw t5, OFF_SHADES(a0)				# T5 para el máximo de grises
la t6, OFF_OUTPUT_FILE(a0)
lw t6, OFF_FILE_NUMBER(t0)			# T6 para el file descriptor.


loop_vertical:
	beq t0, t1, terminar		# Si y llegó a ser el último pixel, termino.
	loop_horizontal:
			beq t2, t3, aumento_loop_vert
			
			mov.s f6, f2			# F6 guarda la copia del ci (zi)
			mov.s f7, f5			# F7 guarda la copia del cr (zr)
			loop_mandelbrot:
					beq t4, t5, imprimir_brillo
					
					# Calculo el módulo
					mul.s f8, f6, f6		# F8 guarda zi*zi
					mul.s f9, f7, f7		# F9 guarda zr*zr
					add.s f10, f8, f9		# F10 guarda zi*zi+zr*zr
					li.s f11, 4.0			# F11 guarda 4
					c.lt.s f10, f11			# Si F10<F11 pone el flag en true.
					bclf imprimir_brillo	# Va a imprimir brillo si el flag es false, es decir, si el módulo es mayor a 4.
					
					mul.s f10, f8, f6		# F10 guarda zi*zi*zi
					mul.s f11, f9, f7		# F11 guarda zr*zr*zr
					mul.s f12, f8, f7		# F12 guarda zi*zi*zr
					mul.s f13, f9, f6		# F13 guarda zr*zr*zi
					li.s f14, 3.0			# F14 guarda 3
					mul.s f12, f14, f12		# F12 guarda 3*zi*zi*zr
					mul.s f13, f14, f13		# F13 guarda 3*zr*zr*zi
					sub.s f11, f11, f12		# F11 guarda zr*zr*zr - 3*zi*zi*zr
					add.s f11, f11, f5		# F11 guarda sr
					sub.s f13, f13, f10		# F13 guarda 3*zr*zr*zi - zi*zi*zi
					add.s f13, f13, f2		# F13 guarda si
					mov.s f6, f13
					mov.s f7, f11
					
					add t4, t4, 1			# Aumento la velocidad de escape en 1
					j loop_mandelbrot
					
			imprimir_brillo:
					### Imprimir número
					
					# Imprimo un enter.
					li v0, 15
					move a0, t6
					la a1, enter      # o dirección del String
					li a2, LARGO_ENTER      # o largo del String
					syscall
					# Si me devolvió un número negativo en v0, hubo error.
					blt v0, zero, imprimir_io_error
					
	aumento_loop_hor:
			add t2, t2, 1		# Aumento x en 1.
			add.s f5, f5, d_re	# Aumento cr según la escala.
			j loop_horizontal
aumento_loop_vert:
	add t0, t0, 1		# Aumento y en 1.
	sub.s f2, f2, f1		# A ci le resto según la escala.
	j loop_vertical


terminar:  #Se debe hacer el flush


imprimir_io_error:
li v0, 15
li a0, STD_ERR
la a1, error_io
li a2, LARGO_IO_ERROR
syscall
ba exit

imprimir_file_error:
li v0, 15
li a0, STD_ERR
la a1, error_file
li a2, LARGO_FILE_ERROR
syscall
ba exit

# Para imprimir Strings:
li v0, 15
  ###Supongo que en t0 está el file descriptor
or a0, t0, zero
la a1, p2      # o dirección del String
li a2, LARGO_P2      # o largo del String
syscall
# Si me devolvió un número negativo en v0, hubo error.
blt v0, zero, salida_error

exit:
li v0, SYS_exit
syscall


.rdata
#define LARGO_P2 3
p2: .asciiz "P2\n"
#define LARGO_FILE_ERROR 25
error_file: .asciiz "cannot flush output file.\n"
#define LARGO_IO_ERROR 11
error_io: .asciiz "i/o error.\n"
#define LARGO_ENTER
enter: .asciiz "\n"

