#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define q 3329
#define nu 2
#define N 256
#define d 12

// Función para convertir bits en bytes
//BitsToBytes
void BitsToBytes(const unsigned int *b, int b_len, unsigned char *B) {
    // Inicializar el array de bytes con ceros
    for (int i = 0; i < b_len / 8; i++) {
        B[i] = 0;
    }

    for (int i = 0; i < b_len; i++) {
        B[i / 8] += b[i] << (i % 8);
    }
}


int main() {
 ///////////////////////////////////////////////////////////////////////////////////
    
    
    unsigned int bits[] = {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1};

    // Calcular la longitud del array de bits
    int bits_len = sizeof(bits) / sizeof(bits[0]);

    // Calcular la longitud del array de bytes (8 bits por byte)
    int bytes_len = (bits_len + 7) / 8;

    // Definir un array para almacenar los bytes convertidos
    unsigned char bytes[bytes_len];

    // Convertir bits en bytes
    BitsToBytes(bits, bits_len, bytes);

    // Imprimir los bytes convertidos
    printf("Bytes convertidos:\n");
    for (int i = 0; i < bytes_len; i++) {
        printf("%d ", bytes[i]);
    }
    printf("\n");

    return 0;
}
