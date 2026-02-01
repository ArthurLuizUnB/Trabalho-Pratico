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

void listar_clientes(Cliente *head) {
    printf("\n--- Clientes Cadastrados ---\n");
    if (head == NULL) {
        printf("Base de clientes vazia.\n");
        pausar_tela();
        return;
    }
    // Itera ate o ponteiro ser NULL (fim da lista)
    Cliente *atual = head;
    while (atual != NULL) {
        printf("CPF: %s | Nome: %s | Email: %s\n", atual->cpf, atual->nome, atual->email);
        atual = atual->prox;
    }
    pausar_tela();
}

void editar_cliente(Cliente *head) {
    char cpf[15];
    printf("Digite o CPF para editar: ");
    fgets(cpf, sizeof(cpf), stdin);
    remover_quebra_linha(cpf);

    // Reusa a funcao de busca pra nao duplicar logica
    Cliente *c = buscar_cliente(head, cpf);
    if (c == NULL) {
        printf("Cliente nao localizado.\n");
        pausar_tela();
        return;
    }

    printf("Editando: %s\n", c->nome);
    char buffer[80];
    
    // Logica de UX: Leitura no buffer. Se apertar Enter (len=0), nao altera o dado original.
    // Isso permite editar so um campo sem ter que redigitar tudo.
    printf("Novo Nome (ENTER mantem): ");
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) strcpy(c->nome, buffer);

    printf("Novo Email (ENTER mantem): ");
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) strcpy(c->email, buffer);

    printf("Novo Telefone (ENTER mantem): ");
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) strcpy(c->telefone, buffer);

    printf("Nova Data Nasc (ENTER mantem): ");
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) strcpy(c->data_nascimento, buffer);

    printf("Dados atualizados!\n");
    pausar_tela();
}

void remover_cliente(Cliente **head) {
    char cpf[15];
    printf("CPF para remocao: ");
    fgets(cpf, sizeof(cpf), stdin);
    remover_quebra_linha(cpf);

    Cliente *atual = *head;
    Cliente *ant = NULL;

    while (atual != NULL && strcmp(atual->cpf, cpf) != 0) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente nao encontrado.\n");
        pausar_tela();
        return;
    }

    // Antes de remover o cliente, limpar a lista de compras/carrinho.
    // Se nao fizer isso, perdemos o acesso a essa memoria e ocorre Memory Leak.
    liberar_carrinho(atual->carrinho); 

    // Logica de remocao de lista encadeada:
    if (ant == NULL) {
        *head = atual->prox;
    } else {
        ant->prox = atual->prox;
    }

    free(atual);
    printf("Cliente e seus dados removidos com sucesso.\n");
    pausar_tela();
}

void ordenar_clientes_nome(Cliente *head) {
    // lista vazia ou com 1 elemento nao precisa ordenar
    if (head == NULL || head->prox == NULL) return;

    int trocou;
    Cliente *ptr1;
    Cliente *lptr = NULL; // Ponteiro para marcar o fim da parte ja ordenada (otimizacao)

    do {
        trocou = 0;
        ptr1 = head;

        while (ptr1->prox != lptr) {
            // Compara strings alfabeticamente
            if (strcmp(ptr1->nome, ptr1->prox->nome) > 0) {
                // SWAP
                // Estamos trocando os dados dentro dos nos, e nao os ponteiros.
                // Isso eh menos eficiente computacionalmente, mas mais seguro
                // para evitar quebrar a lista (perder ponteiros).
                
                char t_cpf[15], t_nome[50], t_email[80], t_tel[20], t_nasc[15];
                ItemCarrinho *t_carrinho;

                // 1. Salva dados do atual em variaveis temporarias
                strcpy(t_cpf, ptr1->cpf);
                strcpy(t_nome, ptr1->nome);
                strcpy(t_email, ptr1->email);
                strcpy(t_tel, ptr1->telefone);
                strcpy(t_nasc, ptr1->data_nascimento);
                t_carrinho = ptr1->carrinho;

                // 2. Copia dados do proximo para o atual
                strcpy(ptr1->cpf, ptr1->prox->cpf);
                strcpy(ptr1->nome, ptr1->prox->nome);
                strcpy(ptr1->email, ptr1->prox->email);
                strcpy(ptr1->telefone, ptr1->prox->telefone);
                strcpy(ptr1->data_nascimento, ptr1->prox->data_nascimento);
                ptr1->carrinho = ptr1->prox->carrinho;

                // 3. Restaura dados do temp para o proximo
                strcpy(ptr1->prox->cpf, t_cpf);
                strcpy(ptr1->prox->nome, t_nome);
                strcpy(ptr1->prox->email, t_email);
                strcpy(ptr1->prox->telefone, t_tel);
                strcpy(ptr1->prox->data_nascimento, t_nasc);
                ptr1->prox->carrinho = t_carrinho;

                trocou = 1;
            }
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    } while (trocou);
    
    printf("Lista ordenada por nome (A-Z).\n");
    pausar_tela();
}

// Funcao de limpeza para encerrar o programa (Garbage Collection manual)
void liberar_clientes(Cliente *head) {
    Cliente *atual = head;
    while (atual != NULL) {
        Cliente *prox = atual->prox;
        
        // Antes de liberar o cliente, libera a lista interna dele
        liberar_carrinho(atual->carrinho);
        
        free(atual);
        atual = prox;
    }
}

// Limpa a lista do carrinho iterativamente.
// while em vez de recursao para evitar Stack Overflow se a lista for gigante.
void liberar_carrinho(ItemCarrinho *head) {
    ItemCarrinho *atual = head;
    while (atual != NULL) {
        ItemCarrinho *prox = atual->prox; // Salva o proximo antes de matar o atual
        free(atual);
        atual = prox;
    }
}