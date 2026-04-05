#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pfrep.h"

// Función para obtener el string decimal (modificada para devolverlo o imprimirlo limpio)
void fp32_to_decimal(int32_t q_val) {
    int32_t entera = q_val >> 15;
    uint32_t fracc_bits = q_val & 0x7FFF;
    int negativo = (q_val & 0x80000000);
    
    if (negativo && fracc_bits != 0) { // se arregla la visualizacion de un negativo con fraccionarios. debido al calculo de interpretacion que tiene el cpu.
        fracc_bits = (~fracc_bits + 1) & 0x7FFF;
        entera += 1;
    }
    
    uint32_t fracc_decimal = (uint32_t)((uint64_t)fracc_bits * 100000 >> 15);

    if (negativo && entera == 0) printf("-0.%05u", fracc_decimal); // no interpreta el negativo de una forma escalada en la impresión, caso 0.xxxx
    else printf("%d.%05u", (int)entera, fracc_decimal);
}
 // argv[1] => m ; argv[2]=> b; argv[3]=> x || out => y 
int main(int argc, char *argv[]) {
    // se utilizan parametros de entrada como entrada (argc/argv)
    if (argc < 4) {
        return 1; // si no hay parametros necesarios exit -1
    }

    // leo mis argumentos enviados
    int16_t m = str_to_q15(argv[1]);
    int16_t b_q78 = str_to_q78(argv[2]);
    int32_t x = str_to_q1615(argv[3]);

    // Cálculo de y = mx + b ; traduzco todo al mismo formato
    int32_t b_32 = (int32_t)b_q78 << 7; 
    int64_t producto_64 = (int64_t)m * x; // se utiliza una base mayor por la multiplicación, aunque deberian ser 48bits debido a ser n + m bits (n=32, m = 16)
    int32_t mx_q15 = (int32_t)(producto_64 >> 15); // se reajusta a nuestra representación q(16,15) sin perdida de info
    int32_t y = mx_q15 + b_32; 

    // SALIDAS EN HEXA
    // m_hex,b_hex,x_hex,y_hex,y_decimal
    printf("0x%08X,", (uint32_t)y);
    fp32_to_decimal(y);
    printf("\n");

    return 0;
}
