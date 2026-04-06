#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t q7_8;
    printf("Ingrese número Q(7,8) en hexadecimal: 0x");
    scanf("%hx", (unsigned short*)&q7_8);

    // Parte entera con máscara y desplazamiento
    // El shift >> 8 en un int16_t ya es un "Arithmetic Shift" (mantiene el signo)
    int16_t parte_entera = q7_8 >> 8;

    //Parte fraccionaria con máscara AND (bits 0 a 7)
    uint8_t parte_frac_bits = q7_8 & 0xFF; 

    int negativo = (q7_8 < 0);
    uint32_t parte_frac_decimal;

    if (negativo && parte_frac_bits != 0) {
        /* Lógica de Máscara para Negativos con parte fraccionaria, problema de interpretacion a decimal:
           (~bits + 1) & 0xFF  <-- El & 0xFF limpia cualquier acarreo fuera de los 8 bits
        */
        uint8_t fracc_invertida = (~parte_frac_bits + 1) & 0xFF;
        
        parte_frac_decimal = (fracc_invertida * 10000) >> 8; // >> 8 es equivalente a / 256
        parte_entera += 1; 
    } else {
        // Para positivos, simplemente escalamos y desplazamos
        parte_frac_decimal = (parte_frac_bits * 10000) >> 8;
    }

    // Manejo del signo del cero (para casos como -0.5)
    if (negativo && parte_entera == 0) {
        printf("Número decimal: -%d.%04u\n", parte_entera, parte_frac_decimal);
    } else {
        printf("Número decimal: %d.%04u\n", parte_entera, parte_frac_decimal);
    }
    return 0;
}
