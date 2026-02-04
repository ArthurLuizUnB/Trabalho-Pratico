#ifndef PRODUTOS_H
#define PRODUTOS_H

/*
 * Estrutura do Produto (No da Lista Encadeada).
 */
typedef struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade_estoque;
    struct Produto *prox; // Ponteiro para o proximo item da lista
} Produto;

// --- Operacoes de CRUD ---
// Recebem **head (ponteiro duplo) quando precisam alterar o inicio da lista
void cadastrar_produto(Produto **head);
void remover_produto(Produto **head);

// Recebem *head (ponteiro simples) quando so precisam ler ou alterar conteudo interno
void listar_produtos(Produto *head);
Produto* buscar_produto(Produto *head, int codigo);
void editar_produto(Produto *head);
void ordenar_produtos_codigo(Produto *head);
void liberar_produtos(Produto *head);

// --- Persistencia ---
void salvar_produtos_arquivo(Produto *head);
void carregar_produtos_arquivo(Produto **head);

#endif