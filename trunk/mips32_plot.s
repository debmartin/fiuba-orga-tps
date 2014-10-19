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

#t0 para el Upper Left Real y lo guardo en el primer lugar de la LTA.
lw f0, OFF_UPPER_LEFT_REAL(a0)
sw f0, ... #Principio de la LTA
#ídem la parte imaginaria, que guardo abajo
lw f0, OFF_UPPER_LEFT_IMAG(a0)
sw f0, ...
# Lower Right real
lw f0, OFF_LOWER_RIGHT_REAL(a0)
sw f0, ...
# Lower Right imag
lw f0, OFF_LOWER_RIGHT_IMAG(a0)
sw f0, ...
# Scale real
lw f0, OFF_SCALE_REAL(a0)
sw f0, ...
# Scale imag
lw f0, OFF_SCALE_IMAG(a0)
sw f0, ...
# Resolution real
lw t0, OFF_RES_REAL(a0)
sw t0, ...
# Resolution imag
lw t0, OFF_RES_IMAG(a0)
sw t0, ...
# Shades
lw t0, OFF_SHADES(a0)
sw t0, ...
#Output file number
la t0, OFF_OUTPUT_FILE(a0)
lw t0, OFF_FILE_NUMBER(t0)
sw t0, ...


li t0, 0		# T0 es y
lw t1, ...		# T1 res_y
lw t2, ...		# T2 para upper_left_im
lw t3, ...		# T3 para d_im
or t4, t2, 0	# T4 copia del up_im para ir modificando (ci)
li t5, 0		# T5 para x	
lw t6, ...		# T6 para res_x
lw t7, ...		# T7 para upper_left_re
lw t8, ...		# T8 para d_re
or t9, t7, 0	# T9 copia del up_re (cr)

loop_vertical:
	beq t0, t1, terminar		# Si y llegó a ser el último pixel, termino.
	sw t0, ...		# Guardo el y en la LTA porque necesito más registros.
	sw t7, ...		# Guardo el ci en la LTA para usar el registro.
	loop_horizontal:
			beq t5, t6, aumento_loop_vert
			sw t5, ...		# Guardo x en la LTA para poder usar el registro.
			sw t9, ...		# Guardo cr en la LTA para usar el registro.
			loop_mandelbrot:
				...
	aumento_loop_hor:
			add t5, t5, 1		# Aumento x en 1.
			add t9, t9, t8		# Aumento cr según la escala.
			j loop_horizontal
aumento_loop_vert:
	add t0, t0, 1		# Aumento y en 1.
	subu t4, t4, t3		# A la parte imaginaria que teníamos (ci) le resto según la escala.
	j loop_vertical

terminar:		#Se debe hacer el flush




# Para imprimir error:
li v0, 15
li a0, STD_ERR
la a1, error_file
li a2, LARGO_ERROR
syscall
ba salida_error

# Para imprimir Strings:
li v0, 15
  ###Supongo que en t0 está el file descriptor
add a0, t0, zero
la a1, p2      # o dirección del String
li a2, LARGO_P2      # o largo del String
syscall
# Si me devolvió un número negativo en v0, hubo error.
blt v0, zero, salida_error

salida_error:
li v0, SYS_exit
syscall


.rdata
#define LARGO_P2 3
p2: .asciiz "P2\n"
#define LARGO_ERROR 25
error_file: .asciiz "cannot flush output file.\n"
enter: .asciiz "\n"

