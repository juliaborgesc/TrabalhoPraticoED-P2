#ifndef PACIENTE_H
#define PACIENTE_H

typedef struct {
    int id;
    char cpf[15];
    char nome[80];
    int idade;
    char data_cadastro[11]; /* AAAA-MM-DD */
} Paciente;

/* Imprime dados do paciente */
void visualizar_paciente(Paciente p);

/* Cria um novo paciente */
Paciente criar_paciente(int id, const char* cpf, const char* nome, int idade, const char* data_cadastro);

#endif