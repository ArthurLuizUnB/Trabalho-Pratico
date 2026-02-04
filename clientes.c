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

// --- LOGICA DO CARRINHO ---

void adicionar_ao_carrinho(Cliente *cliente, Produto *lista_produtos) {
    int codigo, qtd;
    printf("Codigo do produto a comprar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Valida se o produto realmente existe no catalogo
    Produto *prod = buscar_produto(lista_produtos, codigo);
    if (prod == NULL) {
        printf("Erro: Produto nao existe no catalogo.\n");
        pausar_tela();
        return;
    }

    printf("Quantidade desejada: ");
    scanf("%d", &qtd);
    limpar_buffer();

    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        pausar_tela();
        return;
    }

    // Aviso de estoque (Nao bloqueante por decisao da equipe, mas avisa o usuario)
    if (qtd > prod->quantidade_estoque) {
        printf("Aviso: Estoque insuficiente (Disponivel: %d). Adicionando mesmo assim.\n", prod->quantidade_estoque);
    }

    // Cria novo no de compra
    ItemCarrinho *novo = (ItemCarrinho*) alloc_check(sizeof(ItemCarrinho));
    novo->codigo_produto = codigo;
    novo->quantidade = qtd;
    
    // Insere no inicio da lista de compras do cliente
    novo->prox = cliente->carrinho;
    cliente->carrinho = novo;

    printf("Item adicionado ao carrinho!\n");
    pausar_tela();
}

void listar_carrinho(Cliente *cliente, Produto *lista_produtos) {
    printf("\n--- Carrinho de Compras: %s ---\n", cliente->nome);
    if (cliente->carrinho == NULL) {
        printf("Carrinho vazio.\n");
        pausar_tela();
        return;
    }

    ItemCarrinho *item = cliente->carrinho;
    float total_valor = 0;
    int total_itens = 0;

    while (item != NULL) {
        // Cross-Referencing: Temos o ID no carrinho, buscamos os detalhes (Nome/Preco) na lista de produtos.
        // Isso garante que se o preco mudar na loja, o carrinho reflete o preco novo.
        Produto *prod = buscar_produto(lista_produtos, item->codigo_produto);
        
        if (prod != NULL) {
            float subtotal = prod->preco * item->quantidade;
            printf("%d un. x %s (R$ %.2f) = R$ %.2f\n", 
                   item->quantidade, prod->nome, prod->preco, subtotal);
            
            total_valor += subtotal;
            total_itens += item->quantidade;
        } else {
            // Tratamento de erro: Produto foi deletado da loja mas o cliente ainda tinha no carrinho
            printf("[Item Cod %d - Produto nao cadastrado/removido]\n", item->codigo_produto);
        }
        item = item->prox;
    }
    printf("--------------------------\n");
    printf("Total Itens: %d | Valor Total: R$ %.2f\n", total_itens, total_valor);
    pausar_tela();
}

void remover_do_carrinho(Cliente *cliente) {
    int codigo;
    printf("Codigo do produto para remover: ");
    scanf("%d", &codigo);
    limpar_buffer();

    ItemCarrinho *atual = cliente->carrinho;
    ItemCarrinho *ant = NULL;

    // Busca padrao em lista encadeada
    while (atual != NULL && atual->codigo_produto != codigo) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Item nao encontrado no carrinho.\n");
        pausar_tela();
        return;
    }

    // Remocao de lista encadeada
    if (ant == NULL) {
        cliente->carrinho = atual->prox;
    } else {
        ant->prox = atual->prox;
    }

    free(atual);
    printf("Item removido do carrinho.\n");
    pausar_tela();
}

// Implementacao da finalizacao da compra
// faz a baixa no estoque e limpa o carrinho.
void finalizar_compra(Cliente *cliente, Produto *lista_produtos) {
    if (cliente->carrinho == NULL) {
        printf("Carrinho vazio. Nada para comprar.\n");
        pausar_tela();
        return;
    }

    // Etapa 1: verificacao
    // Antes de mexer no estoque, verificamos se todos os itens tem saldo.
    // se um nao tiver, cancelamos tudo.
    ItemCarrinho *item = cliente->carrinho;
    while (item != NULL) {
        Produto *prod = buscar_produto(lista_produtos, item->codigo_produto);
        
        if (prod == NULL) {
            printf("Erro Critico: Produto ID %d nao existe mais no catalogo. Remova-o antes.\n", item->codigo_produto);
            pausar_tela();
            return;
        }
        if (prod->quantidade_estoque < item->quantidade) {
            printf("Erro: Produto '%s' sem estoque suficiente (Disp: %d, Pedido: %d).\n", 
                   prod->nome, prod->quantidade_estoque, item->quantidade);
            printf("Compra cancelada. Ajuste o carrinho.\n");
            pausar_tela();
            return;
        }
        item = item->prox;
    }

    // Etapa 2: efetivacao
    // Ja que passou na validacao, podemos baixar o estoque
    item = cliente->carrinho;
    float total_pago = 0;
    
    while (item != NULL) {
        Produto *prod = buscar_produto(lista_produtos, item->codigo_produto);
        
        // Baixa no estoque
        prod->quantidade_estoque -= item->quantidade;
        total_pago += prod->preco * item->quantidade;
        
        item = item->prox;
    }

    // Limpa o carrinho da memoria (venda feita)
    liberar_carrinho(cliente->carrinho);
    cliente->carrinho = NULL;

    printf("\n=== COMPRA FINALIZADA COM SUCESSO! ===\n");
    printf("Estoque atualizado.\n");
    printf("Valor Total Pago: R$ %.2f\n", total_pago);
    pausar_tela();
}

