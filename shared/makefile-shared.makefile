# Globales
CC = gcc

MODULOS = consola kernel cpu file_system memoria

COMANDO_FIND_BUILD = -type f -iname "*.c" -exec basename {} \;
SRC_CONSOLA = $(shell ./consola/src $(COMANDO_FIND_BUILD))
# SRC_KERNEL = $(shell ./kernel/src $(COMANDO_FIND_BUILD))
# SRC_CPU = $(shell ./cpu/src $(COMANDO_FIND_BUILD))
# SRC_FILE_SYSTEM = $(shell ./file_system/src $(COMANDO_FIND_BUILD))
# SRC_MEMORIA = $(shell ./memoria/src $(COMANDO_FIND_BUILD))

BINS = $(SRC:%.c=./obj/*.o)

build: makefolder $(BINS)
	$(foreach MODULO, $(MODULOS), gcc -o consola.out main.c

./bin/%.o: ./src/%.c
	gcc -c ./src/%.c -o ./bin/%.c
	
	
# Funciones	
makefolder:
	if [ ! -d ./bin ]; then mkdir bin; fi
