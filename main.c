
/*
/brief este es el primer programa pa probar el compilador y el sistema de desarrollo de git hub
/author Daniel Ruiz Guirales, Andres Felipe Pega Betancur
*/
//Librerias


//falta prototipo de la funcion

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// ML-KEM-768 parameters
#define q 3329
#define nu 2
#define N 256
#define d 12

/**
 * Convierte un array de bits en un array de bytes.
 * @param b Array de enteros sin signo que representa los bits.
 * @param b_len Longitud del array de bits.
 * @param B Puntero a un array de caracteres sin signo donde se almacenarán los bytes resultantes.
 */
void BitsToBytes(const unsigned int *b, int b_len, unsigned char *B);

/**
 * Convierte un array de bytes en un array de bits.
 * @param B Array de caracteres sin signo que representa los bytes.
 * @param B_len Longitud del array de bytes.
 * @param b Puntero a un array de enteros sin signo donde se almacenarán los bits resultantes.
 */
void BytesToBits(const unsigned char *B, int B_len, unsigned int *b);

/**
 * Serializa un arreglo de enteros de "d" bits en un arreglo de 32 * d bytes..
 * @param F Array de enteros a serializar.
 * @param B Puntero a un array de caracteres sin signo donde se almacenarán los bytes resultantes.
 */
void ByteEncoded(const unsigned int *F, unsigned char *B);

/**
 * Realiza la operación de deserialización correspondiente, convirtiendo un arreglo de 32 * d bytes en un arreglo de enteros de "d" bits.
 * @param B Array de caracteres sin signo que representa los bytes serializados.
 * @param F Puntero a un array de enteros donde se almacenarán los enteros resultantes.
 */
void ByteDecoded(const unsigned char *B, unsigned int *F);

/**
 * Comprime un entero de 16 bits en otro entero de 16 bits.
 * @param x Entero de 16 bits a comprimir.
 * @return Entero de 16 bits comprimido.
 */
uint16_t Compress(uint16_t x);

/**
 * Descomprime un entero de 16 bits previamente comprimido.
 * @param y Entero de 16 bits comprimido.
 * @return Entero de 16 bits descomprimido.
 */
uint16_t Decompress(uint16_t y);

/**
 * Convierte una secuencia de bytes en un polinomio en el dominio de la Transformada de Fourier Numérica (NTT).
 * @param B Secuencia de bytes.
 * @param a Puntero al array donde se almacenará el polinomio en el dominio NTT.
 */
void sampleNTT(const unsigned char *B, unsigned int *a);

/**
 * Esta función implementa un esquema de muestreo conocido como "Centered Binomial Distribution" (CBD).
 * @param B Secuencia de bytes.
 * @param f Puntero al array donde se almacenará el polinomio muestreado.
 */
void SamplePolyCBD(const unsigned char *B, unsigned int *f);

/**
 * Realiza la Transformada NTT en un polinomio dado.
 * @param f Puntero al array que representa el polinomio en el anillo Rq.
 */
void NTT(uint16_t *f);

/**
 * Realiza la transformada inversa de la Transformada  NTT en un polinomio dado.
 * @param f_ntt Puntero al array que representa el polinomio en el anillo Rq.
 */
void inverseNTT(uint16_t *f_ntt);

/**
 * Realiza una multiplicación específica de los coeficientes de los polinomios en el caso base para la operación de multiplicación de polinomios en el anillo Rq.
 * @param a0 Coeficiente del primer polinomio.
 * @param a1 Coeficiente del primer polinomio.
 * @param b0 Coeficiente del segundo polinomio.
 * @param b1 Coeficiente del segundo polinomio.
 * @param gamma Parámetro de compresión.
 * @return Resultado de la multiplicación.
 */
uint16_t BaseCaseMultiplyc0(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1, uint16_t gamma);

/**
 * Realiza otra multiplicación específica de los coeficientes de los polinomios en el caso base para la operación de multiplicación de polinomios en el anillo Rq.
 * @param a0 Coeficiente del primer polinomio.
 * @param a1 Coeficiente del primer polinomio.
 * @param b0 Coeficiente del segundo polinomio.
 * @param b1 Coeficiente del segundo polinomio.
 * @return Resultado de la multiplicación.
 */
uint16_t BaseCaseMultiplyc1(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1);

/**
 * Multiplica dos polinomios en el dominio de la Transformada de Fourier Numérica (NTT).
 * @param f_ntt Puntero al array que representa el primer polinomio en el dominio NTT.
 * @param g_ntt Puntero al array que representa el segundo polinomio en el dominio NTT.
 * @param h_ntt Puntero al array donde se almacenará el resultado de la multiplicación.
 */
