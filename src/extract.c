#include "ean8_utils.h"
#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>

int extract_bars(const PBMImage *image, char *bars, int num_bars, int pixel_width, int spacing) {
    char _bars[num_bars];

    int offset = spacing + spacing*image->width;
    for (int i = 0; i < num_bars; i++) {
        _bars[i] = image->data[i * pixel_width + offset];
    }
   
    for (int j = spacing; j < image->height - spacing; j++) {

        for (int i = 0; i < spacing; i++) {
            if (image->data[i + j * image->width] != '0') {
                return -1;
            }
        }

        int pos = spacing;

        for (int i = 0; i < num_bars; i++) {
            bars[i] = image->data[j * image->width + pos];
            
            if(bars[i] != _bars[i]) {
                return -1;
            }

            pos = pos + pixel_width;
        }

        for (int i = pos; i < image->width; i++) {
            if (image->data[j * image->width + i] != '0') {
                return -1;
            }
        }
    }

    return 0;
}

int get_spacing(PBMImage *image) {
    int spacing = 0;
    for (int j = 0; j < image->height; j++){
        for (int i = 0; i < image->width; i++){
            if(image->data[j*image->width + i] != '0') {
                return spacing;
            }
        }
        spacing++;
    }
    return -1;
}

int get_bar_width(int w, int s) {
    return (w - 2*s)/67;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.pbm>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    PBMImage *image = load_pbm(input_file);
    if (!image) {
        fprintf(stderr, "Erro: Arquivo inválido ou não encontrado.\n");
        return 1;
    }

    int spacing = get_spacing(image);
    
    if (spacing == -1) {
        fprintf(stderr, "Erro: Imagem não parece conter um código de barras válido.\n");
        free_pbm(image);
        return 1;
    }

    int bar_width = get_bar_width(image->width, spacing);

    if (image->width < bar_width * 67 + 2 * spacing) {
        fprintf(stderr, "Erro: Imagem não parece conter um código de barras válido.\n");
        free_pbm(image);
        return 1;
    }

    char bars[67];
   
    extract_bars(image, bars, 67, bar_width, spacing);
    
    // Reconstrói o identificador a partir das barras
    char identifier[9] = {'0'}; // 8 dígitos + terminador

    if (!decode_bars_to_ean8(bars, identifier)) {
        fprintf(stderr, "Erro: Não foi possível decodificar o identificador do código de barras.\n");
        free_pbm(image);
        return 1;
    }

    printf("Identificador extraído: %s\n", identifier);

    free_pbm(image);
    return 0;
}
