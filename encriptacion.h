#ifndef ENCRIPTACION_H
#define ENCRIPTACION_H

#include <stdio.h>

void compute_encryption_params(const char *archivo, unsigned char *key, int *shift);
int decrypt_file_to_stream(const char *inputFile, FILE *outputFile, unsigned char key, int shift);
int encrypt_stream_to_file(FILE *inputFile, const char *outputFile, unsigned char key, int shift);

#endif