void MultiplyNTTs(uint16_t *f_ntt, uint16_t *g_ntt, uint16_t *h_ntt);






int main(void){
    printf("Hola Mundo");
    return 0;

}

/////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////////////////////////////////////////
/// ByteDecoded
void ByteDecoded(const unsigned char *B,unsigned int *F) {

    unsigned int B_len = (N * d + 7) / 8;
    unsigned int b[B_len * 8];
    BytesToBits(B, B_len, b); 
    // Decodificar los bits en enteros de d bits
    for (int i = 0; i < 256; i++) {
        unsigned int sum = 0;
        for (int j = 0; j < d; j++) {
            sum += b[i * d + j] << j; // Sumar los bits multiplicados por 2^j
        }
        F[i] = sum%q;
    }
}
//////////////////////////////////////////////////////////////////////////////////////

// Compress
uint16_t Compress(uint16_t x) {
    // Realizar la operación con mayor precisión y redondear al entero más cercano
    uint32_t result = (uint32_t)x * (1 << d);
    result += q / 2;  // Sumar la mitad de q para el redondeo
    return (uint16_t)(result / q);
}

////////////////////////////////////////////////////////////////////////////
//Decompress
uint16_t Decompress(uint16_t y) {
    // Realizar la operación con mayor precisión y redondear al entero más cercano
    uint32_t result = (uint32_t)y * q;
    result += (1 << (d - 1));  // Sumar la mitad de 2^d para el redondeo
    return (uint16_t)(result / (1 << d));
}

////////////////////////////////////////////////////////////////////////////////////////////////


