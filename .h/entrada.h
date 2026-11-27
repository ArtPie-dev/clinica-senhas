/* entrada.h - Leitura e validacao de dados digitados pelo usuario. */
#ifndef ENTRADA_H
#define ENTRADA_H

/* Le uma linha com seguranca; encerra o programa se a entrada acabar. */
void lerLinha(char *buffer, int tamanho);

/* Le um inteiro no intervalo [minimo, maximo], repetindo ate ser valido. */
int lerInteiro(const char *mensagem, int minimo, int maximo);

/* Le S/N e devolve 1 (sim) ou 0 (nao). */
int lerSimNao(const char *mensagem);

/* Le o genero e devolve 'M' ou 'F'. */
char lerGenero(void);

#endif
