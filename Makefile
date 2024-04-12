CC=gcc
CFLAGS=-Wall

.PHONY: all clean

all: programaPrincipal rendimientoCPU rendimientoMemoria rendimientoDisco

programaPrincipal: programaPrincipal.c
	$(CC) $(CFLAGS) -o programaPrincipal programaPrincipal.c

rendimientoCPU: rendimientoCPU.c
	$(CC) $(CFLAGS) -o rendimientoCPU rendimientoCPU.c

rendimientoMemoria: rendimientoMemoria.c
	$(CC) $(CFLAGS) -o rendimientoMemoria rendimientoMemoria.c

rendimientoDisco: rendimientoDisco.c
	$(CC) $(CFLAGS) -o rendimientoDisco rendimientoDisco.c

install: all
	cp programaPrincipal .
	cp rendimientoCPU .
	cp rendimientoMemoria .
	cp rendimientoDisco .

clean:
	rm -f programaPrincipal rendimientoCPU rendimientoMemoria rendimientoDisco