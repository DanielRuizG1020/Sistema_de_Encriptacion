#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define q 3329
#define nu 2
#define N 256
#define d 12

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

//SamplePolyCBD
void SamplePolyCBD(const unsigned char *B, unsigned int *f) {
    unsigned int b[64 * nu * 8]; // Array de bits convertidos de los bytes de entrada

    // Convertir los bytes de entrada a bits
    BytesToBits(B, 64 * nu, b);

    // Para cada coeficiente del polinomio
    for (int i = 0; i < N; i++) {
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
        f[i] = (x - y) % q;    // Aritmética modular
    }
}
int main() {

		//Main SamplePolyCBD
///
 // Definir un array de prueba de bytes
 unsigned char bytes[64 * nu];

 // Generar bytes aleatorios para el array de prueba
 srand(time(NULL));
 for (int i = 0; i < (64 * nu + 7) / 8; i++) {
     bytes[i] = rand();
 }

 // Definir un array para almacenar el resultado
 unsigned int result[N];

 // Llamar a la función SamplePolyCBD
 SamplePolyCBD(bytes, result);

 // Imprimir el resultado
 printf("Resultado de SamplePolyCBD:\n");
 for (int i = 0; i < N; i++) {
     printf("%u ", result[i]);
 }
 printf("\n");

 return 0;


}


