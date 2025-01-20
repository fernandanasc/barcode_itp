#ifndef PBM_UTILS_H
#define PBM_UTILS_H

#include <stdbool.h>

// Estrutura para armazenar imagens PBM
typedef struct {
    char *format;
    int width;
    int height;
    unsigned char *data;
} PBMImage;

// Carrega uma imagem PBM
PBMImage *load_pbm(const char *filename);

// Salva uma imagem PBM
bool save_pbm(const char *filename, const PBMImage *image);

// Libera a memória alocada para uma imagem PBM
void free_pbm(PBMImage *image);

#endif
