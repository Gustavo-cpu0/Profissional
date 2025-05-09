// Este será o meu Head Master ai so preciso chamar ele em vez de varios includes hehe:
#ifndef ESTRUTURA_LIVROS_H
#define ESTRUTURA_LIVROS_H
// Definindo o tamanho maximo do array, dessa forma só eu trocar aqui, que troco onde ela estiver:
#define TAMANHO_ARRAY 100
// Importes do sistema:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Minhas libs:
#include "cores.h"
#include "funcoes_uteis.h"
#include "funcoes_CRUD.h"
// Criando uma variavel do tipo struct Livros:
typedef struct {
    int id;
    char nome[TAMANHO_ARRAY];
    char autor[TAMANHO_ARRAY];
    char editora[TAMANHO_ARRAY];
    int paginas;
    int ano;
} Livros;
#endif