/* atendimento.h - Emissao de senhas e controle do atendimento. */
#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H

#include "tipos.h"

void inicializarSistema(Sistema *s);

/* Categoria da proxima senha a ser chamada (U > P > N), ou -1 se nao ha ninguem. */
int proximaCategoria(const Sistema *s);

/* Total de pacientes aguardando, somando as tres filas. */
int totalEmEspera(const Sistema *s);

/* Emite a senha sequencial da categoria do paciente e a coloca na fila.
   Devolve 0 se a fila da categoria estiver cheia. */
int gerarSenha(Sistema *s, Paciente p, Senha *emitida);

/* Operacoes disponiveis no menu */
void registrarSenha(Sistema *s);
void consultarProxima(const Sistema *s);
void chamarProximo(Sistema *s);
void identificarEmAtendimento(const Sistema *s);
void finalizarAtendimento(Sistema *s);

#endif
