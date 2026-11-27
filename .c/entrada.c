/* entrada.c - Leitura e validacao de dados digitados pelo usuario. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "entrada.h"

#define TAM_LINHA 64

void lerLinha(char *buffer, int tamanho)
{
    size_t len;
    int c;

    if (fgets(buffer, tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Finalizando o sistema.\n");
        exit(0);
    }
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) { /* descarta o excedente */ }
    }
}

int lerInteiro(const char *mensagem, int minimo, int maximo)
{
    char linha[TAM_LINHA];
    char *fim;
    long valor;

    for (;;) {
        printf("%s", mensagem);
        lerLinha(linha, sizeof linha);
        valor = strtol(linha, &fim, 10);
        if (fim == linha) {
            printf("  Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }
        while (isspace((unsigned char)*fim)) fim++;
        if (*fim != '\0') {
            printf("  Entrada invalida. Digite apenas numeros.\n");
            continue;
        }
        if (valor < minimo || valor > maximo) {
            printf("  Valor fora do intervalo (%d a %d).\n", minimo, maximo);
            continue;
        }
        return (int)valor;
    }
}

int lerSimNao(const char *mensagem)
{
    char linha[TAM_LINHA];

    for (;;) {
        printf("%s (S/N): ", mensagem);
        lerLinha(linha, sizeof linha);
        if (strlen(linha) == 1) {
            char c = (char)toupper((unsigned char)linha[0]);
            if (c == 'S') return 1;
            if (c == 'N') return 0;
        }
        printf("  Resposta invalida. Digite S ou N.\n");
    }
}

char lerGenero(void)
{
    char linha[TAM_LINHA];

    for (;;) {
        printf("Genero do paciente (M/F): ");
        lerLinha(linha, sizeof linha);
        if (strlen(linha) == 1) {
            char c = (char)toupper((unsigned char)linha[0]);
            if (c == 'M' || c == 'F') return c;
        }
        printf("  Genero invalido. Digite M ou F.\n");
    }
}
