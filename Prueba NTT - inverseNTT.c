#include <stdio.h>
#include <stdint.h>

#define n 256 // Tamaño del arreglo de entrada (debe ser una potencia de 2)
#define q 3329 // Módulo para operaciones modulares

// Suponiendo que tienes definidas las variables globales zetas
// zetas are the values ζ^BitRev7(k) mod q for each index k.-> Doc de Go
const uint16_t zetas[128]= {1, 1729, 2580, 3289, 2642, 630, 1897, 848, 1062, 1919, 193, 797, 2786, 3260, 569, 1746, 296, 2447, 1339, 1476, 3046, 56, 2240, 1333, 1426, 2094, 535, 2882, 2393, 2879, 1974, 821, 289, 331, 3253, 1756, 1197, 2304, 2277, 2055, 650, 1977, 2513, 632, 2865, 33, 1320, 1915, 2319, 1435, 807, 452, 1438, 2868, 1534, 2402, 2647, 2617, 1481, 648, 2474, 3110, 1227, 910, 17, 2761, 583, 2649, 1637, 723, 2288, 1100, 1409, 2662, 3281, 233, 756, 2156, 3015, 3050, 1703, 1651, 2789, 1789, 1847, 952, 1461, 2687, 939, 2308, 2437, 2388, 733, 2337, 268, 641, 1584, 2298, 2037, 3220, 375, 2549, 2090, 1645, 1063, 319, 2773, 757, 2099, 561, 2466, 2594, 2804, 1092, 403, 1026, 1143, 2150, 2775, 886, 1722, 1212, 1874, 1029, 2110, 2935, 885, 2154};

void inverseNTT(uint16_t *f_ntt) {
    uint16_t f[n]; // Crear una copia del array de entrada para calcular la inverse-NTT
    
    // Copiar el array de entrada a f
    for (int i = 0; i < n; i++) {
        f[i] = f_ntt[i];
    }
    
    int k = 127;
    for (int len = 2; len <= 128; len *= 2) {
        for (int start = 0; start < n; start += 2 * len) {
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
    for (int i = 0; i < n; i++) {
        f[i] = (f[i] * 3303) % q;
    }
    
    // Copiar el resultado de la inverse-NTT al array original f_ntt
    for (int i = 0; i < n; i++) {
        f_ntt[i] = f[i];
    }
}


void NTT(uint16_t *f) {
    uint16_t f_ntt[n]; // Crear una copia del array de entrada para calcular la NTT
    
    // Copiar el array de entrada a f_ntt
    for (int i = 0; i < n; i++) {
        f_ntt[i] = f[i];
    }
    
    int k = 1;
    for (int len = 128; len >= 2; len /= 2) {
        for (int start = 0; start < n; start += 2 * len) {
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
    for (int i = 0; i < n; i++) {
        f[i] = f_ntt[i];
    }
}

int main() {
    // Arreglo de prueba
    uint16_t f[n];
    for (int i = 0; i < n; i++) {
        f[i] = i;
    }
    printf("Resultado de NTT:\n");
    for (int i = 0; i < n; i++) {
    printf("%d ", f[i]);
    }
    printf("\n");
    
    // Llamar a la función NTT
    NTT(f);

    // Imprimir el resultado
    printf("Resultado de NTT:\n");
    for (int i = 0; i < n; i++) {
    printf("%d ", f[i]);
    }
    printf("\n");
    
     // Llamar a la función inverseNTT
    inverseNTT(f);

    // Imprimir el resultado
    printf("Resultado de inverseNTT:\n");
    for (int i = 0; i < n; i++) {
    printf("%d ", f[i]);
    }
    printf("\n");
    return 0;
    
}
