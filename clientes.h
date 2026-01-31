#ifndef CLIENTES_H
#define CLIENTES_H

#include "produtos.h"

/*
 * Estrutura do Carrinho.
 * Nota: Nao guardamos o objeto Produto inteiro aqui, so o ID (codigo).
 * Isso eh mais eficiente e garante que o preco seja sempre o atual.
 */
typedef struct ItemCarrinho {
    int codigo_produto;
    int quantidade;
    struct ItemCarrinho *prox;
} ItemCarrinho;

/*
 * Estrutura do Cliente.
 * Possui uma lista encadeada interna (carrinho) para as compras.
 */
typedef struct Cliente {
    char cpf[15]; // Chave Primaria
    char nome[50];
    char email[80];
    char telefone[20];
    char data_nascimento[15];
    ItemCarrinho *carrinho; // Head da lista de compras deste cliente
    struct Cliente *prox;
} Cliente;

// --- CRUD Clientes ---
void cadastrar_cliente(Cliente **head);
void listar_clientes(Cliente *head);
Cliente* buscar_cliente(Cliente *head, char *cpf);
void editar_cliente(Cliente *head);
void remover_cliente(Cliente **head);
void ordenar_clientes_nome(Cliente *head);
void liberar_clientes(Cliente *head);

// --- Operacoes de Carrinho ---
// Adicionamos essas funcoes aqui pois sao dependentes da struct Cliente
void adicionar_ao_carrinho(Cliente *cliente, Produto *lista_produtos);
void listar_carrinho(Cliente *cliente, Produto *lista_produtos);
void remover_do_carrinho(Cliente *cliente);
void liberar_carrinho(ItemCarrinho *head);

#endif