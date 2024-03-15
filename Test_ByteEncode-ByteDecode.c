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

/////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

//ByteEncoded
void ByteEncoded(const unsigned int *F, unsigned char *B) {
    unsigned int b[256 * d]; // matriz temporal para los bits
    for (int i = 0; i < 256; i++) {
        unsigned int a = F[i]; // a ∈ Z2d
        for (int j = 0; j < d; j++) {
            b[i * d + j] = a % 2; // b ∈ {0,1}256·d
            a = (a - b[i * d + j]) / 2; // note a - B[i * d + j] is always even
        }
    }
    BitsToBytes(b, 256 * d, B); // Llamada a la función BitsToBytes
}

///////////////////////////////////
///

/// ByteDecoded
void ByteDecoded(const unsigned char *B,unsigned int *F) {

    unsigned int B_len = (N * d + 7) / 8;
    unsigned int b[B_len * 8];
    BytesToBits(B, B_len, b); // Convertir bytes en bits
    // Decodificar los bits en enteros de d bits
    for (int i = 0; i < 256; i++) {
        unsigned int sum = 0;
        for (int j = 0; j < d; j++) {
            sum += b[i * d + j] << j; // Sumar los bits multiplicados por 2^j
        }
        F[i] = sum%q; //Se acaba de agregar
    }
}


int main() {
		//Main Encode - Decode
     //Definir un array de prueba F de 256 elementos
     unsigned int F[N];
     for (int i = 0; i < N; i++) {
         F[i] = i; // Asignar valores consecutivos del 0 al 255
     }

     // Calcular la longitud del array de bytes (256 * d / 8)
     int bytes_len = (N * d + 7) / 8; // para el redondeo

     // Definir un array para almacenar los bytes codificados
     unsigned char bytes[bytes_len];

     // Codificar bytes
     ByteEncoded(F, bytes);

     // Imprimir los bytes codificados
     printf("Bytes codificados:\n");
     for (int i = 0; i < bytes_len; i++) {
         printf("%u ", bytes[i]); // Para mostrarlos en Hexa 02X
     }
     printf("\n");

     // Definir un array para almacenar los enteros decodificados
     unsigned int decodedF[N];

     // Definir un array temporal para almacenar los bits

     // Decodificar bytes
     ByteDecoded(bytes, decodedF);

     // Imprimir los enteros decodificados
     printf("Enteros decodificados:\n");
     for (int i = 0; i < N; i++) {
         printf("%u ", decodedF[i]);
     }
     printf("\n");

     return 0;
}
