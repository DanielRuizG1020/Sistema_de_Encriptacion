
/*
/brief este es el primer programa pa probar el compilador y el sistema de desarrollo de git hub
/author Daniel Ruiz Guirales, Andres Felipe Pega Betancur
*/
//Librerias


//falta prototipo de la funcion

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


int main(void){
    printf("Hola Mundo");
    return 0;

}

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
        if (d1 < q) {
            a[j] = d1; // a es un array de 256 que cada elemento es maximo q 
            j++;
        }
        if (d2 < q && j < 256) {
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
void SamplePolyCBD(const unsigned char *B, unsigned int *f) {          ///CORREGIR
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
        f[i] = (x - y) % q;    //pendiente de revision : aritmetica modular
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


//Operaciones Modulares.
// fieldElement is an integer modulo q, an element of ℤ_q. It is always reduced.
typedef uint16_t fieldElement;

bool fieldCheckReduced(fieldElement a) {
    return a < q;
}

fieldElement fieldReduceOnce(uint16_t a) {
    int16_t x = a - q;
    // Si x se desborda hacia abajo, entonces x >= 2^16 - q > 2^15, por lo que se establece el bit superior.
    x += (x >> 15) * q;
    return x;
}

fieldElement fieldAdd(fieldElement a, fieldElement b) {
    uint32_t x = (uint32_t)a + b;
    return fieldReduceOnce(x);
}

fieldElement fieldSub(fieldElement a, fieldElement b) {
    uint32_t x = (uint32_t)a - b + q;
    return fieldReduceOnce(x);
}

const uint32_t barrettMultiplier = 5039; // 4^12 / q
const uint32_t barrettShift = 24;        // log₂(4^12)

fieldElement fieldReduce(uint32_t a) {
    uint32_t quotient = (uint64_t)a * barrettMultiplier >> barrettShift;
    return fieldReduceOnce(a - quotient * q);
}

fieldElement fieldMul(fieldElement a, fieldElement b) {
    uint32_t x = (uint32_t)a * b;
    return fieldReduce(x);
}

///

// zetas are the values ζ^BitRev7(k) mod q for each index k.
const uint16_t zetas[128]= {1, 1729, 2580, 3289, 2642, 630, 1897, 848, 1062, 1919, 193, 797, 2786, 3260, 569, 1746, 296, 2447, 1339, 1476, 3046, 56, 2240, 1333, 1426, 2094, 535, 2882, 2393, 2879, 1974, 821, 289, 331, 3253, 1756, 1197, 2304, 2277, 2055, 650, 1977, 2513, 632, 2865, 33, 1320, 1915, 2319, 1435, 807, 452, 1438, 2868, 1534, 2402, 2647, 2617, 1481, 648, 2474, 3110, 1227, 910, 17, 2761, 583, 2649, 1637, 723, 2288, 1100, 1409, 2662, 3281, 233, 756, 2156, 3015, 3050, 1703, 1651, 2789, 1789, 1847, 952, 1461, 2687, 939, 2308, 2437, 2388, 733, 2337, 268, 641, 1584, 2298, 2037, 3220, 375, 2549, 2090, 1645, 1063, 319, 2773, 757, 2099, 561, 2466, 2594, 2804, 1092, 403, 1026, 1143, 2150, 2775, 886, 1722, 1212, 1874, 1029, 2110, 2935, 885, 2154};



void NTT(const uint16_t *f, uint16_t *f_ntt) {
    uint16_t f_copy[n]; // Crear una copia del array de entrada
    
    // Inicializar la copia
    for (int i = 0; i < n; i++) {
        f_ntt[i] = f[i];
    }
    
    unsigned char k = 1;
    for (int len = 128; len >= 2; len /= 2) {
        for (int start = 0; start < n; start += 2 * len) {
            for (int j = start; j < start + len; j++) {
                uint16_t t = zetas[j]* f_ntt[j + len]; /// OJO VA CON OPERACIONES MODULARES PASO 8
                f_ntt[j + len] = (f_ntt[j] - t); //OJO OPERACIONES MODULARES Paso 9
                f_ntt[j] = (f_copy[j] + t); // OJO OPERACIONES MODULARES Paso 10
            }
        }
    }
}