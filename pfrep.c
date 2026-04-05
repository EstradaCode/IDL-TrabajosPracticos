#include "pfrep.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/**
 * Convierte un string "±eee.ffff" a representación Q7.8 (16 bits)
 * 
 */
int16_t str_to_q78(const char* s) {
    uint16_t parte_entera = 0;
    uint32_t parte_fraccionaria = 0;
    uint32_t divisor_escalonado = 1;
    int signo = 1;
    int i = 0;

    if (s[i] == '-') { signo = -1; i++; }
    else if (s[i] == '+') { i++; }

    while (s[i] != '.' && s[i] != '\0') {
        if (s[i] < '0' || s[i] > '9') {
            fprintf(stderr, "Error: Caracter invalido '%c'\n", s[i]);
            exit(1);
        }
        parte_entera = parte_entera * 10 + (s[i++] - '0'); 
    }

    if ((signo == 1 && parte_entera > 127) || (signo == -1 && parte_entera > 128)) {
        fprintf(stderr, "Error: fuera de rango [-128, 127.99]\n");
        exit(1);
    }

    if (s[i] == '.') {
        i++;
        while (s[i] != '\0') {
            if (s[i] < '0' || s[i] > '9') break;
            parte_fraccionaria = parte_fraccionaria * 10 + (s[i++] - '0');
            divisor_escalonado *= 10;
        }
        parte_fraccionaria = (parte_fraccionaria << 8) / divisor_escalonado;
    }

    uint16_t res_abs = (uint16_t)((parte_entera << 8) | (parte_fraccionaria & 0xFF));
    
    int16_t resultado = (int16_t)res_abs;
    return (signo == -1)? -resultado : resultado;

}

void q78_to_hex(int16_t val, char* out) {
    const char hex_chars[] = "0123456789ABCDEF";
    uint16_t uval = (uint16_t)val; //

    out[0] = '0';
    out[1] = 'x';
    out[2] = hex_chars[(uval >> 12) & 0xF]; // (msb)
    out[3] = hex_chars[(uval >> 8) & 0xF];  //
    out[4] = hex_chars[(uval >> 4) & 0xF];  //
    out[5] = hex_chars[uval & 0xF];         // (LSB)
    out[6] = '\0';
}
/**
 * Convierte un string "±.ffff" a representación Q15 (16 bits)
 * 
 */
int16_t str_to_q15(const char* s) {
    int32_t parte_entera = 0;
    uint32_t parte_fraccionaria = 0;
    uint32_t divisor_escalonado = 1;
    int signo = 1;
    int i = 0;

    if (s[i] == '-') { signo = -1; i++; }
    else if (s[i] == '+') { i++; }

    while (s[i] != '.' && s[i] != '\0') {
        parte_entera = parte_entera * 10 + (s[i++] - '0'); 
    }

    if (parte_entera > 1) {
        fprintf(stderr, "Error: m fuera de rango para Q0.15\n");
        exit(1);
    }

    if (s[i] == '.') {
        i++;
        while (s[i] != '\0' && s[i] >= '0' && s[i] <= '9') {
            parte_fraccionaria = parte_fraccionaria * 10 + (s[i++] - '0');
            divisor_escalonado *= 10;
        }
        parte_fraccionaria = (parte_fraccionaria << 15) / divisor_escalonado;
    }

    uint16_t res_abs = (uint16_t)((parte_entera << 15) | (parte_fraccionaria & 0x7FFF));
    
    int16_t resultado = (int16_t)res_abs;
    return (signo == -1) ? -resultado : resultado;
}
/**
 * Convierte un string "±eee.ffff" a representación Q16.15 (32 bits)
 * 
 */
int32_t str_to_q1615(const char* s) {
    int32_t parte_entera = 0;
    uint64_t parte_fraccionaria = 0; 
    uint32_t divisor_escalonado = 1;
    int signo = 1;
    int i = 0;

    if (s[i] == '-') { signo = -1; i++; }
    else if (s[i] == '+') { i++; }

    
    while (s[i] != '.' && s[i] != '\0') {
        if (s[i] >= '0' && s[i] <= '9') {
            parte_entera = parte_entera * 10 + (s[i++] - '0');
        } else break;
    }

    if (parte_entera > 32767) {
        fprintf(stderr, "Error: x fuera de rango para Q16.15\n");
        exit(1);
    }

    if (s[i] == '.') {
        i++;
        while (s[i] != '\0' && s[i] >= '0' && s[i] <= '9') {
            parte_fraccionaria = parte_fraccionaria * 10 + (s[i++] - '0');
            divisor_escalonado *= 10;
        }
        parte_fraccionaria = (parte_fraccionaria << 15) / divisor_escalonado;
    }

    uint32_t res_abs = (uint32_t)((parte_entera << 15) | (parte_fraccionaria & 0x7FFF));
    
    int32_t resultado = (int32_t)res_abs;
    return (signo == -1) ? -resultado : resultado;
}
