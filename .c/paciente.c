/* paciente.c - Cadastro e classificacao de pacientes.
 *
 * Regras didaticas da atividade:
 *   Urgente      : idade superior a 80 anos
 *   Preferencial : gestante, lactante, necessidade especial, obesidade,
 *                  mulher com 60+ anos ou homem com 65+ anos
 *   Normal       : demais pacientes
 */
#include "paciente.h"
#include "entrada.h"

#define IDADE_MAXIMA 120

static const char PREFIXOS[NUM_CATEGORIAS] = { 'U', 'P', 'N' };
static const char *NOMES[NUM_CATEGORIAS] = { "Urgente", "Preferencial", "Normal" };

Paciente cadastrarPaciente(void)
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

Categoria classificarPaciente(const Paciente *p)
{
    if (p->idade > 80)
        return URGENTE;
    if (p->gestante || p->lactante || p->necessidadeEspecial || p->obeso ||
        (p->genero == 'F' && p->idade >= 60) ||
        (p->genero == 'M' && p->idade >= 65))
        return PREFERENCIAL;
    return NORMAL;
}

const char *nomeCategoria(Categoria c)
{
    return NOMES[c];
}

char prefixoCategoria(Categoria c)
{
    return PREFIXOS[c];
}
