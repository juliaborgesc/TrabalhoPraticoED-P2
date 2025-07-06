#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // Para toupper()
#include "paciente.h"
#include "bd_paciente.h"
#include "interface.h"

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
            case '5': 
                exibir_cabecalho_tabela();
                bd_listar_pacientes(bd); break;
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