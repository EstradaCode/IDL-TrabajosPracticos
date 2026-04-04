#ifndef PFREP_H
#define PFREP_H

#include <stdint.h>
int16_t str_to_q78(const char* s); 
void q78_to_hex(int16_t val, char* out);
#endif
