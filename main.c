/*
 * Sistema de Gerenciamento de Senhas de Atendimento Clinico
 * Avaliacao - Algoritmos e Pensamento Computacional (Questao 02)
 *
 * Categorias (regras didaticas da atividade):
 *   U - Urgente      : idade superior a 80 anos
 *   P - Preferencial : gestante, lactante, necessidade especial, obesidade,
 *                      mulher com 60+ anos ou homem com 65+ anos
 *   N - Normal       : demais pacientes
 *
 * Ordem de chamada: Urgente > Preferencial > Normal; FIFO dentro de cada
 * categoria.
 *
 * Compilar: gcc -std=c99 -Wall -Wextra -o clinica main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CAPACIDADE_FILA 100   /* maximo de pacientes aguardando por categoria */
#define TAM_LINHA 64
#define IDADE_MAXIMA 120
#define PROXIMAS_NO_PAINEL 5

/* A ordem do enum e a ordem de prioridade de chamada. */
typedef enum { URGENTE = 0, PREFERENCIAL = 1, NORMAL = 2 } Categoria;
#define NUM_CATEGORIAS 3

static const char PREFIXOS[NUM_CATEGORIAS] = { 'U', 'P', 'N' };
static const char *NOMES[NUM_CATEGORIAS] = { "Urgente", "Preferencial", "Normal" };

typedef struct {
    char genero;               /* 'M' ou 'F' */
    int idade;
    int gestante;              /* 1 = sim, 0 = nao */
    int lactante;
    int necessidadeEspecial;
    int obeso;
} Paciente;

typedef struct {
    char codigo[8];            /* ex.: P001 */
    Categoria categoria;
    Paciente paciente;
} Senha;

/* Fila circular FIFO com capacidade fixa. */
typedef struct {
    Senha itens[CAPACIDADE_FILA];
    int inicio;
    int quantidade;
} Fila;

typedef struct {
    Fila filas[NUM_CATEGORIAS];
    int contadores[NUM_CATEGORIAS];  /* ultimo numero emitido por categoria */
    Senha atual;
    int emAtendimento;               /* 1 se ha paciente sendo atendido */
    int totalAtendidos;
} Sistema;

/* ----------------------- Entrada e validacao ----------------------- */

