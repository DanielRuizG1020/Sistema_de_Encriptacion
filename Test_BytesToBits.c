#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define q 3329
#define nu 2
#define N 256
#define d 12

//BytesToBits
void BytesToBits(const unsigned char *B, int B_len, unsigned int *b) {
    for (int i = 0; i < B_len; i++) {
        unsigned char temp = B[i];
        for (int j = 0; j < 8; j++) {
            b[8 * i + j] = temp % 2;
            temp /= 2;
        }
    }
}


int main() {

unsigned char bytes[] = {229, 166}; // Ejemplo de bytes de entrada
 int bytes_len = sizeof(bytes) / sizeof(bytes[0]);

 // Tamaño del array de bits = tamaño de bytes * 8
 unsigned int bits[bytes_len * 8];

 // Llamada a la función BytesToBits
 BytesToBits(bytes, bytes_len, bits);

 // Imprimir los bits convertidos
 for (int i = 0; i < bytes_len * 8; i++) {
     printf("%u", bits[i]);
 }
 printf("\n");

 return 0;
}

