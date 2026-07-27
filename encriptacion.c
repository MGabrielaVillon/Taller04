#include <stdio.h>
#include <stdlib.h>

// Función para realizar un desplazamiento circular a la izquierda
unsigned char rotate_left(unsigned char byte, int shift) {
    return (byte << shift) | (byte >> (8 - shift));
}

// Función para realizar un desplazamiento circular a la derecha
unsigned char rotate_right(unsigned char byte, int shift) {
    return (byte >> shift) | (byte << (8 - shift));
}

// Función para encriptar/desencriptar un archivo
void encrypt_decrypt(const char *inputFile, const char *outputFile, unsigned char key, int shift, int mode) {
    FILE *input = fopen(inputFile, "rb");
    FILE *output = fopen(outputFile, "wb");

    if (input == NULL || output == NULL) {
        printf("Error al abrir archivos.\n");
        exit(1);
    }

    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, input)) {
        if (mode == 1) {
            // Encriptar: Desplazamiento circular a la izquierda y XOR
            buffer = rotate_left(buffer, shift) ^ key;
        } else {
            // Desencriptar: XOR y desplazamiento circular a la derecha
            buffer = rotate_right(buffer ^ key, shift);
        }
        fwrite(&buffer, sizeof(unsigned char), 1, output);
    }

    fclose(input);
    fclose(output);
}

int main() {
    int option, shift;
    char inputFile[100], outputFile[100];
    unsigned char key;

    do {
        printf("=== Encriptación/Desencriptación de Archivos ===\n");
        printf("1. Encriptar un archivo\n");
        printf("2. Desencriptar un archivo\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Ingrese el nombre del archivo a encriptar: ");
                scanf("%s", inputFile);
                printf("Ingrese el nombre del archivo de salida: ");
                scanf("%s", outputFile);
                printf("Ingrese la clave (byte único): ");
                scanf(" %hhu", &key);
                printf("Ingrese el número de bits para el desplazamiento (1-7): ");
                scanf("%d", &shift);
                encrypt_decrypt(inputFile, outputFile, key, shift, 1);
                printf("Archivo encriptado con éxito.\n");
                break;

            case 2:
                printf("Ingrese el nombre del archivo a desencriptar: ");
                scanf("%s", inputFile);
                printf("Ingrese el nombre del archivo de salida: ");
                scanf("%s", outputFile);
                printf("Ingrese la clave (byte único): ");
                scanf(" %hhu", &key);
                printf("Ingrese el número de bits para el desplazamiento (1-7): ");
                scanf("%d", &shift);
                encrypt_decrypt(inputFile, outputFile, key, shift, 0);
                printf("Archivo desencriptado con éxito.\n");
                break;

            case 3:
                printf("Saliendo del programa.\n");
                break;

            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    } while (option != 3);

    return 0;
}
