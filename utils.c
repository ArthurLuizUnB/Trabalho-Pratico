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

int validar_cpf(char *cpf) {
    char numeros[12];
    int i, j = 0;

    // Sanitizacao: Extrai apenas os numeros do input (ignora pontos/tracos)
    for (i = 0; cpf[i] != '\0'; i++) {
        if (isdigit(cpf[i])) {
            numeros[j++] = cpf[i];
            if (j > 11) return 0; // CPF maior que o permitido
        }
    }
    numeros[j] = '\0';

    if (strlen(numeros) != 11) return 0;

    // Blacklist de CPFs invalidos comuns (todos digitos iguais)
    if (strcmp(numeros, "00000000000") == 0 || strcmp(numeros, "11111111111") == 0 ||
        strcmp(numeros, "22222222222") == 0 || strcmp(numeros, "33333333333") == 0 ||
        strcmp(numeros, "44444444444") == 0 || strcmp(numeros, "55555555555") == 0 ||
        strcmp(numeros, "66666666666") == 0 || strcmp(numeros, "77777777777") == 0 ||
        strcmp(numeros, "88888888888") == 0 || strcmp(numeros, "99999999999") == 0)
        return 0;

    // Calculo do 1o Digito Verificador
    int soma = 0;
    for (i = 0; i < 9; i++) soma += (numeros[i] - '0') * (10 - i);
    int resto = (soma * 10) % 11;
    if (resto == 10 || resto == 11) resto = 0;
    if (resto != (numeros[9] - '0')) return 0;

    // Calculo do 2o Digito Verificador
    soma = 0;
    for (i = 0; i < 10; i++) soma += (numeros[i] - '0') * (11 - i);
    resto = (soma * 10) % 11;
    if (resto == 10 || resto == 11) resto = 0;
    if (resto != (numeros[10] - '0')) return 0;

    return 1; // Valido
}