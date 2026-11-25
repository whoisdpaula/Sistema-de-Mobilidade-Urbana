
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"

// Le um nome e repete a pergunta se estiver vazio
static int lerNomeABB(char nome[], int tamanho) {

    while (1) {

        printf("\nDigite o nome do local: ");

        if (fgets(nome, tamanho, stdin) == NULL) {
            return 0;
        }

        nome[strcspn(nome, "\n")] = '\0';

        if (strlen(nome) == 0) {
            printf("Nome invalido! Tente novamente.\n");
            continue;
        }

        return 1;
    }
}

// Compara dois nomes ignorando maiusculas e minusculas
int compararNomes(char nome1[], char nome2[]) {

    int i = 0;

    while (nome1[i] != '\0' && nome2[i] != '\0') {

        char letra1 = tolower((unsigned char) nome1[i]);
        char letra2 = tolower((unsigned char) nome2[i]);

        if (letra1 < letra2) {
            return -1;
        }

        if (letra1 > letra2) {
            return 1;
        }

        i++;
    }

    if (nome1[i] == '\0' && nome2[i] == '\0') {
        return 0;
    }

    if (nome1[i] == '\0') {
        return -1;
    }

    return 1;
}

// Insere um local na arvore ou adiciona sua referencia
// caso ja exista um no com o mesmo nome
int inserirABB(ArvoreBusca *arvore, Local *local) {

    NoABB **atual = &arvore->raiz;

    // Procura a posicao correta na arvore
    while (*atual != NULL) {

        int resultado = compararNomes(local->nome, (*atual)->nome);

        if (resultado == 0) {

            ReferenciaLocal *novaReferencia =
                malloc(sizeof(ReferenciaLocal));

            if (novaReferencia == NULL) {
                printf("Erro ao alocar memoria para referencia!\n");
                return 0;
            }

            novaReferencia->local = local;
            novaReferencia->prox = (*atual)->locais;
            (*atual)->locais = novaReferencia;

            return 1;
        }

        if (resultado < 0) {
            atual = &(*atual)->esq;
        } else {
            atual = &(*atual)->dir;
        }
    }

    // Cria um novo no quando o nome ainda nao existe
    NoABB *novo = malloc(sizeof(NoABB));

    if (novo == NULL) {
        printf("Erro ao alocar memoria para arvore!\n");
        return 0;
    }

    strcpy(novo->nome, local->nome);
    novo->esq = NULL;
    novo->dir = NULL;
    novo->locais = NULL;

    // Cria a primeira referencia do novo no
    ReferenciaLocal *novaReferencia =
        malloc(sizeof(ReferenciaLocal));

    if (novaReferencia == NULL) {
        free(novo);
        printf("Erro ao alocar memoria para referencia!\n");
        return 0;
    }

    novaReferencia->local = local;
    novaReferencia->prox = NULL;

    novo->locais = novaReferencia;

    *atual = novo;

    return 1;
}

// Busca um no da arvore pelo nome
NoABB *buscarNoABB(ArvoreBusca *arvore, char nome[]) {

    NoABB *atual = arvore->raiz;

    // Percorre a arvore procurando o nome
    while (atual != NULL) {

        int resultado = compararNomes(nome, atual->nome);

        if (resultado == 0) {
            return atual;
        }

        if (resultado < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    return NULL;
}

// Pesquisa todos os locais que possuem o nome informado
void pesquisarPorNome(ArvoreBusca *arvore) {

    char nome[100];

    if (arvore->raiz == NULL) {
        printf("\nNenhum local cadastrado na arvore!\n");
        return;
    }

    while (1) {

        if (!lerNomeABB(nome, sizeof(nome))) {
            return;
        }

        NoABB *atual = buscarNoABB(arvore, nome);

        if (atual == NULL) {
            printf("\nNenhum local encontrado com esse nome!\n");
            printf("Tente pesquisar novamente.\n");
            continue;
        }

        printf("\n========== RESULTADO DA PESQUISA ==========\n");

        ReferenciaLocal *referencia = atual->locais;

        // Apresenta todos os locais associados ao nome
        while (referencia != NULL) {

            printf("Codigo: %d\n", referencia->local->codigo);
            printf("Nome: %s\n", referencia->local->nome);
            printf("---------------------------------------\n");

            referencia = referencia->prox;
        }

        return;
    }
}

// Libera as referencias associadas a um no
static void liberarReferencias(ReferenciaLocal *referencia) {

    while (referencia != NULL) {

        ReferenciaLocal *proxima = referencia->prox;

        // Libera somente a referencia, nao o local
        free(referencia);

        referencia = proxima;
    }
}

// Libera os nos da arvore de forma recursiva
static void liberarNosABB(NoABB *no) {

    if (no == NULL) {
        return;
    }

    // Libera a subarvore esquerda
    liberarNosABB(no->esq);

    // Libera a subarvore direita
    liberarNosABB(no->dir);

    // Libera as referencias
    liberarReferencias(no->locais);

    // Libera o no atual
    free(no);
}

// Libera toda a memoria utilizada pela ABB
void liberarABB(ArvoreBusca *arvore) {

    liberarNosABB(arvore->raiz);

    arvore->raiz = NULL;

    printf("\nMemoria da arvore liberada!\n");
}

