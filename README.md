# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Projeto da avaliação de **Algoritmos e Pensamento Computacional** (Questão 02), desenvolvido em **linguagem C (padrão C99)**.

## 1. Objetivo

Informatizar a triagem e a organização das filas de uma clínica de especialidades médicas: cadastrar pacientes, classificá-los automaticamente em categorias de prioridade, emitir senhas sequenciais e controlar a ordem de chamada e de atendimento.

> **Aviso:** as categorias e faixas etárias são regras didáticas definidas pela atividade. Não reproduzem a legislação de atendimento prioritário nem substituem a avaliação clínica de risco.

## 2. Funcionalidades

- Cadastro do paciente: gênero, idade, gestante (somente se feminino), lactante (somente se feminino), necessidade especial e obesidade.
- Classificação automática e emissão de senha sequencial **independente por categoria** (`N001, N002…`, `P001, P002…`, `U001, U002…`).
- Consulta da próxima senha a ser chamada.
- Chamada do paciente e identificação da senha em atendimento.
- Finalização do atendimento com atualização da fila.
- Painel textual: senha atual, categoria, próximas senhas e quantidade em espera por categoria.
- Validação de todas as entradas (texto no lugar de número, valores fora do intervalo, respostas diferentes de S/N etc.) e tratamento de operações inválidas (chamar com fila vazia, chamar com atendimento em andamento, finalizar sem atendimento).

### Regras de classificação

| Código | Categoria    | Condição                                                                                                              |
|--------|--------------|-----------------------------------------------------------------------------------------------------------------------|
| U001   | Urgente      | Idade **superior a 80** anos                                                                                          |
| P001   | Preferencial | Gestante, lactante, necessidade especial, obesidade, mulher com **60+** anos ou homem com **65+** anos                |
| N001   | Normal       | Sem nenhuma condição de prioridade                                                                                    |

### Regras de chamada

1. Urgentes antes das preferenciais; preferenciais antes das normais.
2. Dentro de cada categoria, ordem de emissão (**FIFO**).
3. Senhas já atendidas não retornam à fila.
4. Vários pacientes podem ser cadastrados durante a execução.

## 3. Estruturas de dados utilizadas

- `struct Paciente`: dados informados no cadastro (definida em `tipos.h`, assim como as demais estruturas).
- `struct Senha`: código, categoria e paciente.
- `struct Fila`: **fila circular** de capacidade fixa (100 senhas aguardando por categoria), com `inicio` e `quantidade`.
- `struct Sistema`: três filas (uma por categoria), contadores de senha, senha atual e total de atendidos.
- `enum Categoria`: `URGENTE`, `PREFERENCIAL`, `NORMAL`. A ordem do enum é a ordem de prioridade.

## 4. Compilação e execução

Requisito: compilador C (GCC, MinGW ou Clang). O projeto tem vários arquivos, então todos os `.c` precisam ser compilados juntos.

**Opção A – direto com o GCC (funciona em qualquer sistema):**

```bash
gcc -std=c99 -Wall -Wextra -pedantic -o clinica main.c entrada.c paciente.c fila.c atendimento.c painel.c
```

**Opção B – com o Makefile (Linux, macOS ou MinGW com `make`):**

```bash
make          # compila
make clean    # remove os arquivos gerados
```

**Execução:**

```bash
./clinica          # Linux / macOS
clinica.exe        # Windows
```

Para finalizar, use a opção `0` do menu.

> As mensagens do programa não usam acentos de propósito, para evitar caracteres quebrados em terminais Windows.

## 5. Organização em módulos e funções implementadas

O programa é dividido em módulos (pares `.h`/`.c`), cada um com uma responsabilidade. A `main()` apenas coordena o fluxo: mostra o menu, lê a opção e chama a função correspondente.

| Módulo | Responsabilidade |
|---|---|
| `tipos.h` | Constantes e estruturas de dados compartilhadas (`Paciente`, `Senha`, `Fila`, `Sistema`, `Categoria`) |
| `entrada.c/.h` | Leitura e validação das entradas do usuário |
| `paciente.c/.h` | Cadastro do paciente e classificação em categorias |
| `fila.c/.h` | Fila circular FIFO de senhas |
| `atendimento.c/.h` | Emissão de senhas e controle do atendimento |
| `painel.c/.h` | Apresentação do menu e do painel de chamada |
| `main.c` | Fluxo principal (menu e `switch`) |

### Funções por módulo

