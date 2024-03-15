#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define q 3329
#define nu 2
#define N 256
#define d 12

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

int main() {
//Semilla para la generación de números aleatorios
 srand(time(NULL));

 // Crear un array de bytes aleatorios como entrada
 unsigned char bytes[3 * N];
 for (int i = 0; i < 3 * N; i++) {
     bytes[i] = rand() % 256; // Generar un byte aleatorio
 }

 // Array para almacenar los resultados de la función sampleNTT
 unsigned int result[N];

 // Llamar a la función sampleNTT
 sampleNTT(bytes, result);

 // Imprimir el resultado
 printf("Resultado de sampleNTT:\n");
 for (int i = 0; i < N; i++) {
     printf("%u ", result[i]);
 }
 printf("\n");

 return 0;
	

}


