#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Estrutura que representa uma conexão entre dois locais
typedef struct Conexao {
    int destino;
    float distancia;
    float tempo;
    int sentidoDuplo;
    int interditada;
    struct Conexao *prox;
} Conexao;

// Declaração antecipada da estrutura Local
typedef struct Local Local;

// Estrutura que representa um local da cidade
struct Local {
    int codigo;
    char nome[100];
    Conexao *conexoes;
    Local *prox;
};

// Estrutura que representa o grafo da cidade
typedef struct {
    Local *locais;
    int quantidade;
} Grafo;

// Estrutura que armazena referências de locais com o mesmo nome
typedef struct ReferenciaLocal {
    Local *local;
    struct ReferenciaLocal *prox;
} ReferenciaLocal;

// Estrutura que representa um nó da Árvore Binária de Busca
typedef struct NoABB {
    char nome[100];
    ReferenciaLocal *locais;
    struct NoABB *esq;
    struct NoABB *dir;
} NoABB;

// Estrutura que representa a Árvore Binária de Busca
typedef struct {
    NoABB *raiz;
} ArvoreBusca;

#endif