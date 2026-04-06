#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pfrep.h"

int main() {
    char input[16];
    char hex_out[7]; // 0xHHHH\0
    int16_t q78_val;

    printf("=== GRUPO 07 - IDL - PUNTO F - REPRESENTACION DECIMAL A Q(7,8) ===\n");
    printf("Ingrese un valor (±eee.ffff) en el rango [-128, 127.99]: ");
    
    if (scanf("%15s", input) != 1) {
        fprintf(stderr, "Error al leer la entrada.\n");
        return 1;
    }
    
    q78_val = str_to_q78(input);

    q78_to_hex(q78_val, hex_out);

    printf("\nEntrada: %s\n", input);
    printf("Representación en hexadecimal: %s\n", hex_out);

    return 0;
}
