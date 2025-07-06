#include <stdio.h>
#include <string.h>
#include "paciente.h"

/*Função para imprimir os dados do paciente*/
void visualizar_paciente(Paciente p) {
    printf("\n[Dados do Paciente]\n");
    printf("ID: %d\n", p.id);
    printf("CPF: %s\n", p.cpf);
    printf("Nome: %s\n", p.nome);
    printf("Idade: %d\n", p.idade);
    printf("Data de Cadastro: %s\n", p.data_cadastro);
}

/* Função para criar um novo paciente */
Paciente criar_paciente(int id, const char* cpf, const char* nome, int idade, const char* data_cadastro) {
    Paciente p;
    p.id = id;
    strcpy(p.cpf, cpf); 
    strcpy(p.nome, nome);
    p.idade = idade;
    strcpy(p.data_cadastro, data_cadastro);
    return p;
}