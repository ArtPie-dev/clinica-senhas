/* tipos.h - Tipos de dados e constantes compartilhados por todos os modulos. */
#ifndef TIPOS_H
#define TIPOS_H

#define CAPACIDADE_FILA 100   /* maximo de pacientes aguardando por categoria */
#define NUM_CATEGORIAS 3

/* A ordem do enum e a ordem de prioridade de chamada. */
typedef enum { URGENTE = 0, PREFERENCIAL = 1, NORMAL = 2 } Categoria;

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

#endif
