/* fila.h - Fila circular FIFO de senhas. */
#ifndef FILA_H
#define FILA_H

#include "tipos.h"

int filaVazia(const Fila *f);
int filaCheia(const Fila *f);

/* Insere no fim da fila. Devolve 1 se conseguiu, 0 se a fila estava cheia. */
int enfileirar(Fila *f, Senha senha);

/* Remove do inicio da fila e grava em *saida. Devolve 0 se estava vazia. */
int desenfileirar(Fila *f, Senha *saida);

/* Item na posicao pos (0 = primeiro da fila), sem remove-lo. NULL se invalida. */
const Senha *itemDaFila(const Fila *f, int pos);

#endif
