#include <stdio.h>
#include <time.h>
#include <string.h>
#include "log.h"

void verificarYRotarLog(const char *archivo_log) {
    FILE *f_lectura = fopen(archivo_log, "r");
    if (f_lectura == NULL) return; 

    int tamano = 0;
    char c;
    while ((c = fgetc(f_lectura)) != EOF) {
        tamano++;
    }
    fclose(f_lectura);

    if (tamano >= 1024) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        char nombre_historico[256];
        sprintf(nombre_historico, "%s_%04d-%02d-%02d_%02d-%02d-%02d", 
                archivo_log, 
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec);

        FILE *origen = fopen(archivo_log, "r");
        FILE *destino = fopen(nombre_historico, "w");

        if (origen != NULL && destino != NULL) {
            char ch;
            while ((ch = fgetc(origen)) != EOF) {
                fputc(ch, destino);
            }
        }
        if (origen != NULL) fclose(origen);
        if (destino != NULL) fclose(destino);

        FILE *limpiar = fopen(archivo_log, "w");
        if (limpiar != NULL) fclose(limpiar);
    }
}

void registrarLog(const char *archivo_log, const char* admin, const char* accion, const char* afectado) {
    verificarYRotarLog(archivo_log);

    FILE *log = fopen(archivo_log, "a");
    if (log == NULL) return;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(log, "%04d-%02d-%02d %02d:%02d:%02d | %s | %s | %s\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec, admin, accion, afectado);
    fclose(log);
}

void eliminarLogAnterior(const char *archivo_log) {
    FILE *log = fopen(archivo_log, "w");
    if (log != NULL) fclose(log);
}