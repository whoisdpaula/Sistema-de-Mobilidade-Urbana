#ifndef GRAFO_H
#define GRAFO_H

#include "estruturas.h"

// Inicializa o grafo
void inicializarGrafo(Grafo *grafo);

// Carrega automaticamente os dados iniciais para os testes
void carregarDadosTeste(Grafo *grafo, ArvoreBusca *arvore);

// Procura um local pelo codigo
Local *buscarLocalPorCodigo(Grafo *grafo, int codigo);

// Localiza um local pelo codigo ou pelo nome
Local *selecionarLocal(Grafo *grafo, ArvoreBusca *arvore);

// Cadastra um local no grafo e na ABB
void cadastrarLocal(Grafo *grafo, ArvoreBusca *arvore);

// Consulta os locais cadastrados
void consultarLocais(Grafo *grafo);

// Cadastra uma conexao entre dois locais
void cadastrarConexao(Grafo *grafo, ArvoreBusca *arvore);

// Consulta todas as conexoes
void consultarConexoes(Grafo *grafo);

// Verifica percurso utilizando BFS
void verificarPercurso(Grafo *grafo, ArvoreBusca *arvore);

// Calcula menor caminho utilizando Dijkstra
void calcularMenorCaminho(Grafo *grafo, ArvoreBusca *arvore);

// Interdita uma conexao
void interditarConexao(Grafo *grafo, ArvoreBusca *arvore);

// Libera uma conexao interditada
void liberarConexao(Grafo *grafo, ArvoreBusca *arvore);

// Libera a memoria do grafo
void liberarGrafo(Grafo *grafo);

#endif