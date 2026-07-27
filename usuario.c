#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "log.h"

Usuario *listaUsuarios = NULL;
int totalUsuarios = 0;
int proximoID = 1;

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int usuarioDuplicado(const char *nombre_usuario) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(listaUsuarios[i].usuario, nombre_usuario) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void cargarDatos(const char *archivo_usuarios) {
    FILE *arch = fopen(archivo_usuarios, "r");
    if (arch == NULL) {
        return; 
    }

    Usuario temp;
    while (fscanf(arch, "%d, %[^,], %[^,], %hhu, %[^,], %[^,], %hhu\n", 
           &temp.id, temp.usuario, temp.nombre_completo, &temp.edad,
           temp.grupo, temp.contrasena, &temp.estado) == 7) {
        
        if (!usuarioDuplicado(temp.usuario)) {
            Usuario *aux = realloc(listaUsuarios, (totalUsuarios + 1) * sizeof(Usuario));
            if (aux != NULL) {
                listaUsuarios = aux;
                listaUsuarios[totalUsuarios] = temp;
                if (listaUsuarios[totalUsuarios].id >= proximoID) {
                    proximoID = listaUsuarios[totalUsuarios].id + 1;
                }
                totalUsuarios++;
            }
        }
    }
    fclose(arch);
}

void crearUsuario(const char *archivo_log) {
    Usuario nuevo;
    printf("Ingrese nombre de usuario (max 6 chars): ");
    scanf("%6s", nuevo.usuario);
    limpiarBuffer();

    if (usuarioDuplicado(nuevo.usuario)) {
        printf("Error: El usuario ya existe.\n");
        return;
    }

    nuevo.id = proximoID++;
    nuevo.estado = 1;

    printf("Ingrese nombre completo (max 15 chars): ");
    fgets(nuevo.nombre_completo, 16, stdin);
    nuevo.nombre_completo[strcspn(nuevo.nombre_completo, "\n")] = 0;

    int edadTemp;
    printf("Ingrese edad: ");
    scanf("%d", &edadTemp);
    nuevo.edad = (unsigned char)edadTemp;

    printf("Ingrese grupo (max 6 chars): ");
    scanf("%6s", nuevo.grupo);
    printf("Ingrese contrasena (max 8 chars): ");
    scanf("%8s", nuevo.contrasena);
    limpiarBuffer();

    Usuario *aux = realloc(listaUsuarios, (totalUsuarios + 1) * sizeof(Usuario));
    if (aux != NULL) {
        listaUsuarios = aux;
        listaUsuarios[totalUsuarios] = nuevo;
        totalUsuarios++;
        registrarLog(archivo_log, "admin", "CREAR", nuevo.usuario);
        printf("Usuario creado exitosamente.\n");
    }
}

void listarUsuarios() {
    if (totalUsuarios == 0) {
        printf("\nLa lista esta vacia.\n");
        return;
    }
    printf("\nID  | Usuario | Nombre Completo | Edad | Grupo  | Estado\n");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < totalUsuarios; i++) {
        printf("%-3d | %-7s | %-15s | %-4d | %-6s | %s\n", 
               listaUsuarios[i].id, listaUsuarios[i].usuario,
               listaUsuarios[i].nombre_completo, listaUsuarios[i].edad,
               listaUsuarios[i].grupo, listaUsuarios[i].estado ? "Activo" : "Bloqueado");
    }
}

void modificarUsuario(const char *archivo_log) {
    char criterio[16];
    printf("Ingrese ID o usuario a modificar: ");
    scanf("%15s", criterio);
    limpiarBuffer();

    for (int i = 0; i < totalUsuarios; i++) {
        char idStr[10];
        sprintf(idStr, "%d", listaUsuarios[i].id);
        if (strcmp(listaUsuarios[i].usuario, criterio) == 0 || strcmp(idStr, criterio) == 0) {
            printf("Nuevo nombre completo (max 15 chars): ");
            fgets(listaUsuarios[i].nombre_completo, 16, stdin);
            listaUsuarios[i].nombre_completo[strcspn(listaUsuarios[i].nombre_completo, "\n")] = 0;
            registrarLog(archivo_log, "admin", "MODIFICAR", listaUsuarios[i].usuario);
            printf("Usuario modificado.\n");
            return;
        }
    }
    printf("No encontrado.\n");
}

void bloquearDesbloquearUsuario(const char *archivo_log) {
    char criterio[16];
    printf("Ingrese ID o usuario: ");
    scanf("%15s", criterio);
    limpiarBuffer();

    for (int i = 0; i < totalUsuarios; i++) {
        char idStr[10];
        sprintf(idStr, "%d", listaUsuarios[i].id);
        if (strcmp(listaUsuarios[i].usuario, criterio) == 0 || strcmp(idStr, criterio) == 0) {
            listaUsuarios[i].estado = !listaUsuarios[i].estado;
            registrarLog(archivo_log, "admin", listaUsuarios[i].estado ? "DESBLOQUEAR" : "BLOQUEAR", listaUsuarios[i].usuario);
            printf("Estado del usuario actualizado.\n");
            return;
        }
    }
    printf("ID/Usuario no encontrado.\n");
}

void eliminarUsuario(const char *archivo_log) {
    int idBusqueda;
    printf("Ingrese ID a eliminar: ");
    if (scanf("%d", &idBusqueda) != 1) {
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    for (int i = 0; i < totalUsuarios; i++) {
        if (listaUsuarios[i].id == idBusqueda) {
            char usuarioAfectado[16];
            strcpy(usuarioAfectado, listaUsuarios[i].usuario);
            for (int j = i; j < totalUsuarios - 1; j++){
                listaUsuarios[j] = listaUsuarios[j + 1];
            } 
            totalUsuarios--;

            if (totalUsuarios > 0) {
                listaUsuarios = realloc(listaUsuarios, totalUsuarios * sizeof(Usuario));
            } else {
                free(listaUsuarios);
                listaUsuarios = NULL;
            }

            registrarLog(archivo_log, "admin", "ELIMINAR", usuarioAfectado);
            printf("Usuario eliminado.\n");
            return;
        }
    }
    printf("ID no encontrado.\n");
}

void guardarYSalir(const char *archivo_usuarios) {
    FILE *arch = fopen(archivo_usuarios, "w");
    if (arch != NULL) {
        for (int i = 0; i < totalUsuarios; i++) {
            fprintf(arch, "%d, %s, %s, %u, %s, %s, %u\n",
                    listaUsuarios[i].id, listaUsuarios[i].usuario,
                    listaUsuarios[i].nombre_completo, listaUsuarios[i].edad,
                    listaUsuarios[i].grupo, listaUsuarios[i].contrasena, 
                    listaUsuarios[i].estado);
        }
        fclose(arch);
    }
    if (listaUsuarios != NULL) {
        free(listaUsuarios);
    }
}