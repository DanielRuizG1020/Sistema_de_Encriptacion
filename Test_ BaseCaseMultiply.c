#include <stdio.h>
#include <stdint.h>

#define q 3329 // Módulo para operaciones modulares

// Definición de la función MultiplyNTTs
void MultiplyNTTs(uint16_t *f_ntt, uint16_t *g_ntt, uint16_t *h_ntt);

// Definición de la función BaseCaseMultiplyc0
uint16_t BaseCaseMultiplyc0(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1, uint16_t gamma);

// Definición de la función BaseCaseMultiplyc1
uint16_t BaseCaseMultiplyc1(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1);

const uint16_t gammas[128]= {17, 3312, 2761, 568, 583, 2746, 2649, 680, 1637, 1692, 723, 2606, 2288, 1041, 1100, 2229, 1409, 1920, 2662, 667, 3281, 48, 233, 3096, 756, 2573, 2156, 1173, 3015, 314, 3050, 279, 1703, 1626, 1651, 1678, 2789, 540, 1789, 1540, 1847, 1482, 952, 2377, 1461, 1868, 2687, 642, 939, 2390, 2308, 1021, 2437, 892, 2388, 941, 733, 2596, 2337, 992, 268, 3061, 641, 2688, 1584, 1745, 2298, 1031, 2037, 1292, 3220, 109, 375, 2954, 2549, 780, 2090, 1239, 1645, 1684, 1063, 2266, 319, 3010, 2773, 556, 757, 2572, 2099, 1230, 561, 2768, 2466, 863, 2594, 735, 2804, 525, 1092, 2237, 403, 2926, 1026, 2303, 1143, 2186, 2150, 1179, 2775, 554, 886, 2443, 1722, 1607, 1212, 2117, 1874, 1455, 1029, 2300, 2110, 1219, 2935, 394, 885, 2444, 2154, 1175};

int main() {
    // Arreglo de prueba para fˆ y gˆ
    uint16_t f_ntt[256];
    uint16_t g_ntt[256];
    for (int i = 0; i < 256; i++) {
        f_ntt[i] = i;
        g_ntt[i] = i + 1;
    }

    // Arreglo para almacenar el resultado de MultiplyNTTs
    uint16_t h_ntt[256];

    // Llamar a la función MultiplyNTTs
    MultiplyNTTs(f_ntt, g_ntt, h_ntt);

    // Imprimir el resultado
    printf("Resultado de MultiplyNTTs:\n");
    for (int i = 0; i < 256; i++) {
        printf("%d ", h_ntt[i]);
    }
    printf("\n");

    return 0;
}

void MultiplyNTTs(uint16_t *f_ntt, uint16_t *g_ntt, uint16_t *h_ntt) {
    for (int i = 0; i < 128; i++) {
        h_ntt[2*i] = BaseCaseMultiplyc0(f_ntt[2*i], f_ntt[2*i + 1], g_ntt[2*i], g_ntt[2*i + 1], gammas[i]);
        h_ntt[2*i + 1] = BaseCaseMultiplyc1(f_ntt[2*i], f_ntt[2*i + 1], g_ntt[2*i], g_ntt[2*i + 1]);
    }
}

uint16_t BaseCaseMultiplyc0(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1, uint16_t gamma) {
    uint16_t c0 = ((uint32_t)a0 * b0 + (uint32_t)a1 * b1 * gamma) % q; 
    return c0;
}

uint16_t BaseCaseMultiplyc1(uint16_t a0, uint16_t a1, uint16_t b0, uint16_t b1) {
    uint16_t c1 = ((uint32_t)a0 * b1 + (uint32_t)a1 * b0) % q; 
    return c1;
}