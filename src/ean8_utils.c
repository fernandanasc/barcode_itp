#include "ean8_utils.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>


bool validate_ean8(const char *identifier) {
    if (strlen(identifier) != 8) {
        fprintf(stderr, "Identificador deve conter exatamente 8 dígitos.\n");
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(identifier[i])) {
            fprintf(stderr, "Identificador contém caracteres não numéricos.\n");
            return false;
        }
    }
    char expected_check_digit = calculate_check_digit(identifier);
    if (identifier[7] != expected_check_digit) {
        fprintf(stderr, "Dígito verificador inválido.\n");
        return false;
    }
    return true;
}

char calculate_check_digit(const char *identifier) {
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        int digit = identifier[i] - '0';
        sum += (i % 2 == 0) ? 3*digit : 1 * digit;
    }
    int remainder = sum % 10;
    return (remainder == 0) ? '0' : '0' + (10 - remainder);
}

 // Definição simplificada do padrão EAN-8
char l_patterns[10][7] = {
    {'0', '0', '0', '1', '1', '0', '1'}, // L-code para '0'
    {'0', '0', '1', '1', '0', '0', '1'}, // L-code para '1'
    {'0', '0', '1', '0', '0', '1', '1'}, // L-code para '2'
    {'0', '1', '1', '1', '1', '0', '1'}, // L-code para '3'
    {'0', '1', '0', '0', '0', '1', '1'}, // L-code para '4'
    {'0', '1', '1', '0', '0', '0', '1'}, // L-code para '5'
    {'0', '1', '0', '1', '1', '1', '1'}, // L-code para '6'
    {'0', '1', '1', '1', '0', '1', '1'}, // L-code para '7'
    {'0', '1', '1', '0', '1', '1', '1'}, // L-code para '8'
    {'0', '0', '0', '1', '0', '1', '1'}  // L-code para '9'
};

char r_patterns[10][7] = {
    {'1', '1', '1', '0', '0', '1', '0'}, // R-code para '0'
    {'1', '1', '0', '0', '1', '1', '0'}, // R-code para '1'
    {'1', '1', '0', '1', '1', '0', '0'}, // R-code para '2'
    {'1', '0', '0', '0', '0', '1', '0'}, // R-code para '3'
    {'1', '0', '1', '1', '1', '0', '0'}, // R-code para '4'
    {'1', '0', '0', '1', '1', '1', '0'}, // R-code para '5'
    {'1', '0', '1', '0', '0', '0', '1'}, // R-code para '6'
    {'1', '0', '0', '0', '1', '0', '0'}, // R-code para '7'
    {'1', '0', '0', '1', '0', '0', '0'}, // R-code para '8'
    {'1', '1', '1', '0', '1', '0', '0'}  // R-code para '9'
};

void ean8_to_bars(const char *identifier, char *bars) {
    // Barra inicial
    bars[0] = '1'; bars[1] = '0'; bars[2] = '1';

    int pos = 3; // Posição atual no array de barras

    // Gerar barras para os primeiros 4 dígitos (L-codes)
    for (int i = 0; i < 4; i++) {
        int digit = identifier[i] - '0';
        for (int j = 0; j < 7; j++) {
            bars[pos] = l_patterns[digit][j];
            pos++;
        }
    }

    // Barra de separação (meio do código de barras)
    bars[pos++] = '0'; bars[pos++] = '1'; bars[pos++] = '0'; bars[pos++] = '1'; bars[pos++] = '0';

    // Gerar barras para os últimos 4 dígitos (R-codes)
    for (int i = 4; i < 8; i++) {
        int digit = identifier[i] - '0';
        for (int j = 0; j < 7; j++) {
            bars[pos++] = r_patterns[digit][j];
        }
    }

    // Barra final
    bars[pos++] = '1'; bars[pos++] = '0'; bars[pos++] = '1';
}


int verificar_bordas(const char *seguiment) {
    return strncmp(seguiment, "101", 3) == 0;
}

int verificar_meio(const char *seguiment) {
    // Verifica se os três primeiros caracteres são "101"
    return strncmp(seguiment, "01010", 5) == 0;
}

int verificar_inicio(const char *bars) {
    // Verifica se os três primeiros caracteres são "101"
    return strncmp(bars, "101", 3) == 0;
}

int verificar_fim(const char *bars) {
    size_t length = strlen(bars); // Obtém o comprimento da string
    if (length < 3) {
        // A string é menor que 3 caracteres, não pode corresponder
        return 0;
    }
    // Compara os últimos 3 caracteres com "101"
    return strncmp(bars + length - 3, "101", 3) == 0;
}

int verificar_padroes(const char *segmento, const char padrao[7]) {
    for (int i = 0; i < 7; i++) {
        if (segmento[i] != padrao[i]) {
            return 0; // Diferente
        }
    }
    return 1; // Iguais
}

char decodificar_bar(const char *digitos, char side) {
    
    if (side == 'l') {
        for (int i = 0; i < 10; i++) {
            if (verificar_padroes(digitos, l_patterns[i])) {
                //printf("chaoum - %d - llll 135", i);
                //printf("---- %c, ",i);
                return '0' + i; // Número encontrado nos padrões L
            }
        }
    } else if (side == 'r') {
        for (int i = 0; i < 10; i++) {
            if (verificar_padroes(digitos, r_patterns[i])) {
                return '0' + i; // Número encontrado nos padrões R
            }
        }
    }
    return -1;
}

// Função para decodificar as barras em um identificador EAN-8
bool decode_bars_to_ean8(const char *bars, char *identifier) {
    if (!bars || !identifier) {
        return false;
    }
    
    const char *borda_inicial = bars;
    const char *digitos_l = bars + 3;
    const char *mid = bars + 3 + 28;
    const char *digitos_r = bars + 3 + 28 + 5;
    const char *borda_final = bars + 3 + 28 + 5 + 28;


    if (!verificar_bordas(borda_inicial)) {
        return false;
    }

    if (!verificar_meio(mid)) {
        return false;
    }
   
    if (!verificar_bordas(borda_final)) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        char digito = decodificar_bar(digitos_l +  i*7, 'l');
        if (digito == -1) {
            return false;
        } else {
            identifier[i] = digito;
        }
    }

    for (int i = 0; i < 4; i++) {
        char digito = decodificar_bar(digitos_r + i*7, 'r');
        if (digito == -1) {
            return false;
        } else {
            identifier[i+4] = digito;
        }
    }

    identifier[8] = '\0';

    return true;
}