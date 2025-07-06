#ifndef INTERFACE_H
#define INTERFACE_H

#include "paciente.h"
#include "bd_paciente.h"

/* Protótipo das funções da interface com usuário */
void menu_principal();
void consulta_nome(BDPaciente* bd);
void consulta_cpf(BDPaciente* bd);
void atualizar_paciente(BDPaciente* bd);
void remover_paciente(BDPaciente* bd);
void inserir_paciente(BDPaciente* bd);

#endif