static void lerLinha(char *buffer, size_t tamanho)
{
    size_t len;
    int c;

    if (fgets(buffer, (int)tamanho, stdin) == NULL) {
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

static int lerInteiro(const char *mensagem, int minimo, int maximo)
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

static int lerSimNao(const char *mensagem)
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

static char lerGenero(void)
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

/* ----------------------- Cadastro e classificacao ----------------------- */

static Paciente cadastrarPaciente(void)
{
    Paciente p;

    p.genero = lerGenero();
    p.idade = lerInteiro("Idade: ", 0, IDADE_MAXIMA);
    p.gestante = 0;
    p.lactante = 0;
    if (p.genero == 'F') {          /* perguntas aplicaveis apenas a mulheres */
        p.gestante = lerSimNao("Esta gestante?");
        p.lactante = lerSimNao("E lactante?");
    }
    p.necessidadeEspecial = lerSimNao("Possui necessidade especial?");
    p.obeso = lerSimNao("Possui condicao de obesidade?");
    return p;
}

static Categoria classificarPaciente(const Paciente *p)
{
    if (p->idade > 80)
        return URGENTE;
    if (p->gestante || p->lactante || p->necessidadeEspecial || p->obeso ||
        (p->genero == 'F' && p->idade >= 60) ||
        (p->genero == 'M' && p->idade >= 65))
        return PREFERENCIAL;
    return NORMAL;
}

/* ----------------------- Gerenciamento das filas ----------------------- */

static void inicializarSistema(Sistema *s)
{
    memset(s, 0, sizeof *s);
}

static int filaVazia(const Fila *f)  { return f->quantidade == 0; }
static int filaCheia(const Fila *f)  { return f->quantidade == CAPACIDADE_FILA; }

static int enfileirar(Fila *f, Senha senha)
{
    if (filaCheia(f)) return 0;
    f->itens[(f->inicio + f->quantidade) % CAPACIDADE_FILA] = senha;
    f->quantidade++;
    return 1;
}

static int desenfileirar(Fila *f, Senha *saida)
{
    if (filaVazia(f)) return 0;
    *saida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
    f->quantidade--;
    return 1;
}

/* Retorna a categoria da proxima senha a ser chamada, ou -1 se nao ha ninguem. */
static int proximaCategoria(const Sistema *s)
{
    int c;
    for (c = 0; c < NUM_CATEGORIAS; c++)
        if (!filaVazia(&s->filas[c])) return c;
    return -1;
}

static int totalEmEspera(const Sistema *s)
{
    int c, total = 0;
    for (c = 0; c < NUM_CATEGORIAS; c++) total += s->filas[c].quantidade;
    return total;
}

/* Emite a senha sequencial da categoria do paciente e a coloca na fila. */
static int gerarSenha(Sistema *s, Paciente p, Senha *emitida)
{
    Categoria cat = classificarPaciente(&p);

    if (filaCheia(&s->filas[cat])) return 0;
    s->contadores[cat]++;
    emitida->categoria = cat;
    emitida->paciente = p;
    sprintf(emitida->codigo, "%c%03d", PREFIXOS[cat], s->contadores[cat]);
    enfileirar(&s->filas[cat], *emitida);
    return 1;
}

/* ----------------------- Operacoes do atendimento ----------------------- */

static void registrarSenha(Sistema *s)
{
    Paciente p = cadastrarPaciente();
    Senha senha;

    if (gerarSenha(s, p, &senha)) {
        printf("\n>> Senha emitida: %s (%s)\n", senha.codigo, NOMES[senha.categoria]);
    } else {
        printf("\n>> Fila %s cheia (limite de %d). Senha nao emitida.\n",
               NOMES[classificarPaciente(&p)], CAPACIDADE_FILA);
    }
}

static void consultarProxima(const Sistema *s)
{
    int c = proximaCategoria(s);

    if (c < 0) {
        printf("\n>> Nao ha pacientes aguardando.\n");
        return;
    }
    printf("\n>> Proxima senha a ser chamada: %s (%s)\n",
           s->filas[c].itens[s->filas[c].inicio].codigo, NOMES[c]);
}

static void chamarProximo(Sistema *s)
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
    printf("\n>> CHAMANDO senha %s (%s)\n", s->atual.codigo, NOMES[c]);
}

static void identificarEmAtendimento(const Sistema *s)
{
    if (!s->emAtendimento)
        printf("\n>> Nenhuma senha em atendimento no momento.\n");
    else
        printf("\n>> Em atendimento: %s (%s)\n",
               s->atual.codigo, NOMES[s->atual.categoria]);
}

static void finalizarAtendimento(Sistema *s)
{
    if (!s->emAtendimento) {
        printf("\n>> Nao ha atendimento em andamento para finalizar.\n");
        return;
    }
    printf("\n>> Atendimento da senha %s finalizado.\n", s->atual.codigo);
    s->emAtendimento = 0;
    s->totalAtendidos++;
    /* A fila ja esta atualizada: a senha atendida saiu dela ao ser chamada
       e nao pode retornar. Mostramos quem e o proximo. */
    consultarProxima(s);
}

/* ----------------------- Painel e menu ----------------------- */

static void exibirPainel(const Sistema *s)
{
    int c, i, mostradas = 0;

    printf("\n=========== PAINEL DE CHAMADA ===========\n");
    if (s->emAtendimento)
        printf(" Senha chamada : %s\n Categoria     : %s\n",
               s->atual.codigo, NOMES[s->atual.categoria]);
    else
        printf(" Senha chamada : -- (nenhuma)\n");

    printf(" Proximas      :");
    for (c = 0; c < NUM_CATEGORIAS && mostradas < PROXIMAS_NO_PAINEL; c++) {
        const Fila *f = &s->filas[c];
        for (i = 0; i < f->quantidade && mostradas < PROXIMAS_NO_PAINEL; i++) {
            printf(" %s", f->itens[(f->inicio + i) % CAPACIDADE_FILA].codigo);
            mostradas++;
        }
    }
    if (mostradas == 0) printf(" (fila vazia)");
    printf("\n\n Em espera por categoria:\n");
    for (c = 0; c < NUM_CATEGORIAS; c++)
        printf("   %-13s: %d\n", NOMES[c], s->filas[c].quantidade);
    printf("   Total em espera: %d | Atendidos: %d\n", totalEmEspera(s), s->totalAtendidos);
    printf("=========================================\n");
}

static void exibirMenu(void)
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

int main(void)
{
    Sistema sistema;
    int opcao;

    inicializarSistema(&sistema);
    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0, 6);
        switch (opcao) {
            case 1: registrarSenha(&sistema);          break;
            case 2: consultarProxima(&sistema);        break;
            case 3: chamarProximo(&sistema);           break;
            case 4: identificarEmAtendimento(&sistema);break;
            case 5: finalizarAtendimento(&sistema);    break;
            case 6: exibirPainel(&sistema);            break;
            case 0:
                if (sistema.emAtendimento || totalEmEspera(&sistema) > 0)
                    printf("\nAtencao: ainda ha senhas pendentes.\n");
                printf("Encerrando o sistema. Ate logo!\n");
                break;
        }
    } while (opcao != 0);
    return 0;
}
