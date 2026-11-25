#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "grafo.h"
#include "abb.h"

// Le uma opcao numerica do menu
int lerOpcaoMenu(int *opcao) {

    char entrada[100];
    char *fim;
    long valor;

    while (1) {

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            return 0;
        }

        valor = strtol(entrada, &fim, 10);

        if (fim == entrada) {
            printf("\nOpcao invalida! Digite um numero de 0 a 9.\n");
            printf("Escolha uma opcao: ");
            continue;
        }

        while (*fim == ' ' || *fim == '\t' ||
               *fim == '\r' || *fim == '\n') {
            fim++;
        }

        if (*fim != '\0' || valor < 0 || valor > 9) {
            printf("\nOpcao invalida! Digite um numero de 0 a 9.\n");
            printf("Escolha uma opcao: ");
            continue;
        }

        *opcao = (int) valor;
        return 1;
    }
}

// Funcao principal do sistema
int main() {

    Grafo cidade;
    ArvoreBusca arvore;

    // Inicializa o grafo
    inicializarGrafo(&cidade);

    // Inicializa a arvore binaria de busca
    arvore.raiz = NULL;

    // Carrega os dados iniciais para os testes
    carregarDadosTeste(&cidade, &arvore);

    int opcao;

    do {

        printf("\n===== SISTEMA DE MOBILIDADE URBANA =====\n");
        printf("1 - Cadastrar local\n");
        printf("2 - Consultar locais\n");
        printf("3 - Cadastrar conexao\n");
        printf("4 - Consultar conexoes\n");
        printf("5 - Verificar percurso\n");
        printf("6 - Calcular menor caminho\n");
        printf("7 - Pesquisar local por nome\n");
        printf("8 - Interditar conexao\n");
        printf("9 - Liberar conexao\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (!lerOpcaoMenu(&opcao)) {
            printf("\nEntrada encerrada. Finalizando o sistema...\n");
            break;
        }

        switch (opcao) {

            case 1:
                cadastrarLocal(&cidade, &arvore);
                break;

            case 2:
                consultarLocais(&cidade);
                break;

            case 3:
                cadastrarConexao(&cidade, &arvore);
                break;

            case 4:
                consultarConexoes(&cidade);
                break;

            case 5:
                verificarPercurso(&cidade, &arvore);
                break;

            case 6:
                calcularMenorCaminho(&cidade, &arvore);
                break;

            case 7:
                pesquisarPorNome(&arvore);
                break;

            case 8:
                interditarConexao(&cidade, &arvore);
                break;

            case 9:
                liberarConexao(&cidade, &arvore);
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao inexistente!\n");
                break;
        }

    } while (opcao != 0);

    // Libera primeiro a arvore e suas referencias
    liberarABB(&arvore);

    // Depois libera os locais e conexoes do grafo
    liberarGrafo(&cidade);

    return 0;
}
