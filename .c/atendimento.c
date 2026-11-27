/* atendimento.c - Emissao de senhas e controle do atendimento. */
#include <stdio.h>
#include <string.h>
#include "atendimento.h"
#include "paciente.h"
#include "fila.h"

void inicializarSistema(Sistema *s)
{
    memset(s, 0, sizeof *s);
}

int proximaCategoria(const Sistema *s)
{
    int c;
    for (c = 0; c < NUM_CATEGORIAS; c++)
        if (!filaVazia(&s->filas[c])) return c;
    return -1;
}

int totalEmEspera(const Sistema *s)
{
    int c, total = 0;
    for (c = 0; c < NUM_CATEGORIAS; c++) total += s->filas[c].quantidade;
    return total;
}

int gerarSenha(Sistema *s, Paciente p, Senha *emitida)
{
    Categoria cat = classificarPaciente(&p);

    if (filaCheia(&s->filas[cat])) return 0;
    s->contadores[cat]++;
    emitida->categoria = cat;
    emitida->paciente = p;
    snprintf(emitida->codigo, sizeof emitida->codigo, "%c%03d",
             prefixoCategoria(cat), s->contadores[cat]);
    enfileirar(&s->filas[cat], *emitida);
    return 1;
}

void registrarSenha(Sistema *s)
{
    Paciente p = cadastrarPaciente();
    Senha senha;

    if (gerarSenha(s, p, &senha)) {
        printf("\n>> Senha emitida: %s (%s)\n", senha.codigo, nomeCategoria(senha.categoria));
    } else {
        printf("\n>> Fila %s cheia (limite de %d). Senha nao emitida.\n",
               nomeCategoria(classificarPaciente(&p)), CAPACIDADE_FILA);
    }
}

void consultarProxima(const Sistema *s)
{
    int c = proximaCategoria(s);

    if (c < 0) {
        printf("\n>> Nao ha pacientes aguardando.\n");
        return;
    }
    printf("\n>> Proxima senha a ser chamada: %s (%s)\n",
           itemDaFila(&s->filas[c], 0)->codigo, nomeCategoria((Categoria)c));
}

void chamarProximo(Sistema *s)
{
    int c;

    if (s->emAtendimento) {
        printf("\n>> A senha %s ainda esta em atendimento. Finalize-a antes de chamar outra.\n",
               s->atual.codigo);
        return;
    }
    c = proximaCategoria(s);
    if (c < 0) {
        printf("\n>> Nao ha pacientes aguardando.\n");
        return;
    }
    desenfileirar(&s->filas[c], &s->atual);
    s->emAtendimento = 1;
    printf("\n>> CHAMANDO senha %s (%s)\n", s->atual.codigo, nomeCategoria((Categoria)c));
}

void identificarEmAtendimento(const Sistema *s)
{
    if (!s->emAtendimento)
        printf("\n>> Nenhuma senha em atendimento no momento.\n");
    else
        printf("\n>> Em atendimento: %s (%s)\n",
               s->atual.codigo, nomeCategoria(s->atual.categoria));
}

void finalizarAtendimento(Sistema *s)
{
    if (!s->emAtendimento) {
        printf("\n>> Nao ha atendimento em andamento para finalizar.\n");
        return;
    }
    printf("\n>> Atendimento da senha %s finalizado.\n", s->atual.codigo);
    s->emAtendimento = 0;
    s->totalAtendidos++;
    /* A senha atendida ja saiu da fila ao ser chamada e nao pode retornar.
       Mostramos quem e o proximo. */
    consultarProxima(s);
}
