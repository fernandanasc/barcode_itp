#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

PBMImage *load_pbm(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
        return NULL;
    }

    char format[3];
    if (fscanf(file, "%2s", format) != 1 || strcmp(format, "P1") != 0) {
        fprintf(stderr, "Arquivo não é um PBM válido.\n");
        fclose(file);
        return NULL;
    }

    PBMImage *image = malloc(sizeof(PBMImage));
    if (!image) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d", &image->width, &image->height) != 2) {
        fprintf(stderr, "Erro ao ler dimensões da imagem.\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fgetc(file); // Consumir o caractere de nova linha após o cabeçalho

    size_t data_size = image->width * image->height;
    image->data = calloc(image->width * image->height * sizeof(char), 1);//malloc(data_size); calloc(width * height * sizeof(char), 1)
    if (!image->data) {
        fprintf(stderr, "Erro ao alocar memória para os dados.\n");
        free(image);
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(image->data, 1, data_size, file);
    if (bytes_read != data_size) {
        fprintf(stderr, "Erro ao ler os dados da imagem.\n");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return image;
}

bool save_pbm(const char *filename, const PBMImage *image) {

    char file_with_extension[256];
    strncpy(file_with_extension, filename, sizeof(file_with_extension) - 1);
    file_with_extension[sizeof(file_with_extension) - 1] = '\0';  // Garante a terminação da string

    // Verifica se o nome já termina com ".pbm", se não, adiciona a extensão
    if (strlen(file_with_extension) < 4 || strcmp(file_with_extension + strlen(file_with_extension) - 4, ".pbm") != 0) {
        strcat(file_with_extension, ".pbm");
    }

     // Verifica se o arquivo já existe
    FILE *file_check = fopen(file_with_extension, "r");
    if (file_check) {
        // O arquivo já existe, pergunta ao usuário se deseja sobrescrever
        fclose(file_check);  // Fecha o arquivo de verificação

        char response[10];
        printf("O arquivo %s já existe. Deseja sobrescrevê-lo? (sim/nao): ", file_with_extension);
        scanf("%s", response);

        // Converte a resposta para minúsculas
        for (int i = 0; response[i]; i++) {
            response[i] = tolower(response[i]);
        }

        if (strcmp(response, "sim") != 0) {
            // Se a resposta não for "sim", mostra a mensagem de erro e sai
            printf("Arquivo resultante já existe. Operação abortada.\n");
            return false;
        }
    }

    FILE *file = fopen(file_with_extension, "wb");
    if (!file) {
        fprintf(stderr, "Erro ao criar o arquivo %s.\n", filename);
        return false;
    }

    fprintf(file, "%s\n%d %d\n", image->format, image->width, image->height);
    size_t data_size = image->width * image->height * sizeof(char);
    fwrite(image->data, 1, data_size, file);
    fclose(file);
    return true;
}

void free_pbm(PBMImage *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}
