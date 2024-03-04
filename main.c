
/*
/brief este es el primer programa pa probar el compilador y el sistema de desarrollo de git hub
/author Daniel Ruiz Guirales, Andres Felipe Pega Betancur
*/
//Librerias


//falta prototipo de la funcion

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int main(void){
    printf("Hola Mundo");
    return 0;

}




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
const int barrettMultiplier = 5039;
const int barrettShift  = 24 ;


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

/*unsigned char* BitsToBytes(const unsigned char *b, int length) {
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
}*/

//La funcion BitstoBytes es utilizada por ByteEncode 

///

/*uint8_t* polyByteEncode(uint8_t* b, uint16_t* f) {
    uint8_t* out = b;
    for (size_t i = 0; i < n; i += 2) {
        uint32_t x = ((uint32_t)f[i]) | ((uint32_t)f[i+1] << 12);
        *b++ = (uint8_t)x;
        *b++ = (uint8_t)(x >> 8);
        *b++ = (uint8_t)(x >> 16);
    }
    return out;
}


uint16_t compress(uint16_t x, uint8_t d) {
    // Queremos calcular (x * 2ᵈ) / q, redondeado al entero más cercano, con redondeo hacia arriba.
    // La reducción de Barrett produce un cociente y un resto en el rango [0, 2q),
    // de modo que el dividendo = cociente * q + resto.
    uint32_t dividend = (uint32_t)x << d; // x * 2ᵈ
    uint32_t quotient = ((uint64_t)dividend * barrettMultiplier) >> barrettShift;
    uint32_t remainder = dividend - quotient * q;

    // Dado que el resto está en el rango [0, 2q), no [0, q), necesitamos
    // dividirlo en tres rangos para redondear.
    //
    //     [ 0,       q/2     ) -> redondear a 0
    //     [ q/2,     q + q/2 ) -> redondear a 1
    //     [ q + q/2, 2q      ) -> redondear a 2
    //
    // Podemos convertir eso en la siguiente lógica: agregar 1 si el resto > q/2,
    // luego agregar 1 nuevamente si el resto > q + q/2.
    //
    // Tenga en cuenta que si el resto > x, entonces ⌊x⌋ - resto entra en desbordamiento, y el bit superior
    // de la diferencia se establecerá.
    quotient += (q / 2 - remainder) >> 31 & 1;
    quotient += (q + q / 2 - remainder) >> 31 & 1;

    // El cociente podría haberse desbordado en este punto, así que lo reducimos mediante máscara.
    uint32_t mask = (1 << d) - 1;
    return (uint16_t)(quotient & mask);
}*/


void sampleNTT(const unsigned char *B, unsigned int *a) {  // tener en cuenta que B y a son arrays 
    int i = 0;
    int j = 0;
    while (j < 256) {
        unsigned int d1 = B[i] + 256 * (B[i + 1] % 16);
        unsigned int d2 = (B[i + 1] / 16) + 16 * B[i + 2];
        if (d1 < 3329) {
            a[j] = d1; // a es un array de 256 que cada elemento es maximo q 
            j++;
        }
        if (d2 < 3329 && j < 256) {
            a[j] = d2;
            j++;
        }
        i += 3;
    }
}

void BytesToBits(const unsigned char *B, int B_len, unsigned int *b) {
    for (int i = 0; i < B_len; i++) {
        unsigned char temp = B[i];
        for (int j = 0; j < 8; j++) {
            b[8 * i + j] = temp % 2;
            temp /= 2;
        }
    }
}
void SamplePolyCBD(const unsigned char *B, unsigned int *f) {
    unsigned int b[256]; // Array de bits convertidos de los bytes de entrada

    // Convertir los bytes de entrada a bits
    BytesToBits(B, 64 * nu, b);

    // Para cada coeficiente del polinomio
    for (int i = 0; i < 256; i++) {
        // Calcular la suma de bits para la primera mitad de la muestra
        unsigned int x = 0;
        for (int j = 0; j < nu; j++) {
            x += b[2 * i * nu + j];
        }

        // Calcular la suma de bits para la segunda mitad de la muestra
        unsigned int y = 0;
        for (int j = 0; j < nu; j++) {
            y += b[2 * i * nu + nu + j];
        }

        // Calcular el coeficiente del polinomio
        f[i] = (x - y) % q;
    }
}


void BitsToBytes(const unsigned int *b, int b_len, unsigned char *B) {
    // Inicializar el array de bytes con ceros
    for (int i = 0; i < b_len / 8; i++) {
        B[i] = 0;
    }

    for (int i = 0; i < b_len; i++) {
        B[i / 8] += b[i] << (i % 8);
    }
}


void ByteEncoded(const unsigned int *F, int d, unsigned char *B) {
    unsigned int b[256 * 12]; // matriz temporal para los bits
    for (int i = 0; i < 256; i++) {
        unsigned int a = F[i]; // a ∈ Z2d
        for (int j = 0; j < d; j++) {
            b[i * d + j] = a % 2; // b ∈ {0,1}256·d
            a = (a - b[i * d + j]) / 2; // note a - B[i * d + j] is always even
        }
    }
    BitsToBytes(b, 256 * d, B); // Llamada a la función BitsToBytes
}

void ByteDecoded(const unsigned char *B, int B_len, int d, unsigned int *F) {
    unsigned int b[32 * d]; // Definir un array temporal para almacenar los bits
    BytesToBits(B, B_len, b); // Convertir bytes en bits

    // Decodificar los bits en enteros de d bits
    for (int i = 0; i < n; i++) {
        unsigned int sum = 0;
        for (int j = 0; j < d; j++) {
            sum += b[i * d + j] << j; // Sumar los bits multiplicados por 2^j
        }
        F[i] = sum;
    }
}