#ifndef EAN8_UTILS_H
#define EAN8_UTILS_H

#include <stdbool.h>

// Estrutura para o identificador EAN-8
typedef struct {
    char digits[8];
} EAN8;

// Verifica se um identificador é válido
bool validate_ean8(const char *identifier);

// Converte o identificador EAN-8 em uma sequência de barras (0 = branco, 1 = preto)
void ean8_to_bars(const char *identifier, char *bars);

// Calcula o dígito verificador para um identificador EAN-8
char calculate_check_digit(const char *identifier);

/**
 * Decodifica as barras de um código de barras em EAN-8.
 * 
 * @param bars Array representando as barras (preto/branco).
 * @param ean8 Array para armazenar o identificador EAN-8 extraído.
 * @return true se a decodificação foi bem-sucedida; false caso contrário.
 */
bool decode_bars_to_ean8(const char *bars, char *identifier);

#endif
