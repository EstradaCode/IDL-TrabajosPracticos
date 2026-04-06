#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pfrep.h"
 // Procesa el string y valida que la parte entera esté entre los límites.
 //Retorna 1 si es válido, 0 si falla.
int validar_str(const char* s, int32_t sup, int32_t inf, int32_t* resultado_entero) {
    int64_t acumulador = 0;
    int signo = 1;
    int i = 0;

    if (s[0] == '-') {
        signo = -1;
        i = 1;
    }

    if (s[i] == '\0' || s[i] == '.') return 0;

    while (s[i] != '\0' && s[i] != '.') {
        if (s[i] < '0' || s[i] > '9') return 0;
        
        acumulador = acumulador * 10 + (s[i] - '0');
        
        // Validación en tiempo real para no desbordar el int64_t 
        if ((acumulador * signo) > sup || (acumulador * signo) < inf) return 0;
        
        i++;
    }

    *resultado_entero = (int32_t)(acumulador * signo);
    return 1;
}
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
 // 
int main() {
    char str_m[64], str_b[64], str_x[64];
    int32_t m_entero, b_entero, x_entero;

    printf("=== Representación recta en punto fijo Q(16,15) - GRUPO07 - IDL ===\n");

    // Validacion de m (Límites -1 a 0 para los enteros)
    printf("m [-1, 1): ");
    scanf("%63s", str_m);
    if (!validar_str(str_m, 0, -1, &m_entero)) {
        // Caso borde: si el string es "0.99", la parte entera es 0, es válido.
        // Solo falla si la parte entera es >= 1 o < -1.
        printf("Error: m fuera de rango.\n"); return 1;
    }

    // Validacion de b (Límites -128 a 127)
    printf("b [-128, 128): ");
    scanf("%63s", str_b);
    if (!validar_str(str_b, 127, -128, &b_entero)) {
        printf("Error: b fuera de rango.\n"); return 1;
    }

    // Validacion de x (Límites -65407 a 65407)
    printf("x [-65407, 65407]: ");
    scanf("%63s", str_x);
    if (!validar_str(str_x, 65407, -65407, &x_entero)) {
        printf("Error: x fuera de rango.\n");
        return 1;
    }

    // leo mis argumentos enviados
    int16_t m = str_to_q15(str_m);
    int16_t b_q78 = str_to_q78(str_b);
    int32_t x = str_to_q1615(str_x);

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
