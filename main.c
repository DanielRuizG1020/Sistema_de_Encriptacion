
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


//SAMPLENTT 
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

///////////////////////////////////////////////

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

////////////////////////////////////////////

//SamplePolyCBD ----- Pendiente de correción con operacion modular
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

//////////////////////////////////////////


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
///////////////////////////

//ByteEncoded
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

///////////////////////////////////


//ByteDecoded
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
/////////////////////////////

//Operaciones Modulares con el type
// fieldElement is an integer modulo q, an element of ℤ_q. It is always reduced.
/*typedef uint16_t fieldElement;

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
}*/

////////////////////////////////////////////////////////

//Operaciones Modulares
bool fieldCheckReduced(uint16_t a) {
    return a < q;
}

uint16_t fieldReduceOnce(uint16_t a) {
    int16_t x = a - q;
    // Si x se desborda hacia abajo, entonces x >= 2^16 - q > 2^15, por lo que se establece el bit superior.
    x += (x >> 15) * q;
    return x;
}

uint16_t fieldAdd(uint16_t a, uint16_t b) {
    uint32_t x = (uint32_t)a + b;
    return fieldReduceOnce(x);
}

uint16_t fieldSub(uint16_t a, uint16_t b) {
    uint32_t x = (uint32_t)a - b + q;
    return fieldReduceOnce(x);
}

const uint32_t barrettMultiplier = 5039; // 4^12 / q
const uint32_t barrettShift = 24;        // log₂(4^12)

uint16_t fieldReduce(uint32_t a) {
    uint32_t quotient = (uint64_t)a * barrettMultiplier >> barrettShift;
    return fieldReduceOnce(a - quotient * q);
}

uint16_t fieldMul(uint16_t a, uint16_t b) {
    uint32_t x = (uint32_t)a * b;
    return fieldReduce(x);
}

////////////////////////////////////////////////


// zetas are the values ζ^BitRev7(k) mod q for each index k.-> Doc de Go
const uint16_t zetas[128]= {1, 1729, 2580, 3289, 2642, 630, 1897, 848, 1062, 1919, 193, 797, 2786, 3260, 569, 1746, 296, 2447, 1339, 1476, 3046, 56, 2240, 1333, 1426, 2094, 535, 2882, 2393, 2879, 1974, 821, 289, 331, 3253, 1756, 1197, 2304, 2277, 2055, 650, 1977, 2513, 632, 2865, 33, 1320, 1915, 2319, 1435, 807, 452, 1438, 2868, 1534, 2402, 2647, 2617, 1481, 648, 2474, 3110, 1227, 910, 17, 2761, 583, 2649, 1637, 723, 2288, 1100, 1409, 2662, 3281, 233, 756, 2156, 3015, 3050, 1703, 1651, 2789, 1789, 1847, 952, 1461, 2687, 939, 2308, 2437, 2388, 733, 2337, 268, 641, 1584, 2298, 2037, 3220, 375, 2549, 2090, 1645, 1063, 319, 2773, 757, 2099, 561, 2466, 2594, 2804, 1092, 403, 1026, 1143, 2150, 2775, 886, 1722, 1212, 1874, 1029, 2110, 2935, 885, 2154};

/////////////////////////////////////////////////



//NTT
void NTT(const uint16_t *f, uint16_t *f_ntt) {
    uint16_t f_copy[n]; // Crear una copia del array de entrada
    
    // Inicializar la copia
    for (int i = 0; i < n; i++) {
        f_ntt[i] = f[i];
    }
    
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
/////////////////////////////////////////////////////////////////////////////

//NTT Con operacion modular??

/*void NTT(const uint16_t *f, uint16_t *f_ntt) {
    uint16_t f_copy[n]; // Crear una copia del array de entrada
    
    // Inicializar la copia
    for (int i = 0; i < n; i++) {
        f_copy[i] = f[i];
    }
    
    for (int len = 128; len >= 2; len /= 2) {
        for (int start = 0; start < n; start += 2 * len) {
            for (int j = start; j < start + len; j++) {
                uint16_t t = fieldMul(zetas[j], f_ntt[j + len]); // Paso 8 (operación modular)
                f_ntt[j + len] = fieldSub(f_ntt[j], t); // Paso 9 (operación modular)
                f_ntt[j] = fieldAdd(f_copy[j], t); // Paso 10 (operación modular)
            }
        }
    }
}
*/

//////////////////////////////////////////////////////////////////////////////////

//inverseNTT
void inverseNTT(const uint16_t *f_ntt, uint16_t *f) {
    uint16_t f_copy[n]; // Crear una copia del array de entrada f_ntt
    
    // Inicializar la copia
    for (int i = 0; i < n; i++) {
        f_copy[i] = f_ntt[i];
    }
    
    // Iteración inversa en la longitud de la transformada
    for (int len = 2; len <= 128; len *= 2) {
        // Iteración sobre los subgrupos de la transformada
        for (int start = 0; start < n; start += 2 * len) {
            // Iteración sobre los coeficientes en el subgrupo
            for (int j = start; j < start + len; j++) {
                uint32_t t = zetas[j] * (uint32_t)f_ntt[j + len]; 
                f_copy[j + len] = (f_copy[j] + 3329 - t) % 3329; // Paso 9 MODULAR 
                f_copy[j] = (f_copy[j] + t) % 3329; // Paso 10 MODULAR 
            }
        }
    }
    
    // Multiplicar cada entrada por 3303 mod q
    for (int i = 0; i < n; i++) {
        f[i] = (f[i] * 3303) % 3329;
    }
}
/////////////////////////////////////////////////////////////////////////////

//INVERSE NTT con operación modular??

/*
void inverseNTT(const uint16_t *f_ntt, uint16_t *f) {
    uint16_t f_copy[n]; // Crear una copia del array de entrada f_ntt
    
    // Inicializar la copia
    for (int i = 0; i < n; i++) {
        f_copy[i] = f_ntt[i];
    }
    
    // Iteración inversa en la longitud de la transformada
    for (int len = 2; len <= 128; len *= 2) {
        // Iteración sobre los subgrupos de la transformada
        for (int start = 0; start < n; start += 2 * len) {
            uint16_t zeta = zetas[start / len]; // Obtener zeta inverso
            // Iteración sobre los coeficientes en el subgrupo
            for (int j = start; j < start + len; j++) {
                uint16_t t = f_ntt[j];
                f[j] = fieldAdd(t, f_ntt[j + len]); // Paso 9: sumar y reducir
                f[j + len] = fieldMul(zeta, fieldSub(t, f_ntt[j + len])); // Paso 10: multiplicar por zeta y reducir
            }
        }
    }
    
    // Multiplicar cada entrada por 3303 mod q
    for (int i = 0; i < n; i++) {
        f[i] = fieldMul(f[i], 3303);
    }
}

*/
/////////////////////////////////////////////////////////////////////


//COMPRESS-DECOMPRESS -> Pendiente de revisar el redondeo

uint16_t Compress(uint16_t x, int d) {
    return (x * (1 << d)) / q;
}

uint16_t Decompress(uint16_t y, int d) {
    return (y * q) / (1 << d);
}


///////////////////////////////////////