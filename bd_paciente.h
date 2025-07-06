#ifndef BD_PACIENTE_H
#define BD_PACIENTE_H

#include "paciente.h"

/* Estrutura que representa um nó da lista encadeada */
typedef struct NodePaciente {
    Paciente paciente;              /* Dados do paciente */ 
    struct NodePaciente* next;    /* Ponteiro para o próximo nó */
} NodePaciente;

// Estrutura do banco de dados de pacientes
typedef struct {
    NodePaciente* first;    /* Ponteiro para o primeiro paciente */
    int tamanho;           /* Quantidade armazenada */
} BDPaciente;

/* Funções para manipulação do banco de dados de pacientes */
/* Cria e inicializa o banco de dados */
BDPaciente* bd_criar(); 

/* Carrega pacientes do banco de dados CSV */
BDPaciente* bd_carregar_arquivo(const char* filename);

/* Libera a memória alocada*/
void bd_free(BDPaciente* bd);

/* Salva pacientes no CSV*/
int bd_salvar_arquivo(const char* filename, BDPaciente* bd);

/* Insere um novo paciente no bd*/
void bd_inserir_paciente(BDPaciente* bd, Paciente paciente);

/* Remove um paciente do bd pelo id*/
int bd_remover_paciente(BDPaciente* bd, int id);

/* Busca um paciente pelo id*/
Paciente* bd_buscar_id(BDPaciente* bd, int id);

/* Busca por nome */
NodePaciente* bd_buscar_nome(BDPaciente* bd, const char* nome);

/* Busca por cpf */
NodePaciente* bd_buscar_cpf(BDPaciente* bd, const char* cpf);

/* Lista todos os pacientes */
void bd_listar_pacientes(BDPaciente* bd);

/* Gera novo ID para um paciente */
int bd_gerar_id(BDPaciente* bd);

#endif
