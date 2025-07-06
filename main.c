#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "paciente.h"
#include "bd_paciente.h"

void menu_principal() {
    printf("\n[MENU]\n");
    printf("1 - Consultar paciente\n");
    printf("2 - Atualizar paciente\n");
    printf("3 - Remover paciente\n");
    printf("4 - Inserir paciente\n");
    printf("5 - Imprimir lista de pacientes\n");
    printf("Q - Sair\n");
    printf("Escolha uma opcao: ");
}

void consulta_nome(BDPaciente* bd) {
    char nome[80];
    printf("Digite o nome que deseja pesquisar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; /* Remove o \n */

    /* Converte a busca para minúsculas */
    char busca[80];
    for(int i = 0; nome[i]; i++) {
        busca[i] = tolower(nome[i]);
    }
    busca[strlen(nome)] = '\0';

    NodePaciente* resultados = NULL;
    NodePaciente* atual = bd->first;
    
    while(atual) {
        char nome_paciente[80];
        /* Converte o nome do paciente para minúsculas */
        for(int i = 0; atual->paciente.nome[i]; i++) {
            nome_paciente[i] = tolower(atual->paciente.nome[i]);
        }
        nome_paciente[strlen(atual->paciente.nome)] = '\0';

        /* Verifica se contém o prefixo buscado */
        if(strstr(nome_paciente, busca) != NULL) {
            NodePaciente* novo = (NodePaciente*)malloc(sizeof(NodePaciente));
            if(novo) {
                novo->paciente = atual->paciente;
                novo->next = resultados;
                resultados = novo;
            }
        }
        atual = atual->next;
    }

    if (!resultados) {
        printf("Nenhum paciente encontrado com '%s'.\n", nome);
        return;
    }

    printf("\nResultados para '%s':\n", nome);
    exibir_cabecalho_tabela();
    
    NodePaciente* temp = resultados;
    while(temp) {
        exibir_paciente_tabela(temp->paciente);
        NodePaciente* proximo = temp->next;
        free(temp);
        temp = proximo;
    }
}

void consulta_cpf(BDPaciente* bd) {
    char cpf[20];
    printf("Digite o CPF que deseja pesquisar: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    NodePaciente* resultados = bd_buscar_cpf(bd, cpf);
    if (!resultados) {
        printf("Nenhum paciente encontrado.\n");
        return;
    }
    
    printf("\nResultados:\n");
    exibir_cabecalho_tabela();
    
    NodePaciente* atual = resultados;
    while (atual) {
        exibir_paciente_tabela(atual->paciente);
        NodePaciente* temp = atual;
        atual = atual->next;
        free(temp);
    }
}

void atualizar_paciente(BDPaciente* bd) {
    printf("\n[Atualizar Paciente]\n");

    /* Lista todos para que se escolha um */
    printf("\nLista de pacientes:\n");
    exibir_cabecalho_tabela();
    bd_listar_pacientes(bd);
    
    printf("\nDigite o ID do paciente: ");
    int id;
    scanf("%d", &id); 
    getchar(); /* Limpa o buffer de entrada */
    
    Paciente* p = bd_buscar_id(bd, id);
    if (!p) {
        printf("Paciente não encontrado!\n");
        return;
    }
    
    printf("\nDigite os novos valores (para manter o valor atual de um campo, digite '-'):\n");
    
    Paciente novo = *p;
    char entrada[100];
    
    printf("CPF [%s]: ", p->cpf);
    fgets(entrada, sizeof(entrada), stdin);
    if(strcmp(entrada, "-") != 0) {
        char cpf_sem_formatacao[12];
        int j = 0;
        for(int i = 0; entrada[i] && j < 11; i++) {
            if(isdigit(entrada[i])) {
                cpf_sem_formatacao[j++] = entrada[i];
            }
        }
        cpf_sem_formatacao[j] = '\0';
        
        if(strlen(cpf_sem_formatacao) == 11) {
            formatar_cpf(novo.cpf, cpf_sem_formatacao);
        } else {
            printf("CPF deve conter 11 dígitos!\n");
        }
    }
    
    printf("Nome [%s]: ", p->nome);
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';
    if(strcmp(entrada, "-") != 0 && strlen(entrada) > 0) {
        strcpy(novo.nome, entrada);
    }
    
    printf("Idade [%d]: ", p->idade);
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';
    if(strcmp(entrada, "-") != 0) {
        int idade = atoi(entrada);
        if(idade > 0 && idade < 120) {
            novo.idade = idade;
        } else {
            printf("Idade inválida!\n");
        }
    }
    
    printf("Data Cadastro [%s]: ", p->data_cadastro);
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';
    if(strcmp(entrada, "-") != 0) {
        if(strlen(entrada) == 10 && entrada[4] == '-' && entrada[7] == '-') {
            strcpy(novo.data_cadastro, entrada);
        } else {
            printf("Formato de data inválido! Use AAAA-MM-DD\n");
        }
    }
    
    printf("\nConfirma os novos valores para o registro abaixo? (S/N): ");
    exibir_cabecalho_tabela();
    exibir_paciente_tabela(novo);
    char op;
    scanf(" %c", &op);
    getchar();
    
    if (toupper(op) == 'S') {
        *p = novo;
        printf("Atualizado com sucesso!\n");
    } else {
        printf("Cancelado.\n");
    }
}

void remover_paciente(BDPaciente* bd) {
    printf("\n[Remover Paciente]\n");
    bd_listar_pacientes(bd);
    
    printf("\nDigite o ID do paciente: ");
    int id;
    scanf("%d", &id);
    getchar();
    
    Paciente* p = bd_buscar_id(bd, id);
    if (!p) {
        printf("Paciente não encontrado!\n");
        return;
    }
    
    printf("\nTem certeza de que deseja excluir o registro abaixo? (S/N)");
    exibir_cabecalho_tabela();
    exibir_paciente_tabela(*p);
    char op;
    scanf(" %c", &op);
    getchar();
    
    if (toupper(op) == 'S') {
        if (bd_remover_paciente(bd, id)) {
            printf("Removido com sucesso!\n");
        } else {
            printf("Erro ao remover.\n");
        }
    } else {
        printf("Cancelado.\n");
    }
}

void inserir_paciente(BDPaciente* bd) {
    printf("\n[Inserir Novo Paciente]\n");
    
    Paciente novo;
    novo.id = bd_gerar_id(bd);
    
    printf("CPF (apenas números): ");
    char cpf[12];
    scanf("%11s", cpf);
    getchar();
    formatar_cpf(novo.cpf, cpf);
    /* Verifica se o cpf já está cadastrado*/
    if (bd_buscar_cpf(bd, novo.cpf)) {
    printf("Erro: CPF já cadastrado!\n");
    return;
    }
    
    printf("Nome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    
    printf("Idade: ");
    scanf("%d", &novo.idade);
    getchar();
    
    printf("Data de Cadastro (AAAA-MM-DD): ");
    scanf("%10s", novo.data_cadastro); // Lê a data no formato AAAA-MM-DD
    getchar();
    
    printf("\nNovo paciente:\n");
    exibir_cabecalho_tabela();
    exibir_paciente_tabela(novo);
    
    printf("\nConfirmar inserção? (S/N): ");
    char op;
    scanf(" %c", &op);
    getchar();
    
    if (toupper(op) == 'S') {
        bd_inserir_paciente(bd, novo);
        printf("Inserido com sucesso!\n");
    } else {
        printf("Cancelado.\n");
    }
}

int main() {
    /* Carrega os pacientes do arquivo csv */
    BDPaciente* bd = bd_carregar_arquivo("bd_paciente.csv");
    /* Verifica se o banco de dados foi carregado corretamente */
    if (!bd) {
        printf("Erro ao carregar pacientes do arquivo.\n");
        return 1;
    }

    char opcao;
    do {
        menu_principal();
        scanf(" %c", &opcao);
        opcao = toupper(opcao);
        getchar();

        switch (opcao) {
            case '1':
                printf("1 - Por nome\n2 - Por CPF\nEscolha: ");
                char sub;
                scanf(" %c", &sub);
                getchar();
                if (sub == '1') consulta_nome(bd);
                else if (sub == '2') consulta_cpf(bd);
                else printf("Opção inválida.\n");
                break;
            case '2': atualizar_paciente(bd); break;
            case '3': remover_paciente(bd); break;
            case '4': inserir_paciente(bd); break;
            case '5': bd_listar_pacientes(bd); break;
            case 'Q': 
                printf("Saindo...\n");
                break;
            default: 
                printf("Opção inválida.\n");
        }

    } while (opcao != 'Q');

    if (!bd_salvar_arquivo("bd_paciente.csv", bd)) {
        printf("Não foi possível salvar as alterações no arquivo!\n");
    }

    /* Libera a memória alocada para o banco de dados */
    bd_free(bd);

    return 0;
}