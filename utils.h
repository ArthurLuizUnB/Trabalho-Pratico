#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* * Utilitarios gerais do sistema.
 * Separamos aqui o que nao eh regra de negocio (cliente/produto)
 * mas que todo mundo precisa usar.
 */

// Consome o "Enter" que sobra no buffer apos um scanf. 
// Evita que o proximo fgets pule a leitura.
void limpar_buffer();

// O fgets mantem o \n final na string. Essa funcao remove ele pra estetica.
void remover_quebra_linha(char *str);

// "System pause" portavel. Segura a tela pro usuario ler o output.
void pausar_tela();

// Wrapper seguro para o malloc. 
// Se faltar memoria RAM, ele avisa e encerra o programa na hora,
// evitando segfault aleatorio no meio do codigo.
void *alloc_check(size_t tamanho);

// Valida CPF usando o calculo oficial dos digitos verificadores.
int validar_cpf(char *cpf);

#endif