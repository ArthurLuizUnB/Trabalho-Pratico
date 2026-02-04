#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "produtos.h"
#include "clientes.h"
#include "utils.h"

/*
 * --- WRAPPERS DE MENU ---
 * Decidimos deixar a logica de "printar menu e ler opcao" aqui na main
 * para não poluir os arquivos de logica (clientes.c/produtos.c).
 * Assim, os modulos ficam focados so nos dados, e a main cuida da interface.
 */

void menu_clientes(Cliente **lista_cli) {
    int op;
    do {
        // Limpa tela funciona em Windows (cls) e Linux (clear)
        system("cls || clear");
        
        printf("--- GESTAO DE CLIENTES ---\n");
        printf("1. Cadastrar\n");
        printf("2. Listar\n");
        printf("3. Buscar (por CPF)\n");
        printf("4. Editar\n");
        printf("5. Remover\n");
        printf("6. Ordenar (A-Z)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        
        scanf("%d", &op);
        limpar_buffer(); // Sempre limpar buffer apos scanf pra nao pular o proximo fgets

        switch(op) {
            case 1: 
                cadastrar_cliente(lista_cli); 
                break;
            case 2: 
                listar_clientes(*lista_cli); 
                break;
            case 3: {
                char cpf[15]; 
                printf("Digite o CPF: "); 
                fgets(cpf, 15, stdin); 
                remover_quebra_linha(cpf);
                
                Cliente *c = buscar_cliente(*lista_cli, cpf);
                if(c) {
                    printf("Cliente encontrado: %s (Email: %s)\n", c->nome, c->email);
                } else {
                    printf("Nenhum registro encontrado com esse CPF.\n");
                }
                pausar_tela(); // Espera o usuario ler antes de limpar a tela
                break;
            }
            case 4: 
                editar_cliente(*lista_cli); 
                break;
            case 5: 
                remover_cliente(lista_cli); 
                break;
            case 6: 
                ordenar_clientes_nome(*lista_cli); 
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida. Tente de 0 a 6.\n"); 
                pausar_tela();
        }
    } while (op != 0);
}

void menu_produtos(Produto **lista_prod) {
    int op;
    do {
        system("cls || clear");
        printf("--- GESTAO DE PRODUTOS ---\n");
        printf("1. Cadastrar\n");
        printf("2. Listar\n");
        printf("3. Buscar (por Codigo)\n");
        printf("4. Editar\n");
        printf("5. Remover\n");
        printf("6. Ordenar (por Codigo)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        
        scanf("%d", &op);
        limpar_buffer();

        switch(op) {
            case 1: 
                cadastrar_produto(lista_prod); 
                break;
            case 2: 
                listar_produtos(*lista_prod); 
                break;
            case 3: {
                int c; 
                printf("Digite o Codigo: "); 
                scanf("%d", &c); 
                limpar_buffer();
                
                Produto *p = buscar_produto(*lista_prod, c);
                if(p) {
                    printf("Produto: %s | Preco: R$ %.2f | Estoque: %d\n", p->nome, p->preco, p->quantidade_estoque);
                } else {
                    printf("Produto nao encontrado no catalogo.\n");
                }
                pausar_tela();
                break;
            }
            case 4: 
                editar_produto(*lista_prod); 
                break;
            case 5: 
                remover_produto(lista_prod); 
                break;
            case 6: 
                ordenar_produtos_codigo(*lista_prod); 
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida.\n"); 
                pausar_tela();
        }
    } while (op != 0);
}

void menu_compra(Cliente **lista_cli, Produto *lista_prod) {
    char cpf[15];
    system("cls || clear");
    printf("--- MODO COMPRA ---\n");
    printf("Identifique o cliente (CPF): ");
    fgets(cpf, 15, stdin);
    remover_quebra_linha(cpf);

    // Valida se o cliente existe antes de prosseguir
    Cliente *cli = buscar_cliente(*lista_cli, cpf);
    if (cli == NULL) {
        printf("Erro: Cliente nao encontrado na base.\nCadastre o cliente antes de realizar compras.\n");
        pausar_tela();
        return;
    }

    // Se achou o cliente, entra no loop do carrinho dele
    int op;
    do {
        system("cls || clear");
        printf("--- CARRINHO DE: %s ---\n", cli->nome);
        printf("1. Adicionar Produto ao Carrinho\n");
        printf("2. Ver Resumo do Carrinho (Total)\n");
        printf("3. Remover Item do Carrinho\n");
        printf("4. Finalizar Compra (Baixar Estoque)\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        
        scanf("%d", &op);
        limpar_buffer();

        switch(op) {
            case 1: 
                // Passamos a lista de produtos pra ele poder buscar preco/nome pelo ID
                adicionar_ao_carrinho(cli, lista_prod); 
                break;
            case 2: 
                listar_carrinho(cli, lista_prod); 
                break;
            case 3: 
                remover_do_carrinho(cli); 
                break;
            case 4:
                finalizar_compra(cli, lista_prod);
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida.\n"); 
                pausar_tela();
        }
    } while (op != 0);
}

// --- FUNCAO PRINCIPAL ---

int main() {
    // Arruma acentuacao no terminal (pra nao ficar imprimindo caracteres estranhos)
    setlocale(LC_ALL, "Portuguese");

    Produto *lista_produtos = NULL;
    Cliente *lista_clientes = NULL;

    // --- Carregamento de dados/persistencia ---
    printf("Inicializando sistema...\n");
    printf("Carregando banco de dados (arquivos .txt)...\n");

    carregar_produtos_arquivo(&lista_produtos);
    carregar_clientes_arquivo(&lista_clientes); 
    
    pausar_tela();

    int opcao;
    do {
        system("cls || clear");
        printf("=== SISTEMA DE GESTAO DE LOJA ===\n");
        printf("1. Gestao de Clientes\n");
        printf("2. Gestao de Produtos\n");
        printf("3. Modo Compra (Carrinho)\n");
        printf("0. Sair e Salvar Dados\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = -1;      // Forca cair no default
        }
        limpar_buffer();

        switch (opcao) {
            case 1: 
                // Passamos o endereco (&) pq a lista pode mudar (como inserir no inicio)
                menu_clientes(&lista_clientes); 
                break;
            case 2: 
                menu_produtos(&lista_produtos); 
                break;
            case 3: 
                menu_compra(&lista_clientes, lista_produtos); 
                break;
            case 0: 
                printf("Salvando alteracoes no disco...\n");
                
                // Salva tudo antes de liberar a memoria RAM
                salvar_produtos_arquivo(lista_produtos);
                salvar_clientes_arquivo(lista_clientes);
                
                printf("Dados salvos. Encerrando o programa.\n"); 
                break;
            default: 
                printf("Opcao invalida! Digite um numero do menu.\n"); 
                pausar_tela();
        }
    } while (opcao != 0);

    liberar_clientes(lista_clientes);
    liberar_produtos(lista_produtos);

    return 0;
}