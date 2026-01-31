#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clientes.h"
#include "utils.h"

// --- CRUD CLIENTES ---

void cadastrar_cliente(Cliente **head) {
    printf("\n--- Novo Cliente ---\n");
    char cpf[15];
    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    remover_quebra_linha(cpf);

    // Valida digitos verificadores
    if (!validar_cpf(cpf)) {
        printf("CPF Invalido (Verifique os digitos).\n");
        pausar_tela();
        return;
    }

    // Verifica duplicidade na lista linearmente
    if (buscar_cliente(*head, cpf) != NULL) {
        printf("Erro: Cliente ja cadastrado.\n");
        pausar_tela();
        return;
    }

    // Alocacao segura (wrapper alloc_check)
    Cliente *novo = (Cliente*) alloc_check(sizeof(Cliente));
    strcpy(novo->cpf, cpf);

    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    remover_quebra_linha(novo->nome);

    printf("Email: ");
    fgets(novo->email, sizeof(novo->email), stdin);
    remover_quebra_linha(novo->email);

    printf("Telefone: ");
    fgets(novo->telefone, sizeof(novo->telefone), stdin);
    remover_quebra_linha(novo->telefone);

    printf("Data Nasc (DD/MM/AAAA): ");
    fgets(novo->data_nascimento, sizeof(novo->data_nascimento), stdin);
    remover_quebra_linha(novo->data_nascimento);

    novo->carrinho = NULL; // Novo cliente comeca sem compras
    
    // Atualiza o ponteiro HEAD da lista (passagem por referencia)
    novo->prox = *head;
    *head = novo;

    printf("Cliente salvo!\n");
    pausar_tela();
}

Cliente* buscar_cliente(Cliente *head, char *cpf) {
    Cliente *atual = head;
    while (atual != NULL) {
        // strcmp retorna 0 se as strings forem identicas
        if (strcmp(atual->cpf, cpf) == 0) return atual;
        atual = atual->prox;
    }
    return NULL; // Nao achou
}