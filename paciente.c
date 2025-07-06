#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "paciente.h"

/*Funções para imprimir os dados do paciente em formato tabela*/
void exibir_cabecalho_tabela() {
    printf("\n%-4s %-15s %-25s %-6s %-13s\n", 
           "ID", "CPF", "Nome", "Idade", "Data_Cadastro");
}

void exibir_paciente_tabela(Paciente p) {
    printf("%-4d %-15s %-25s %-6d %-13s\n", 
           p.id, p.cpf, p.nome, p.idade, p.data_cadastro);
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

/* Formata CPF no padrão XXX.XXX.XXX-XX */
void formatar_cpf(char cpf_destino[15], const char cpf_origem[12]) {
    // Verifica se já está formatado
    if(strlen(cpf_origem) == 14 && cpf_origem[3] == '.' && cpf_origem[7] == '.' && cpf_origem[11] == '-') {
        strcpy(cpf_destino, cpf_origem);
        return;
    }
    
    // Verifica se tem 11 dígitos
    if(strlen(cpf_origem) != 11) {
        strcpy(cpf_destino, "CPF Inválido");
        return;
    }
    
    // Formata XXX.XXX.XXX-XX
    sprintf(cpf_destino, "%.3s.%.3s.%.3s-%.2s", 
            cpf_origem, 
            cpf_origem + 3, 
            cpf_origem + 6, 
            cpf_origem + 9);
}