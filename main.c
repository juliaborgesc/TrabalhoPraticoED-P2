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
    char nome[100];
    printf("Digite o nome que deseja pesquisar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';
    
    char busca[100];
    strcpy(busca, nome);
    for(int i = 0; busca[i]; i++) {
        busca[i] = tolower(busca[i]);
    }
    
    NodePaciente* resultados = NULL;
    NodePaciente* atual = bd->first;
    
    while(atual) {
        char nome_paciente[100];
        strcpy(nome_paciente, atual->paciente.nome);
        for(int i = 0; nome_paciente[i]; i++) {
            nome_paciente[i] = tolower(nome_paciente[i]);
        }
        
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
        printf("Nenhum paciente encontrado.\n");
        return;
    }
    
    printf("\nResultados:\n");
    NodePaciente* temp = resultados;
    while (temp) {
        visualizar_paciente(temp->paciente);
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
    NodePaciente* atual = resultados;
    while (atual) {
        visualizar_paciente(atual->paciente);
        NodePaciente* temp = atual;
        atual = atual->next;
        free(temp);
    }
}

void atualizar_paciente(BDPaciente* bd) {
    printf("\nAtualizar Paciente:\n");
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
    
    printf("\nDados atuais:\n");
    visualizar_paciente(*p);
    
    printf("\nNovos dados:\n");
    
    char entrada[100];
    Paciente new = *p;
    
    printf("CPF [%s]: ", p->cpf);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(new.cpf, entrada);
    }
    
    printf("Nome [%s]: ", p->nome);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(new.nome, entrada);
    }
    
    printf("Idade [%d]: ", p->idade);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        new.idade = atoi(entrada);
    }
    
    printf("Data Cadastro [%s]: ", p->data_cadastro);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(new.data_cadastro, entrada);
    }
    
    printf("\nConfirmar atualização? (S/N): ");
    char op;
    scanf(" %c", &op);
    getchar();
    
    if (toupper(op) == 'S') {
        *p = new;
        printf("Atualizado com sucesso!\n");
    } else {
        printf("Cancelado.\n");
    }
}

void remover_paciente(BDPaciente* bd) {
    printf("\nRemover Paciente:\n");
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
    
    printf("\nPaciente a remover:\n");
    visualizar_paciente(*p);
    
    printf("\nConfirmar remoção? (S/N): ");
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
    printf("\nInserir Novo Paciente:\n");
    
    Paciente new;
    new.id = bd_gerar_id(bd);
    
    printf("CPF (apenas números): ");
    char cpf[12];
    scanf("%11s", cpf);
    getchar();
    sprintf(new.cpf, "%.3s.%.3s.%.3s-%.2s", cpf, cpf+3, cpf+6, cpf+9); // Formata o CPF para o padrão XXX.XXX.XXX-XX
    
    printf("Nome: ");
    fgets(new.nome, sizeof(new.nome), stdin);
    new.nome[strcspn(new.nome, "\n")] = '\0';
    
    printf("Idade: ");
    scanf("%d", &new.idade);
    getchar();
    
    printf("Data de Cadastro (AAAA-MM-DD): ");
    scanf("%10s", new.data_cadastro); // Lê a data no formato AAAA-MM-DD
    getchar();
    
    printf("\nNovo paciente:\n");
    visualizar_paciente(new);
    
    printf("\nConfirmar inserção? (S/N): ");
    char op;
    scanf(" %c", &op);
    getchar();
    
    if (toupper(op) == 'S') {
        bd_inserir_paciente(bd, new);
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