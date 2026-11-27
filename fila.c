/* fila.c - Fila circular FIFO de senhas. */
#include <stddef.h>
#include "fila.h"

int filaVazia(const Fila *f)
{
    return f->quantidade == 0;
}

int filaCheia(const Fila *f)
{
    return f->quantidade == CAPACIDADE_FILA;
}

int enfileirar(Fila *f, Senha senha)
{
    if (filaCheia(f)) return 0;
    f->itens[(f->inicio + f->quantidade) % CAPACIDADE_FILA] = senha;
    f->quantidade++;
    return 1;
}

int desenfileirar(Fila *f, Senha *saida)
{
    if (filaVazia(f)) return 0;
    *saida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
    f->quantidade--;
    return 1;
}

const Senha *itemDaFila(const Fila *f, int pos)
{
    if (pos < 0 || pos >= f->quantidade) return NULL;
    return &f->itens[(f->inicio + pos) % CAPACIDADE_FILA];
}
