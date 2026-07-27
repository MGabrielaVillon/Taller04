# Definición de variables de compilación
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = programa

# Listado de archivos objeto intermedios
OBJ = main.o usuario.o log.o

# Regla raíz (Compilación total)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compilación de módulos independientes
main.o: main.c usuario.h log.h
	$(CC) $(CFLAGS) -c main.c

usuario.o: usuario.c usuario.h log.h
	$(CC) $(CFLAGS) -c usuario.c

log.o: log.c log.h
	$(CC) $(CFLAGS) -c log.c

# Comando para limpiar el espacio de trabajo
clean:
	rm -f *.o $(TARGET)