//SampleNTT
void sampleNTT(const unsigned char *B, unsigned int *a) {
    int i = 0;
    int j = 0;
    while (j < 256) {
        unsigned int d1 = B[i] + 256 * (B[i + 1] % 16);
        unsigned int d2 = (B[i + 1] / 16) + 16 * B[i + 2];
        if (d1 < q) {
            a[j] = d1;
            j++;
        }
        if (d2 < q && j < 256) {
            a[j] = d2;
            j++;
        }
        i += 3;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//SamplePolyCBD
void SamplePolyCBD(const unsigned char *B, unsigned int *f) {
    unsigned int b[64 * nu * 8]; 
    BytesToBits(B, 64 * nu, b);

    for (int i = 0; i < N; i++) {
       
        unsigned int x = 0;
        for (int j = 0; j < nu; j++) {
            x += b[2 * i * nu + j];
        }
        unsigned int y = 0;
        for (int j = 0; j < nu; j++) {
            y += b[2 * i * nu + nu + j];
        }

        // Calcular el coeficiente del polinomio
        f[i] = (x - y) % q;    // Aritmética modular
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

//Este son valores precalculados segun el doc
// zetas are the values ζ^BitRev7(k) mod q for each index k.-> Doc de Go
const uint16_t zetas[128]= {1, 1729, 2580, 3289, 2642, 630, 1897, 848, 1062, 1919, 193, 797, 2786, 3260, 569, 1746, 296, 2447, 1339, 1476, 3046, 56, 2240, 1333, 1426, 2094, 535, 2882, 2393, 2879, 1974, 821, 289, 331, 3253, 1756, 1197, 2304, 2277, 2055, 650, 1977, 2513, 632, 2865, 33, 1320, 1915, 2319, 1435, 807, 452, 1438, 2868, 1534, 2402, 2647, 2617, 1481, 648, 2474, 3110, 1227, 910, 17, 2761, 583, 2649, 1637, 723, 2288, 1100, 1409, 2662, 3281, 233, 756, 2156, 3015, 3050, 1703, 1651, 2789, 1789, 1847, 952, 1461, 2687, 939, 2308, 2437, 2388, 733, 2337, 268, 641, 1584, 2298, 2037, 3220, 375, 2549, 2090, 1645, 1063, 319, 2773, 757, 2099, 561, 2466, 2594, 2804, 1092, 403, 1026, 1143, 2150, 2775, 886, 1722, 1212, 1874, 1029, 2110, 2935, 885, 2154};

// gammas are the values ζ^2BitRev7(i)+1 mod q for each index i.
const uint16_t gammas[128]= {17, 3312, 2761, 568, 583, 2746, 2649, 680, 1637, 1692, 723, 2606, 2288, 1041, 1100, 2229, 1409, 1920, 2662, 667, 3281, 48, 233, 3096, 756, 2573, 2156, 1173, 3015, 314, 3050, 279, 1703, 1626, 1651, 1678, 2789, 540, 1789, 1540, 1847, 1482, 952, 2377, 1461, 1868, 2687, 642, 939, 2390, 2308, 1021, 2437, 892, 2388, 941, 733, 2596, 2337, 992, 268, 3061, 641, 2688, 1584, 1745, 2298, 1031, 2037, 1292, 3220, 109, 375, 2954, 2549, 780, 2090, 1239, 1645, 1684, 1063, 2266, 319, 3010, 2773, 556, 757, 2572, 2099, 1230, 561, 2768, 2466, 863, 2594, 735, 2804, 525, 1092, 2237, 403, 2926, 1026, 2303, 1143, 2186, 2150, 1179, 2775, 554, 886, 2443, 1722, 1607, 1212, 2117, 1874, 1455, 1029, 2300, 2110, 1219, 2935, 394, 885, 2444, 2154, 1175};


/////////////////////////////////////////////////
//NTT
void NTT(uint16_t *f) {
    uint16_t f_ntt[N]; // Crear una copia del array de entrada para calcular la NTT
    
    // Copiar el array de entrada a f_ntt
    for (int i = 0; i < N; i++) {
        f_ntt[i] = f[i];
    }
    
    int k = 1;
    for (int len = 128; len >= 2; len /= 2) {
        for (int start = 0; start < N; start += 2 * len) {
            uint16_t zeta = zetas[k];
            k++;

            for (int j = start; j < start + len; j++) {
                uint16_t t = (uint32_t)zeta * f_ntt[j + len] % q; // Paso 8 (operación modular)
                f_ntt[j + len] = (f_ntt[j] + q - t) % q; // Paso 9 (operación modular)
                f_ntt[j] = (f_ntt[j] + t) % q; // Paso 10 (operación modular)
            }
        }
    }
    
    // Copiar el resultado de la NTT al array original f
    for (int i = 0; i < N; i++) {
        f[i] = f_ntt[i];
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//InverseNTT

void inverseNTT(uint16_t *f_ntt) {
    uint16_t f[N]; // Crear una copia del array de entrada para calcular la inverse-NTT
    
    // Copiar el array de entrada a f
    for (int i = 0; i < N; i++) {
        f[i] = f_ntt[i];
    }
    
    int k = 127;
    for (int len = 2; len <= 128; len *= 2) {
        for (int start = 0; start < N; start += 2 * len) {
            uint16_t zeta = zetas[k]; // Obtener el valor de zeta según el pseudocódigo
            k--;

            for (int j = start; j < start + len; j++) {
                uint16_t t = f[j];
                f[j] = (t + f[j + len]) % q; // Paso 9 (operación modular)
                f[j + len] = (uint32_t)zeta * (f[j + len] + q - t) % q; // Paso 10 (operación modular)
            }
        }
    }
    
    // Multiplicar cada entrada por 3303 mod q
    for (int i = 0; i < N; i++) {
        f[i] = (f[i] * 3303) % q;
    }
    
    // Copiar el resultado de la inverse-NTT al array original f_ntt
    for (int i = 0; i < N; i++) {
        f_ntt[i] = f[i];
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Esta funcion esta en el doc con dos retornos, nosotros lo separamos. C0 Y C1

// BaseCaseMultiplyC0
uint16_t BaseCaseMultiplyc0(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1, uint16_t gamma) {
    uint16_t c0 = ((uint32_t)a0 * b0 + (uint32_t)a1 * b1 * gamma) % q; 
    return c0;
}
/////////////////////////////////////////////////////////////////////////
// BaseCaseMultiplyC1
uint16_t BaseCaseMultiplyc1(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1) {
    uint16_t c1 = ((uint32_t)a0 * b1 + (uint32_t)a1 * b0) % q; 
    return c1;
}
///////////////////////////////////////////////////////////////////////////////////////



//MultiplyNTTs
void MultiplyNTTs(uint16_t *f_ntt, uint16_t *g_ntt, uint16_t *h_ntt) {
    for (int i = 0; i < 128; i++) {
        h_ntt[2*i] = BaseCaseMultiplyc0(f_ntt[2*i], f_ntt[2*i + 1], g_ntt[2*i], g_ntt[2*i + 1], gammas[i]);
        h_ntt[2*i + 1] = BaseCaseMultiplyc1(f_ntt[2*i], f_ntt[2*i + 1], g_ntt[2*i], g_ntt[2*i + 1]);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/*//Operaciones Modulares
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
*/
//////////////////////////////////////////////////////////////////////////////////////////////
