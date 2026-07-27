#ifndef USUARIO_H
#define USUARIO_H

//Usamos tamaños especificos para cada campo y guardamos solo el espacio necesario.
//Cada cadena incluye espacio adicional para el terminador "\0"

typedef struct {
    int id;
    char nombre_completo[16];
    char contrasena[9];
    char usuario[7];
    char grupo[7];
    unsigned char edad;
    unsigned char estado;
} Usuario;

//Lista de usuarios dinamica: se asgina con malloc en tiempo de ejecucion
//Antes esto habria sido un array estatico fijo

extern Usuario *listaUsuarios;
extern int totalUsuarios;
extern int proximoID;

void cargarDatos(const char *archivo_usuarios);
void guardarYSalir(const char *archivo_usuarios);
int usuarioDuplicado(const char *nombre_usuario);
void crearUsuario(const char *archivo_log);
void listarUsuarios();
void modificarUsuario(const char *archivo_log);
void bloquearDesbloquearUsuario(const char *archivo_log);
void eliminarUsuario(const char *archivo_log);
void limpiarBuffer();

#endif