/*
 * Sistema de Gerenciamento de Senhas de Atendimento Clinico
 * Avaliacao - Algoritmos e Pensamento Computacional (Questao 02)
 *
 * A main() apenas coordena o fluxo: mostra o menu, le a opcao e delega
 * o trabalho aos modulos (entrada, paciente, fila, atendimento e painel).
 *
 * Compilar: make   (ou veja o README)
 */
#include <stdio.h>
#include "tipos.h"
#include "entrada.h"
#include "atendimento.h"
#include "painel.h"

int main(void)
{
    Sistema sistema;
    int opcao;

    inicializarSistema(&sistema);
    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0, 6);
        switch (opcao) {
            case 1: registrarSenha(&sistema);           break;
            case 2: consultarProxima(&sistema);         break;
            case 3: chamarProximo(&sistema);            break;
            case 4: identificarEmAtendimento(&sistema); break;
            case 5: finalizarAtendimento(&sistema);     break;
            case 6: exibirPainel(&sistema);             break;
            case 0:
                if (sistema.emAtendimento || totalEmEspera(&sistema) > 0)
                    printf("\nAtencao: ainda ha senhas pendentes.\n");
                printf("Encerrando o sistema. Ate logo!\n");
                break;
        }
    } while (opcao != 0);
    return 0;
}
