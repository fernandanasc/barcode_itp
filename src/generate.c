#include "ean8_utils.h"
#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH 209

#define DEFAULT_SPACING 4
#define DEFAULT_BAR_WIDTH 3
#define DEFAULT_BAR_HEIGHT 100
#define DEFAULT_NAME "barcode"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <identificador> [opções]\n", argv[0]);
        return 1;
    }

    char *identifier = argv[1];
    if (!validate_ean8(identifier)) {
        fprintf(stderr, "Identificador EAN-8 inválido.\n");
        return 1;
    }

    //int width = (argc > 2) ? atoi(argv[2]) : DEFAULT_WIDTH;
    int spacing = (argc > 2) ? atoi(argv[2]) : DEFAULT_SPACING;
    int bar_width = (argc > 3) ? atoi(argv[3]) : DEFAULT_BAR_WIDTH;
    int bar_height = (argc > 4) ? atoi(argv[4]) : DEFAULT_BAR_HEIGHT;
    char *name = (argc > 5) ? argv[5] : DEFAULT_NAME;

    int width = ((bar_width*67) + (spacing * 2));
    int height = (bar_height + (spacing * 2));
    char *format = "P1";

    PBMImage image = {format, width, height, calloc(width * height * sizeof(char), 1)};
    memset(image.data, '0', (width * height * sizeof(char)));

    if (!image.data) {
        fprintf(stderr, "Erro ao alocar memória para a imagem.\n");
        return 1;
    }

    char bars[67]; // Número total de barras no EAN-8
    ean8_to_bars(identifier, bars);

    // Adiciona espaçamento à esquerda
    int x_offset = spacing; //DEFAULT_SPACING;

    for (int i = 0; i < 67; i++) {
        if (bars[i] == '1') { // Se a barra estiver ativa (1)
            // Desenha a barra no formato PBM
            for (int j = 0; j < bar_width; j++) {
                // Para cada posição horizontal da barra
                for (int k = spacing; k < bar_height + spacing; k++) {
                    int pixel_index = (k * width + (x_offset + j));
                    image.data[pixel_index] = '1';
                }
            }
        }
        // Move para a próxima barra
        x_offset += bar_width;
    }

    if (!save_pbm(name, &image)) {
        fprintf(stderr, "Erro ao salvar a imagem.\n");
        free_pbm(&image);
        return 1;
    }

    free_pbm(&image);
    return 0;
}
