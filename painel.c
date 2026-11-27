/* painel.c - Apresentacao do menu e do painel de chamada. */
#include <stdio.h>
#include "painel.h"
#include "atendimento.h"
#include "paciente.h"
#include "fila.h"

#define PROXIMAS_NO_PAINEL 5

void exibirMenu(void)
{
    printf("\n------------- CLINICA - SENHAS -------------\n");
    printf(" 1 - Registrar paciente / emitir senha\n");
    printf(" 2 - Consultar proxima senha\n");
    printf(" 3 - Chamar proximo paciente\n");
    printf(" 4 - Identificar senha em atendimento\n");
    printf(" 5 - Finalizar atendimento\n");
    printf(" 6 - Exibir painel de chamada\n");
    printf(" 0 - Sair\n");
    printf("--------------------------------------------\n");
}

void exibirPainel(const Sistema *s)
{
    int c, i, mostradas = 0;

    printf("\n=========== PAINEL DE CHAMADA ===========\n");
    if (s->emAtendimento)
        printf(" Senha chamada : %s\n Categoria     : %s\n",
               s->atual.codigo, nomeCategoria(s->atual.categoria));
    else
        printf(" Senha chamada : -- (nenhuma)\n");

    printf(" Proximas      :");
    for (c = 0; c < NUM_CATEGORIAS && mostradas < PROXIMAS_NO_PAINEL; c++) {
        const Fila *f = &s->filas[c];
        for (i = 0; i < f->quantidade && mostradas < PROXIMAS_NO_PAINEL; i++) {
            printf(" %s", itemDaFila(f, i)->codigo);
            mostradas++;
        }
    }
    if (mostradas == 0) printf(" (fila vazia)");
    printf("\n\n Em espera por categoria:\n");
    for (c = 0; c < NUM_CATEGORIAS; c++)
        printf("   %-13s: %d\n", nomeCategoria((Categoria)c), s->filas[c].quantidade);
    printf("   Total em espera: %d | Atendidos: %d\n", totalEmEspera(s), s->totalAtendidos);
    printf("=========================================\n");
}
