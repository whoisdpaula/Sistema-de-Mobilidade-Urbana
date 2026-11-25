
#ifndef ABB_H
#define ABB_H

#include "estruturas.h"

// Compara nomes ignorando maiusculas e minusculas
int compararNomes(char nome1[], char nome2[]);

// Insere um local na arvore
int inserirABB(ArvoreBusca *arvore, Local *local);

// Busca um no da arvore pelo nome
NoABB *buscarNoABB(ArvoreBusca *arvore, char nome[]);

// Pesquisa locais pelo nome
void pesquisarPorNome(ArvoreBusca *arvore);

// Libera a memoria da arvore
void liberarABB(ArvoreBusca *arvore);

#endif