| Módulo | Função | Responsabilidade |
|---|---|---|
| `main.c` | `main` | Coordena o fluxo: exibe o menu, lê a opção e chama a função correspondente |
| `entrada.c` | `lerLinha` | Lê uma linha com segurança (`fgets`), descarta o excesso e trata fim de entrada |
| `entrada.c` | `lerInteiro` | Lê e valida um inteiro dentro de um intervalo |
| `entrada.c` | `lerSimNao` | Lê e valida resposta S/N |
| `entrada.c` | `lerGenero` | Lê e valida gênero M/F |
| `paciente.c` | `cadastrarPaciente` | Coleta os dados do paciente |
| `paciente.c` | `classificarPaciente` | Define a categoria (urgente, preferencial ou normal) |
| `paciente.c` | `nomeCategoria` / `prefixoCategoria` | Devolvem o nome ("Urgente") e a letra (`U`) de uma categoria |
| `fila.c` | `filaVazia` / `filaCheia` | Verificam o estado de uma fila |
| `fila.c` | `enfileirar` / `desenfileirar` | Inserem e removem senhas (FIFO) |
| `fila.c` | `itemDaFila` | Consulta uma posição da fila sem removê-la |
| `atendimento.c` | `inicializarSistema` | Zera filas, contadores e estado |
| `atendimento.c` | `proximaCategoria` | Aplica a precedência U > P > N para achar a próxima fila |
| `atendimento.c` | `totalEmEspera` | Soma os pacientes em espera |
| `atendimento.c` | `gerarSenha` | Emite a senha sequencial e a insere na fila da categoria |
| `atendimento.c` | `registrarSenha` | Operação do menu: cadastra o paciente e emite a senha |
| `atendimento.c` | `consultarProxima` | Mostra a próxima senha sem retirá-la da fila |
| `atendimento.c` | `chamarProximo` | Retira a próxima senha da fila e a coloca em atendimento |
| `atendimento.c` | `identificarEmAtendimento` | Mostra a senha em atendimento |
| `atendimento.c` | `finalizarAtendimento` | Conclui o atendimento e informa a próxima senha |
| `painel.c` | `exibirMenu` | Mostra o menu de opções |
| `painel.c` | `exibirPainel` | Mostra o painel de chamada |

## 6. Exemplos de utilização

### Emissão de senhas

```
Escolha uma opcao: 1
Genero do paciente (M/F): M
Idade: 30
Possui necessidade especial? (S/N): N
Possui condicao de obesidade? (S/N): N

>> Senha emitida: N001 (Normal)

Escolha uma opcao: 1
Genero do paciente (M/F): F
Idade: 70
Esta gestante? (S/N): N
E lactante? (S/N): N
Possui necessidade especial? (S/N): N
Possui condicao de obesidade? (S/N): N

>> Senha emitida: P001 (Preferencial)

Escolha uma opcao: 1
Genero do paciente (M/F): F
Idade: 85
Esta gestante? (S/N): N
E lactante? (S/N): N
Possui necessidade especial? (S/N): N
Possui condicao de obesidade? (S/N): N

>> Senha emitida: U001 (Urgente)
```

### Painel e chamada (U001 chamada antes de P001 e N001, mesmo emitida por último)

```
=========== PAINEL DE CHAMADA ===========
 Senha chamada : -- (nenhuma)
 Proximas      : U001 P001 N001

 Em espera por categoria:
   Urgente      : 1
   Preferencial : 1
   Normal       : 1
   Total em espera: 3 | Atendidos: 0
=========================================

Escolha uma opcao: 3

>> CHAMANDO senha U001 (Urgente)

Escolha uma opcao: 3

>> A senha U001 ainda esta em atendimento. Finalize-a antes de chamar outra.

Escolha uma opcao: 5

>> Atendimento da senha U001 finalizado.

>> Proxima senha a ser chamada: P001 (Preferencial)
```

### Validação de entradas

```
Genero do paciente (M/F): X
  Genero invalido. Digite M ou F.
Idade: abc
  Entrada invalida. Digite um numero inteiro.
Idade: 200
  Valor fora do intervalo (0 a 120).
```

## 7. Casos de teste realizados

| Entrada | Resultado esperado | Resultado |
|---|---|---|
| Homem, 30 anos, sem condições | `N001` | OK |
| Mulher, 70 anos | `P001` (mulher 60+) | OK |
| Homem, 64 anos, sem condições | Normal | OK |
| Mulher, 59 anos, sem condições | Normal | OK |
| Mulher, 60 anos | Preferencial | OK |
| Homem, 65 anos | Preferencial | OK |
| Mulher gestante, 25 anos | Preferencial | OK |
| Qualquer paciente com 81 anos | `U001` | OK |
| Mulher com exatamente 80 anos | Preferencial (não é urgente: a regra exige superior a 80) | OK |
| Homem com exatamente 80 anos | Preferencial (65+), não urgente | OK |
| Chamar com a fila vazia | Mensagem de aviso | OK |
| Chamar com atendimento em andamento | Bloqueado | OK |
| Finalizar sem atendimento | Mensagem de aviso | OK |
| Idade `abc`, `200`, gênero `X`, opção `9` | Nova solicitação com mensagem de erro | OK |
| 101 pacientes normais | A 101ª senha é recusada (fila cheia, limite de 100) | OK |
| Numeração por categoria | `N001, N002`, `P001, P002`, `U001, U002` independentes | OK |

## 8. Estrutura do repositório

```
.
├── main.c
├── tipos.h
├── entrada.c / entrada.h
├── paciente.c / paciente.h
├── fila.c / fila.h
├── atendimento.c / atendimento.h
├── painel.c / painel.h
├── Makefile
├── README.md
└── .gitignore
```

## Autor

Arthur Pierre de Aguiar
