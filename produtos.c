#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"
#include "utils.h"

// --- FUNCOES CRUD ---

void cadastrar_produto(Produto **head) {
    printf("\n--- Cadastro de Produto ---\n");
    int codigo;
    printf("Codigo (numerico): ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Validacao de unicidade (Primary Key)
    if (buscar_produto(*head, codigo) != NULL) {
        printf("Erro: Ja existe produto com esse codigo.\n");
        pausar_tela();
        return;
    }

    Produto *novo = (Produto*) alloc_check(sizeof(Produto));
    novo->codigo = codigo;
    
    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    remover_quebra_linha(novo->nome);
    
    printf("Preco: ");
    scanf("%f", &novo->preco);
    limpar_buffer();

    printf("Quantidade Inicial: ");
    scanf("%d", &novo->quantidade_estoque);
    limpar_buffer();

    // Insercao na cabeca da lista (Head)
    novo->prox = *head;
    *head = novo;

    printf("Produto salvo!\n");
    pausar_tela();
}

void listar_produtos(Produto *head) {
    printf("\n--- Catalogo de Produtos ---\n");
    if (head == NULL) {
        printf("Nenhum produto cadastrado.\n");
        pausar_tela();
        return;
    }
    Produto *atual = head;
    while (atual != NULL) {
        printf("ID: %d | %s | R$ %.2f | Qtd: %d\n", 
               atual->codigo, atual->nome, atual->preco, atual->quantidade_estoque);
        atual = atual->prox;
    }
    pausar_tela();
}