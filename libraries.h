// Andres Felipe Penagos Betancur - Daniel Ruiz Guirales


#include <stdio.h>
#include <stdlib.h>

//Variables Globales
const int n = 256;
const int q = 3329;

// ML-KEM-768 parameters
const int k  = 3;
const int nu  = 2;   //η1 = η2 = η = 2
const int du  = 10;
const int dv  = 4;
    
/*const int   encodingSize12 = n * 12 / 8;
const int   encodingSize10 = n * 10 / 8;
const int   encodingSize4  = n * 4 / 8;
const int   encodingSize1  = n * 1 / 8;

const int   messageSize       = encodingSize1;
const int   decryptionKeySize = k * encodingSize12;
const int   encryptionKeySize = k*encodingSize12 + 32;

const int   CiphertextSize       = k*encodingSize10 + encodingSize4;
const int   EncapsulationKeySize = encryptionKeySize;
const int   DecapsulationKeySize = decryptionKeySize + encryptionKeySize + 32 + 32;
const int   SharedKeySize        = 32;
const int   SeedSize             = 32 + 32;*/


// Function to convert a bit string into an array of bytes:

unsigned char* BitsToBytes(const unsigned char *b, int length) {
    // Calcular el numero de bytes requerido
    int byte_length = length / 8;
    
    // Allocate memory for the byte array
    unsigned char *B = (unsigned char*)malloc(byte_length * sizeof(unsigned char));  //e utiliza para determinar el tamaño de un solo byte en el sistema actual. La memoria asignada se reserva en el montón 
    if (B == NULL) {
        return NULL; //Si malloc devuelve un puntero nulo (NULL), indica que la asignación de memoria falló debido a la falta de memoria disponible. 
    }
    
    // Inicializando el arreglo en 0:
    
    for (int i = 0; i < byte_length; i++) {
        B[i] = 0;
    }
    
    // Convert bits to bytes
    for (int i = 0; i < length; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        B[byte_index] |= b[i] << bit_index;
    }

    return B;
}

//La funcion BitstoBytes es utilizada por ByteEncode 