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

Produto* buscar_produto(Produto *head, int codigo) {
    Produto *atual = head;
    while (atual != NULL) {
        if (atual->codigo == codigo) return atual;
        atual = atual->prox;
    }
    return NULL;
}

void editar_produto(Produto *head) {
    int codigo;
    printf("Digite o ID do produto para editar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    Produto *p = buscar_produto(head, codigo);
    if (p == NULL) {
        printf("Produto nao encontrado.\n");
        pausar_tela();
        return;
    }

    printf("Editando: %s\n", p->nome);
    printf("Novo nome (ENTER para manter atual): ");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) strcpy(p->nome, buffer);

    printf("Novo preco (-1 para manter): ");
    float novo_preco;
    scanf("%f", &novo_preco);
    limpar_buffer();
    if (novo_preco >= 0) p->preco = novo_preco;

    printf("Novo estoque (-1 para manter): ");
    int nova_qtd;
    scanf("%d", &nova_qtd);
    limpar_buffer();
    if (nova_qtd >= 0) p->quantidade_estoque = nova_qtd;

    printf("Dados atualizados.\n");
    pausar_tela();
}

void remover_produto(Produto **head) {
    int codigo;
    printf("ID do produto a remover: ");
    scanf("%d", &codigo);
    limpar_buffer();

    Produto *atual = *head;
    Produto *ant = NULL;

    // Busca o no e mantem o ponteiro do anterior (ant)
    while (atual != NULL && atual->codigo != codigo) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Produto nao existe.\n");
        pausar_tela();
        return;
    }

    // Se ant for NULL, estamos removendo o primeiro da lista
    if (ant == NULL) *head = atual->prox;
    else ant->prox = atual->prox;

    free(atual);
    printf("Produto removido.\n");
    pausar_tela();
}