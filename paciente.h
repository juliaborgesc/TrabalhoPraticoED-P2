#ifndef PACIENTE_H
#define PACIENTE_H

/* Estrutura que representa um paciente */
typedef struct {
    int id;
    char cpf[15];
    char nome[80];
    int idade;
    char data_cadastro[11]; /* AAAA-MM-DD */
} Paciente;

/* Exibe paciente em formato tabela para melhor visualização */
void exibir_paciente_tabela(Paciente p);
void exibir_cabecalho_tabela();

/* Cria um novo paciente */
Paciente criar_paciente(int id, const char* cpf, const char* nome, int idade, const char* data_cadastro);

/* Formata o CPF (remove pontuações) */
void formatar_cpf(char cpf_destino[15], const char cpf_origem[12]);
#endif