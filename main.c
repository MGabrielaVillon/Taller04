#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "usuario.h"
#include "log.h"

#define DEFAULT_USERS "usuarios.dat"
#define DEFAULT_LOG "transacciones.log"

//Joao Pincay
//Luis Villegas
//Maria Villon


int main(int argc, char *argv[]) {
    char *archivo_usuarios = DEFAULT_USERS;
    char *archivo_log = DEFAULT_LOG;
    char buffer_usuarios[256];
    char buffer_log[256];
    int flag_eliminar_log = 0;
    int opt;
    int ingreso_u = 0;
    int ingreso_a = 0;

    while ((opt = getopt(argc, argv, "u:a:n")) != -1) {
        switch (opt) {
            case 'u':
                archivo_usuarios = optarg;
                ingreso_u = 1;
                break;
            case 'a':
                archivo_log = optarg;
                ingreso_a = 1;
                break;
            case 'n':
                flag_eliminar_log = 1;
                break;
            default:
                fprintf(stderr, "Uso: %s [-u archivo_usuarios.dat] [-a archivo_auditoria.log] [-n]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (ingreso_u == 0) {
        buffer_usuarios[0] = '\0';
        while (strlen(buffer_usuarios) == 0) {
        printf("Ingrese el nombre del archivo de usuarios: ");
        
    if (scanf("%255s", buffer_usuarios) != 1) {
        limpiarBuffer();
        }
    }
        archivo_usuarios = buffer_usuarios;
    }

if (ingreso_a == 0) {
buffer_log[0] = '\0';
while (strlen(buffer_log) == 0) {
printf("Ingrese el nombre del archivo de log: ");
if (scanf("%255s", buffer_log) != 1) {
limpiarBuffer();
}
}
archivo_log = buffer_log;
}

    if (flag_eliminar_log) {
        eliminarLogAnterior(archivo_log);
    }

    cargarDatos(archivo_usuarios); 
    
    int opcion;
    do {
        printf("\n--- GESTION DE USUARIOS ---\n"
               "1. Crear \n"
               "2. Listar \n"
               "3. Modificar \n"
               "4. Bloquear \n"
               "5. Eliminar \n"
               "0. Salir \n"
               "Ingrese una opción: ");
               
        if (scanf("%d", &opcion) != 1) { 
            limpiarBuffer(); 
            continue; 
        }
        
        switch (opcion) {
            case 1: 
                crearUsuario(archivo_log); 
                break;
            case 2: 
                listarUsuarios(); 
                break;
            case 3:
                modificarUsuario(archivo_log);
                break;
            case 4: 
                bloquearDesbloquearUsuario(archivo_log);
                break;
            case 5:
                eliminarUsuario(archivo_log);
                break;
            case 0: 
                guardarYSalir(archivo_usuarios);
                break;
        }
    } while (opcion != 0);

    return 0;
}