// --- LOGICA DE PERSISTENCIA (Relacional) ---

void salvar_clientes_arquivo(Cliente *head) {
    FILE *arq = fopen("clientes.txt", "w");
    if (arq == NULL) {
        printf("Erro de permissao ao salvar clientes.\n");
        return;
    }

    // Cabecalho
    fprintf(arq, "CPF;NOME;EMAIL;TELEFONE;NASCIMENTO\n");

    Cliente *atual = head;
    while (atual != NULL) {
        fprintf(arq, "%s;%s;%s;%s;%s\n", 
            atual->cpf, atual->nome, atual->email, atual->telefone, atual->data_nascimento);
        atual = atual->prox;
    }
    fclose(arq);
    
    // Apos salvar os clientes, salvamos os relacionamentos (carrinhos)
    // Se nao fizermos isso, perde-se ohistorico de compras ao fechar o programa.
    salvar_carrinhos_arquivo(head);
    printf("Backup de clientes e carrinhos realizado.\n");
}

void carregar_clientes_arquivo(Cliente **head) {
    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) return;

    char lixo[256];
    if (fgets(lixo, sizeof(lixo), arq) == NULL) {
        fclose(arq);
        return;
    }

    while (!feof(arq)) {
        Cliente *novo = (Cliente*) alloc_check(sizeof(Cliente));
        novo->carrinho = NULL;

        int res = fscanf(arq, "%14[^;];%49[^;];%79[^;];%19[^;];%14[^\n]\n", 
               novo->cpf, novo->nome, novo->email, novo->telefone, novo->data_nascimento);
        
        if (res == 5) {
            // Sucesso na leitura, insere na lista encadeada
            novo->prox = *head;
            *head = novo;
        } else {
            // Falha na leitura (linha em branco ou corrompida), libera pra nao vazar memoria
            free(novo);
        }
    }
    fclose(arq);
    
    // Com os clientes na memoria, carregamos as compras deles
    carregar_carrinhos_arquivo(*head);
}

/*
 * Salva os carrinhos num arquivo separado (carrinhos.txt).
 * Logica: "Normalizacao" de banco de dados. 
 * Em vez de salvar a lista de produtos dentro da linha do cliente (o que seria um caos pra ler depois),
 * salvamos em um arquivo separado relacionando CPF -> ID_PRODUTO.
 */
void salvar_carrinhos_arquivo(Cliente *lista_clientes) {
    FILE *arq = fopen("carrinhos.txt", "w");
    if (arq == NULL) return; // Se falhar permissao, segue a vida sem travar

    // Cabecalho CSV
    fprintf(arq, "CPF_CLIENTE;COD_PRODUTO;QUANTIDADE\n");

    // Nested Loop:
    // 1. Percorre a lista de clientes
    Cliente *cli = lista_clientes;
    while (cli != NULL) {
        // 2. Para cada cliente, percorre a lista de compras dele
        ItemCarrinho *item = cli->carrinho;
        while (item != NULL) {
            // Salva a relacao: Quem comprou (CPF) -> O que (ID) -> Quanto
            fprintf(arq, "%s;%d;%d\n", cli->cpf, item->codigo_produto, item->quantidade);
            item = item->prox;
        }
        cli = cli->prox;
    }
    fclose(arq);
}

/*
 * Le o arquivo de carrinhos e reconecta os itens aos seus donos (Clientes)
 * baseando-se no CPF salvo.
 * So deve ser chamada depois que a lista de clientes ja estiver na memoria.
 */
void carregar_carrinhos_arquivo(Cliente *lista_clientes) {
    FILE *arq = fopen("carrinhos.txt", "r");
    if (arq == NULL) return; // Arquivo nao existe na primeira vez, continua.

    // Descarta cabecalho
    char lixo[256];
    if (fgets(lixo, sizeof(lixo), arq) == NULL) {
        fclose(arq);
        return; 
    }

    char cpf_lido[15];
    int cod_prod, qtd;

    while (!feof(arq)) {
        if (fscanf(arq, "%14[^;];%d;%d\n", cpf_lido, &cod_prod, &qtd) == 3) {
            
            // Busca o ponteiro do cliente na memoria RAM usando o CPF lido do arquivo
            Cliente *dono = buscar_cliente(lista_clientes, cpf_lido);
            
            if (dono != NULL) {
                // Se achou o dono, aloca o item de carrinho e pendura na lista dele
                ItemCarrinho *novo = (ItemCarrinho*) alloc_check(sizeof(ItemCarrinho));
                novo->codigo_produto = cod_prod;
                novo->quantidade = qtd;
                
                // Insere no inicio da lista do cliente
                novo->prox = dono->carrinho;
                dono->carrinho = novo;
            }
            // Se dono == NULL (ex: cliente foi deletado manualmente do txt), 
            // a compra fica orfa e eh ignorada.
        }
    }
    fclose(arq);
}