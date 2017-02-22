###############################################################################
# Makefile para GameLord
# Autor: Miguel Escribano Pérez (miguescri)
###############################################################################

###############################################################################
# definicion de variables y constantes
###############################################################################

# directorio fuentes
src     := src/
# directorio binarios
bin     := bin/
# directorio cabeceras
head    := include/
# path documentación del código
docu    := doxygen/
# nombre fichero comprimido
file    := GameLord

RM      := rm -rf
CC      := gcc
CCFLAGS := -Wall -I ${head}
#-lfl necesaria para linkar objetos generados a partir de flex
CCLINK  := -lfl
CPP     := g++
CPPFLAGS := -Wall -std=c++11
CPPLINK :=

# nombres de ejecutables
game    := GameLord

# fuente del main
main    := game_main
# fuente de pieza
piece   := pieza
# fuente de casilla
square  := casilla
# fuente de tablero
board   := tablero
# fuente de evento
event   := evento
# fuente de gfx
graphics := gfx_interface
# fuente de input
input   := input_interface
# fuente de parser
parser  := comment_eraser

# identificador de conjuntos de inclusión
square_o    := ${bin}${piece}.o ${bin}${square}.o
square_h    := ${head}${piece}.h ${head}${square}.h
board_o     := ${bin}${board}.o ${square_o}
board_h     := ${head}${board}.h ${square_h} ${head}${parser}.h
graphics_h  := ${head}${graphics}.h ${board_h}
event_h     := ${head}${event}.h ${board_h} ${graphics_h}

###############################################################################
# definicion de objetivos
###############################################################################

.PHONY:all
# objetivo por defecto
all: ${game}

# objetivo juego
${game}: ${bin}${main}.o ${board_o} ${bin}${event}.o ${bin}${input}.o ${bin}${graphics}.o ${bin}${parser}.o
	${CC} ${bin}${main}.o ${board_o} ${bin}${event}.o ${bin}${input}.o ${bin}${graphics}.o ${bin}${parser}.o -o ${game} ${CCLINK}

# objetivo main.o
${bin}${main}.o: ${src}${main}.c ${board_h} ${event_h} ${head}${input}.h ${graphics_h}
	${CC} -o ${bin}${main}.o -c ${CCFLAGS} ${src}${main}.c

# objetivo piece.o
${bin}${piece}.o: ${src}${piece}.c ${head}${piece}.h
	${CC} -o ${bin}${piece}.o -c ${CCFLAGS} ${src}${piece}.c

# objetivo square.o
${bin}${square}.o: ${src}${square}.c ${square_h}
	${CC} -o ${bin}${square}.o -c ${CCFLAGS} ${src}${square}.c

# objetivo board.o
${bin}${board}.o: ${src}${board}.c ${board_h}
	${CC} -o ${bin}${board}.o -c ${CCFLAGS} ${src}${board}.c

# objetivo event.o
${bin}${event}.o: ${src}${event}.c ${event_h}
	${CC} -o ${bin}${event}.o -c ${CCFLAGS} ${src}${event}.c

# objetivo input.o
${bin}${input}.o: ${src}${input}.c ${head}${input}.h
	${CC} -o ${bin}${input}.o -c ${CCFLAGS} ${src}${input}.c

# objetivo graphics.o
${bin}${graphics}.o: ${src}${graphics}.c ${graphics_h}
	${CC} -o ${bin}${graphics}.o -c ${CCFLAGS} ${src}${graphics}.c

# objetivo parser.o
${bin}${parser}.o: ${src}${parser}.c ${head}${parser}.h
	${CC} -o ${bin}${parser}.o -c ${CCFLAGS} ${src}${parser}.c

# objetivo para generar parser.c con flex
${src}${parser}.c:  ${src}${parser}.l
	flex -o ${src}${parser}.c ${src}${parser}.l

.PHONY:clean
# objetivo para eliminar objetos, programas y ficheros no indispensables
clean:
	$(RM) ${bin}*
	$(RM) ${game}
	$(RM) ${file}.zip
	$(RM) ${docu}

.PHONY:zip
# objetivo para comprimir los fuentes y el Makefile
zip:
	zip ${file}.zip -u Makefile ${src}* ${head}*


.PHONY:documents
# objetivo para generar documentación
documentacion: ${docu}
	mkdir ${docu}
	doxygen -g
	doxygen
