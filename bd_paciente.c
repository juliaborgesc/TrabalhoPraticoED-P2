#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // Para isdigit()
#include <string.h>
#include "bd_paciente.h"

/* Implementação das funções de bd_paciente.h */
BDPaciente* bd_criar() {
    BDPaciente* bd = (BDPaciente*)malloc(sizeof(BDPaciente));
    if (bd) {
        bd->first = NULL;
        bd->tamanho = 0;
    }
    return bd;
}

/* Libera a memória alocada para o banco de dados */
void bd_free(BDPaciente* bd) {
    if (!bd) return;
    
    NodePaciente* atual = bd->first;
    while (atual) {
        NodePaciente* temp = atual;
        atual = atual->next;
        free(temp);
    }
    free(bd);
}

/* Carrega pacientes do banco de dados CSV */
BDPaciente* bd_carregar_arquivo(const char* bd_paciente) {
    FILE* file = fopen(bd_paciente, "r");
    if (!file) return NULL;

    /* Cria o bd*/
    BDPaciente* bd = bd_criar();
    if (!bd) {
        fclose(file);
        return NULL;
    }
    
    char linha[256];
    /* Pula o cabeçalho */
    fgets(linha, sizeof(linha), file);
    
    while (fgets(linha, sizeof(linha), file)) {
        Paciente p;
        if (sscanf(linha, "%d,%14[^,],%99[^,],%d,%10s", 
                  &p.id, p.cpf, p.nome, &p.idade, p.data_cadastro) == 5) {
            bd_inserir_paciente(bd, p);
        }
    }
    
    fclose(file);
    return bd;
}

/* Salva pacientes no CSV */
int bd_salvar_arquivo(const char* bd_paciente, BDPaciente* bd) {
    FILE* file = fopen(bd_paciente, "w");
    if (!file) return 0;
    
    fprintf(file, "ID,CPF,Nome,Idade,Data_Cadastro\n");
    
    NodePaciente* atual = bd->first;
    while (atual) {
        fprintf(file, "%d,%s,%s,%d,%s\n", 
                atual->paciente.id,
                atual->paciente.cpf,
                atual->paciente.nome,
                atual->paciente.idade,
                atual->paciente.data_cadastro);
        atual = atual->next;
    }
    
    fclose(file);
    return 1;
}
/* Insere um novo paciente no bd*/
void bd_inserir_paciente(BDPaciente* bd, Paciente paciente) {
    NodePaciente* new = (NodePaciente*)malloc(sizeof(NodePaciente));
    if (!new) return; 
    
    new->paciente = paciente;
    new->next = bd->first;
    bd->first = new;
    bd->tamanho++;
}

/* Remove um paciente do bd pelo id*/
int bd_remover_paciente(BDPaciente* bd, int id) {
    NodePaciente* prev = NULL;
    NodePaciente* atual = bd->first;
    
    while (atual && atual->paciente.id != id) {
        prev = atual;
        atual = atual->next;
    }
    
    if (!atual) return 0; /* Não encontrado */
    
    /* Se o paciente removido é o primeiro */
    if (!prev) {
        bd->first = atual->next;
    } else {
        prev->next = atual->next;
    }
    free(atual);
    bd->tamanho--;
    return 1;
}

/* Busca um paciente pelo id*/
Paciente* bd_buscar_id(BDPaciente* bd, int id) {
    NodePaciente* atual = bd->first;
    while (atual) {
        if (atual->paciente.id == id) {
            return &(atual->paciente);
        }
        atual = atual->next;
    }
    return NULL;
}

/* Busca por nome */
NodePaciente* bd_buscar_nome(BDPaciente* bd, const char* nome){
    NodePaciente* resultados = NULL;
    NodePaciente* atual = bd->first;
    
    while (atual) {
        /* Verifica se o nome contém o prefixo */
        if (strstr(atual->paciente.nome, nome) != NULL) {
            NodePaciente* new = (NodePaciente*)malloc(sizeof(NodePaciente));
            if (new) {
                new->paciente = atual->paciente;
                new->next = resultados;
                resultados = new;
            }
        }
        atual = atual->next;
    }
    
    return resultados;
}
/* Busca por cpf */
NodePaciente* bd_buscar_cpf(BDPaciente* bd, const char* cpf){
    NodePaciente* resultados = NULL;
    NodePaciente* atual = bd->first;
    
    /* Extrai apenas dígitos do CPF buscado */
    char cpf_buscado[12] = {0};
    int j = 0;
    for(int i = 0; cpf[i] && j < 11; i++) {
        if(isdigit((unsigned char)cpf[i])) {
            cpf_buscado[j++] = cpf[i];
        }
    }
    
    /* Se nenhum dígito válido foi informado */
    if(j == 0) return NULL;
    
    while(atual) {
        /* Extrai apenas dígitos do CPF armazenado */
        char cpf_armazenado[12] = {0};
        int k = 0;
        for(int i = 0; atual->paciente.cpf[i] && k < 11; i++) {
            if(isdigit((unsigned char)atual->paciente.cpf[i])) {
                cpf_armazenado[k++] = atual->paciente.cpf[i];
            }
        }
        
        /* Verifica se o CPF buscado é prefixo do CPF armazenado */
        int match = 1; // 1 para verdadeiro
        for(int i = 0; i < j&& cpf_buscado[i] && cpf_armazenado[i]; i++) {
            if(cpf_buscado[i] != cpf_armazenado[i]) {
                match = 0;
                break;
            }
        }
        
        if(match) {
            NodePaciente* novo = (NodePaciente*)malloc(sizeof(NodePaciente));
            if(novo) {
                novo->paciente = atual->paciente;
                novo->next = resultados;
                resultados = novo;
            }
        }
        atual = atual->next;
    }
    
    return resultados;
}
/* Lista todos os pacientes */
void bd_listar_pacientes(BDPaciente* bd){
    NodePaciente* atual = bd->first;
    while (atual) {
        exibir_paciente_tabela(atual->paciente);
        atual = atual->next;
    }
}
/* Gera novo ID para um paciente */
int bd_gerar_id(BDPaciente* bd) {
    int max_id = 0; 
    NodePaciente* atual = bd->first;
    while (atual) {
        if (atual->paciente.id > max_id) {
            max_id = atual->paciente.id;
        }
        atual = atual->next;
    } 
    return max_id + 1;
}

