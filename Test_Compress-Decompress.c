#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define q 3329
#define nu 2
#define N 256
#define d 12

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


int main() {

	//Compress y Decompress
///
 // Semilla para la generación de números aleatorios
 srand(time(NULL));

 // Valor inicial aleatorio para comprimir
 uint16_t x = rand() % q;  // Generar un valor aleatorio entre 0 y q - 1

 // Comprimir el valor inicial
 uint16_t compressed = Compress(x);

 // Descomprimir el valor comprimido
 uint16_t decompressed = Decompress(compressed);

 // Imprimir los resultados
 printf("Valor inicial: %u\n", x);
 printf("Valor comprimido: %u\n", compressed);
 printf("Valor descomprimido: %u\n", decompressed);

 return 0;	




}
