# Barcode ITP - Gerador e Leitor de Códigos de Barras EAN-8

## Descrição

O projeto **Barcode ITP** consiste no desenvolvimento de dois programas em C:

1. **Gerador de Código de Barras EAN-8**: Gera uma imagem no formato PBM com um código de barras EAN-8 baseado em um identificador fornecido.
2. **Leitor de Código de Barras EAN-8**: Extrai o identificador a partir de uma imagem PBM contendo um código de barras EAN-8.

O projeto inclui um **código compartilhado** entre os dois programas, permitindo a reutilização de funções e structs para manipulação de imagens, valores e outras funcionalidades essenciais.

## Estrutura do Projeto

A estrutura do projeto é organizada da seguinte forma:

- **generate.c**: Contém a função `main()` para gerar a imagem com o código de barras.
- **extract.c**: Contém a função `main()` para ler o identificador do código de barras a partir de uma imagem.
- **ean8_utils.c** e **ean8_utils.h**: Arquivos com funções e structs compartilhadas para manipulação de imagens e outros processos comuns entre os dois programas.
- **Makefile**: Arquivo para automatizar o processo de compilação dos programas.

## Como Usar

### Compilação

1 - Para compilar os programas, utilize o **Makefile** incluído no projeto. O Makefile irá compilar tanto o programa de geração de código de barras quanto o de leitura de código de barras com um único comando.

2 - Após a compilação, você pode executar o programa generate para gerar o código de barras. Você será solicitado a fornecer um identificador (7 dígitos úteis), e o programa gerará uma imagem no formato PBM com o código de barras correspondente.
Código de barras gerado: barcode.pbm

3 - Após gerar a imagem com o código de barras, execute o programa extract para ler o identificador a partir da imagem gerada. Você deve fornecer o nome da imagem PBM gerada anteriormente, e o programa irá extrair o identificador (7 dígitos úteis).

Execute o seguinte comando para compilar os programas:

```bash
make

./generate (7 digitos)

./extract barcode.pbm 
```

### Autoria
Disciplina: Introdução às Técnicas de Programação - 2024.2

Discente: Fernanda Vitoria Silva do Nascimento

20 de janeiro de 2025