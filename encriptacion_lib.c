#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encriptacion.h"

static unsigned char rotate_left(unsigned char byte, int shift) {
    return (unsigned char)((byte << shift) | (byte >> (8 - shift)));
}

static unsigned char rotate_right(unsigned char byte, int shift) {
    return (unsigned char)((byte >> shift) | (byte << (8 - shift)));
}

void compute_encryption_params(const char *archivo, unsigned char *key, int *shift) {
    size_t len = strlen(archivo);
    if (len == 0) {
        *key = 0;
        *shift = 1;
        return;
    }

    *key = (unsigned char)archivo[0];
    int shift_val = (int)len;
    while (shift_val > 7) {
        shift_val -= 7;
    }
    if (shift_val <= 0) {
        shift_val = 1;
    }
    *shift = shift_val;
}

int decrypt_file_to_stream(const char *inputFile, FILE *outputFile, unsigned char key, int shift) {
    FILE *input = fopen(inputFile, "rb");
    if (input == NULL) {
        return -1;
    }

    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, input) == 1) {
        buffer = rotate_right(buffer ^ key, shift);
        if (fwrite(&buffer, sizeof(unsigned char), 1, outputFile) != 1) {
            fclose(input);
            return -2;
        }
    }

    fclose(input);
    return 0;
}

int encrypt_stream_to_file(FILE *inputFile, const char *outputFile, unsigned char key, int shift) {
    FILE *output = fopen(outputFile, "wb");
    if (output == NULL) {
        return -1;
    }

    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, inputFile) == 1) {
        buffer = rotate_left(buffer, shift) ^ key;
        if (fwrite(&buffer, sizeof(unsigned char), 1, output) != 1) {
            fclose(output);
            return -2;
        }
    }

    fclose(output);
    return 0;
}
