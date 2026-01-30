#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void limpar_buffer() {
    int c;
    // Loop que le char por char ate achar o final da linha ou do arquivo
    while ((c = getchar()) != '\n' && c != EOF);
}

void remover_quebra_linha(char *str) {
    size_t len = strlen(str);
    // Se o ultimo char for pular linha, troca pelo terminador nulo
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void pausar_tela() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void *alloc_check(size_t tamanho) {
    void *ptr = malloc(tamanho);
    if (ptr == NULL) {
        printf("Erro critico: Falha na alocacao de memoria (malloc retornou NULL).\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}