#ifndef LOG_H
#define LOG_H

void registrarLog(const char *archivo_log, const char* admin, const char* accion, const char* afectado);
void eliminarLogAnterior(const char *archivo_log);

#endif