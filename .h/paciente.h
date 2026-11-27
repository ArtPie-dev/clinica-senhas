/* paciente.h - Cadastro e classificacao de pacientes. */
#ifndef PACIENTE_H
#define PACIENTE_H

#include "tipos.h"

/* Coleta do usuario os dados do paciente (com validacao). */
Paciente cadastrarPaciente(void);

/* Define a categoria: urgente, preferencial ou normal. */
Categoria classificarPaciente(const Paciente *p);

/* Nome da categoria, ex.: "Urgente". */
const char *nomeCategoria(Categoria c);

/* Letra usada no codigo da senha: 'U', 'P' ou 'N'. */
char prefixoCategoria(Categoria c);

#endif
