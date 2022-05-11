CFLAGS=-ggdb -O0 -std=c99 -Wall -Wconversion -Wtype-limits  -Werror -I src
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2

valgrind-interactivo: interactivo
	valgrind ./interactivo

interactivo: *.c *.h
	$(CC) $(CFLAGS) abb.c mostrar_abb.c interactivo.c -o interactivo 2>&1
