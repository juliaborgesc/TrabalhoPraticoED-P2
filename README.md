# Trabalho Prático da disciplina Estrutura de Dados — Parte II

A segunda parte deste trabalho tem como objetivo implementar um sistema de gerenciamento de pacientes de uma clínica utilizando a linguagem C e o TAD **LinkedList** para manipulação dinâmica dos dados. Os registros dos pacientes são armazenados em um arquivo CSV, que é carregado e salvo durante a execução do sistema.

O sistema permite realizar operações como consulta, inserção, atualização e remoção de pacientes, além da listagem de todos os registros.

---

## Estrutura do Projeto

O projeto está organizado nos seguintes arquivos:

- **"main.c"** — Contém o loop principal do programa e controle de fluxo;
- **"interface.h/c"** - Implementa todas as funções de interface com o usuário;
- **"paciente.h/c"** — Define a estrutura `Paciente`, além de funções auxiliares como exibição em tabela e formatação de CPF;
- **"bd_paciente.h/c"** — Implementa o TAD `BDPaciente`, que utiliza uma lista encadeada simples para armazenar e manipular os pacientes;
- **"bd_paciente.csv"** — Arquivo onde os dados dos pacientes são persistidos (ID, CPF, nome, idade e data de cadastro);
- **"Makefile"** — Define os comandos de compilação e execução do programa via terminal.

---

## TADs (Tipos Abstratos de Dados)

### "Paciente"
Representa os dados de um paciente individual.

```c
typedef struct {
    int id;
    char cpf[15];
    char nome[80];
    int idade;
    char data_cadastro[11]; 
} Paciente;
```

### "BdPaciente"
Representa o banco de dados e o nó da lista encadeada.

```c
typedef struct NodePaciente {
    Paciente paciente;
    struct NodePaciente* next;
} NodePaciente;

typedef struct {
    NodePaciente* first;  
    int tamanho;          
} BDPaciente;
```

## Funcionalidades implementadas

- Consulta

    - Por nome (busca por prefixo, case‑insensitive)

    - Por CPF (busca por prefixo)

- Inserção de novo paciente (com verificação de CPF duplicado)

- Atualização de dados de um paciente existente

- Remoção de paciente por ID (com confirmação do usuário)

- Listagem de todos os pacientes em formato de tabela

- Geração automática de IDs únicos via bd_gerar_id()

- Formatação automática de CPF e data

## Decisões de Implementação

- Utilização de lista encadeada simples para gerenciar dinamicamente os registros, conforme abordado em aula;

- A função bd_gerar_id() itera pela lista para encontrar o maior ID e retorna maior + 1, fazendo com que fique em ordem;

- Carregamento e salvamento do CSV ocorrendo apenas no início e no término do programa, evitando leituras/escritas repetidas durante a execução;

- Busca por nome convertendo tanto o termo buscado quanto o nome dos pacientes para minúsculas, evitando erros;

- Arquivos de interface para manter boas práticas e evitar que o main fique muito grande.

# Como executar o programa

Primeiramente é válido dizer que o programa foi feito em Windows, mas pensado para ser executado também em Linux Ubuntu.

###  Usando o Makefile (recomendado)
```bash
# Clone o repositório 
git clone https://github.com/juliaborgesc/TrabalhoPraticoED-P2.git
cd TrabalhoPraticoED-P2

# Compile e execute o programa
make