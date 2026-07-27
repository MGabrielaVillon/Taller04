# Definición de variables de compilación
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

STATIC_LIB = libencriptacion.a
SHARED_LIB = libencriptacion.so

STATIC_TARGET = programa_static
SHARED_TARGET = programa_shared

# Listado de archivos objeto intermedios
OBJ_MAIN = main.o log.o usuario_enc.o
OBJ_LIB = encriptacion_lib.o

# Regla raíz (Compilación total)
all: $(STATIC_TARGET) $(SHARED_TARGET)

$(STATIC_LIB): $(OBJ_LIB)
	ar rcs $(STATIC_LIB) $(OBJ_LIB)

$(SHARED_LIB): $(OBJ_LIB)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ_LIB)

$(STATIC_TARGET): $(OBJ_MAIN) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $(STATIC_TARGET) $(OBJ_MAIN) $(STATIC_LIB)

$(SHARED_TARGET): $(OBJ_MAIN) $(SHARED_LIB)
	$(CC) $(CFLAGS) -o $(SHARED_TARGET) $(OBJ_MAIN) -L. -lencriptacion -Wl,-rpath=.

# Compilación de módulos independientes
main.o: main.c usuario.h log.h encriptacion.h
	$(CC) $(CFLAGS) -c main.c

usuario_enc.o: usuario_enc.c usuario.h log.h encriptacion.h
	$(CC) $(CFLAGS) -c usuario_enc.c

log.o: log.c log.h
	$(CC) $(CFLAGS) -c log.c

encriptacion_lib.o: encriptacion_lib.c encriptacion.h
	$(CC) $(CFLAGS) -fPIC -c encriptacion_lib.c

# Comando para limpiar el espacio de trabajo
clean:
	rm -f *.o $(STATIC_LIB) $(SHARED_LIB) $(STATIC_TARGET) $(SHARED_TARGET)
