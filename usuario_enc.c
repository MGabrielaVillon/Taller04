#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "usuario.h"
#include "log.h"
#include "encriptacion.h"

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
    FILE *arch = fopen(archivo_usuarios, "rb");
    if (arch == NULL) {
        return;
    }
    fclose(arch);

    unsigned char key;
    int shift;
    compute_encryption_params(archivo_usuarios, &key, &shift);

    FILE *temp = tmpfile();
    if (temp == NULL) {
        return;
    }

    if (decrypt_file_to_stream(archivo_usuarios, temp, key, shift) != 0) {
        fclose(temp);
        return;
    }

    rewind(temp);
    Usuario tempUsuario;
    while (fscanf(temp, "%d, %6[^,], %15[^,], %hhu, %6[^,], %8[^,], %hhu\n",
                  &tempUsuario.id, tempUsuario.usuario, tempUsuario.nombre_completo, &tempUsuario.edad,
                  tempUsuario.grupo, tempUsuario.contrasena, &tempUsuario.estado) == 7) {
        if (!usuarioDuplicado(tempUsuario.usuario)) {
            Usuario *aux = realloc(listaUsuarios, (totalUsuarios + 1) * sizeof(Usuario));
            if (aux != NULL) {
                listaUsuarios = aux;
                listaUsuarios[totalUsuarios] = tempUsuario;
                if (listaUsuarios[totalUsuarios].id >= proximoID) {
                    proximoID = listaUsuarios[totalUsuarios].id + 1;
                }
                totalUsuarios++;
            }
        }
    }

    fclose(temp);
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
    char tempName[] = "/tmp/usuarios_temp_XXXXXX";
    int tempFd = mkstemp(tempName);
    if (tempFd == -1) {
        return;
    }

    FILE *temp = fdopen(tempFd, "wb");
    if (temp == NULL) {
        close(tempFd);
        remove(tempName);
        return;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(temp, "%d, %s, %s, %u, %s, %s, %u\n",
                listaUsuarios[i].id, listaUsuarios[i].usuario,
                listaUsuarios[i].nombre_completo, listaUsuarios[i].edad,
                listaUsuarios[i].grupo, listaUsuarios[i].contrasena,
                listaUsuarios[i].estado);
    }
    fclose(temp);

    unsigned char key;
    int shift;
    compute_encryption_params(archivo_usuarios, &key, &shift);

    FILE *source = fopen(tempName, "rb");
    if (source != NULL) {
        encrypt_stream_to_file(source, archivo_usuarios, key, shift);
        fclose(source);
    }
    remove(tempName);

    if (listaUsuarios != NULL) {
        free(listaUsuarios);
        listaUsuarios = NULL;
        totalUsuarios = 0;
    }
}
