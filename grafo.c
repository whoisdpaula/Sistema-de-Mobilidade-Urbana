
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <errno.h>
#include <math.h>
#include "grafo.h"
#include "abb.h"

// Le um numero inteiro e repete a pergunta em caso de erro
static int lerInteiro(const char mensagem[], int *valor) {

    char entrada[100];
    char *fim;
    long numero;

    while (1) {

        printf("%s", mensagem);

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            return 0;
        }

        numero = strtol(entrada, &fim, 10);

        if (fim == entrada) {
            printf("Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }

        while (*fim == ' ' || *fim == '\t' ||
               *fim == '\r' || *fim == '\n') {
            fim++;
        }

        if (*fim != '\0' ||
            numero < -2147483647L - 1 ||
            numero > 2147483647L) {

            printf("Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }

        *valor = (int) numero;
        return 1;
    }
}

// Le um numero decimal e repete a pergunta em caso de erro
static int lerReal(const char mensagem[], float *valor) {

    char entrada[100];
    char *fim;
    float numero;

    while (1) {

        printf("%s", mensagem);

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            return 0;
        }

        numero = strtof(entrada, &fim);

        if (fim == entrada) {
            printf("Valor invalido! Digite um numero.\n");
            continue;
        }

        while (*fim == ' ' || *fim == '\t' ||
               *fim == '\r' || *fim == '\n') {
            fim++;
        }

        if (*fim != '\0' || !isfinite(numero)) {
            printf("Valor invalido! Digite um numero valido.\n");
            continue;
        }

        *valor = numero;
        return 1;
    }
}

// Le um texto e repete a pergunta se estiver vazio
static int lerTexto(const char mensagem[], char texto[], int tamanho) {

    while (1) {

        printf("%s", mensagem);

        if (fgets(texto, tamanho, stdin) == NULL) {
            return 0;
        }

        texto[strcspn(texto, "\n")] = '\0';

        if (strlen(texto) == 0) {
            printf("Texto invalido! Tente novamente.\n");
            continue;
        }

        return 1;
    }
}

// Inicializa o grafo
void inicializarGrafo(Grafo *grafo) {

    grafo->locais = NULL;
    grafo->quantidade = 0;
}

// Procura um local pelo codigo
Local *buscarLocalPorCodigo(Grafo *grafo, int codigo) {

    Local *atual = grafo->locais;

    while (atual != NULL) {

        if (atual->codigo == codigo) {
            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

// Procura uma conexao pelo codigo do destino
static Conexao *buscarConexao(Local *origem, int destino) {

    Conexao *atual = origem->conexoes;

    while (atual != NULL) {

        if (atual->destino == destino) {
            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

// Localiza um local pelo codigo ou pelo nome
Local *selecionarLocal(Grafo *grafo, ArvoreBusca *arvore) {

    int opcao;

    while (1) {

        printf("\nComo deseja localizar o local?\n");
        printf("1 - Pelo codigo\n");
        printf("2 - Pelo nome\n");

        if (!lerInteiro("Escolha: ", &opcao)) {
            return NULL;
        }

        if (opcao == 1) {

            int codigo;

            if (!lerInteiro("Digite o codigo: ", &codigo)) {
                return NULL;
            }

            Local *local = buscarLocalPorCodigo(grafo, codigo);

            if (local == NULL) {
                printf("Local nao encontrado! Tente novamente.\n");
                continue;
            }

            return local;
        }

        else if (opcao == 2) {

            char nome[100];

            if (!lerTexto("Digite o nome: ", nome, sizeof(nome))) {
                return NULL;
            }

            NoABB *no = buscarNoABB(arvore, nome);

            if (no == NULL) {
                printf("Nenhum local encontrado com esse nome! Tente novamente.\n");
                continue;
            }

            ReferenciaLocal *referencia = no->locais;

            printf("\n========== LOCAIS ENCONTRADOS ==========\n");

            while (referencia != NULL) {

                printf("Codigo: %d | Nome: %s\n",
                       referencia->local->codigo,
                       referencia->local->nome);

                referencia = referencia->prox;
            }

            int codigoEscolhido;
            int encontrado;

            while (1) {

                if (!lerInteiro(
                    "\nDigite o codigo do local desejado: ",
                    &codigoEscolhido)) {
                    return NULL;
                }

                encontrado = 0;
                referencia = no->locais;

                while (referencia != NULL) {

                    if (referencia->local->codigo == codigoEscolhido) {
                        encontrado = 1;
                        break;
                    }

                    referencia = referencia->prox;
                }

                if (encontrado) {
                    return referencia->local;
                }

                printf("Codigo nao pertence aos resultados! Tente novamente.\n");
            }
        }

        else {
            printf("Opcao invalida! Digite 1 ou 2.\n");
        }
    }
}

// Cadastra um local no grafo e na ABB
void cadastrarLocal(Grafo *grafo, ArvoreBusca *arvore) {

    int codigo;
    char nome[100];

    printf("\n========== CADASTRAR LOCAL ==========\n");

    while (1) {

        if (!lerInteiro("Digite o codigo do local: ", &codigo)) {
            return;
        }

        if (codigo <= 0) {
            printf("O codigo deve ser positivo!\n");
            continue;
        }

        if (buscarLocalPorCodigo(grafo, codigo) != NULL) {
            printf("Ja existe um local com esse codigo!\n");
            continue;
        }

        break;
    }

    if (!lerTexto("Digite o nome do local: ", nome, sizeof(nome))) {
        return;
    }

    Local *novo = malloc(sizeof(Local));

    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->conexoes = NULL;
    novo->prox = NULL;

    if (!inserirABB(arvore, novo)) {
        printf("Erro ao inserir local na arvore!\n");
        free(novo);
        return;
    }

    novo->prox = grafo->locais;
    grafo->locais = novo;

    grafo->quantidade++;

    printf("\nLocal cadastrado com sucesso!\n");
}

// Consulta os locais cadastrados
void consultarLocais(Grafo *grafo) {

    Local *atual = grafo->locais;

    if (atual == NULL) {
        printf("\nNenhum local cadastrado!\n");
        return;
    }

    printf("\n========== LOCAIS CADASTRADOS ==========\n");

    while (atual != NULL) {

        printf("Codigo: %d\n", atual->codigo);
        printf("Nome: %s\n", atual->nome);
        printf("---------------------------------------\n");

        atual = atual->prox;
    }
}

// Cadastra uma conexao entre dois locais
void cadastrarConexao(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade < 2) {
        printf("\nCadastre pelo menos dois locais antes de criar conexoes!\n");
        return;
    }

    printf("\n========== LOCAL DE ORIGEM ==========\n");

    Local *origem = selecionarLocal(grafo, arvore);

    if (origem == NULL) {
        return;
    }

    Local *destino;

    while (1) {

        printf("\n========== LOCAL DE DESTINO ==========\n");

        destino = selecionarLocal(grafo, arvore);

        if (destino == NULL) {
            return;
        }

        if (origem->codigo == destino->codigo) {
            printf("Nao e permitido conectar um local a ele mesmo!\n");
            continue;
        }

        if (buscarConexao(origem, destino->codigo) != NULL) {
            printf("Essa conexao ja esta cadastrada!\n");
            continue;
        }

        break;
    }

    float distancia;
    float tempo;
    int sentido;

    while (1) {

        if (!lerReal("Digite a distancia em km: ", &distancia)) {
            return;
        }

        if (distancia < 0) {
            printf("A distancia nao pode ser negativa!\n");
            continue;
        }

        break;
    }

    while (1) {

        if (!lerReal("Digite o tempo estimado em minutos: ", &tempo)) {
            return;
        }

        if (tempo < 0) {
            printf("O tempo nao pode ser negativo!\n");
            continue;
        }

        break;
    }

    while (1) {

        printf("\nTipo de conexao:\n");
        printf("1 - Mao unica\n");
        printf("2 - Mao dupla\n");

        if (!lerInteiro("Escolha: ", &sentido)) {
            return;
        }

        if (sentido != 1 && sentido != 2) {
            printf("Opcao invalida! Digite 1 ou 2.\n");
            continue;
        }

        if (sentido == 2 &&
            buscarConexao(destino, origem->codigo) != NULL) {

            printf("Ja existe uma conexao no sentido contrario!\n");
            printf("Escolha outra opcao ou selecione outro destino.\n");
            continue;
        }

        break;
    }

    Conexao *ida = malloc(sizeof(Conexao));

    if (ida == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    ida->destino = destino->codigo;
    ida->distancia = distancia;
    ida->tempo = tempo;
    ida->sentidoDuplo = (sentido == 2);
    ida->interditada = 0;
    ida->prox = NULL;

    Conexao *volta = NULL;

    if (sentido == 2) {

        volta = malloc(sizeof(Conexao));

        if (volta == NULL) {
            free(ida);
            printf("Erro ao alocar memoria!\n");
            return;
        }

        volta->destino = origem->codigo;
        volta->distancia = distancia;
        volta->tempo = tempo;
        volta->sentidoDuplo = 1;
        volta->interditada = 0;
        volta->prox = NULL;
    }

    ida->prox = origem->conexoes;
    origem->conexoes = ida;

    if (volta != NULL) {
        volta->prox = destino->conexoes;
        destino->conexoes = volta;
    }

    printf("\nConexao cadastrada com sucesso!\n");
}

// Consulta todas as conexoes cadastradas
void consultarConexoes(Grafo *grafo) {

    Local *origem = grafo->locais;

    if (origem == NULL) {
        printf("\nNenhum local cadastrado!\n");
        return;
    }

    printf("\n========== CONEXOES CADASTRADAS ==========\n");

    int encontrou = 0;

    while (origem != NULL) {

        Conexao *atual = origem->conexoes;

        while (atual != NULL) {

            Local *destino =
                buscarLocalPorCodigo(grafo, atual->destino);

            if (destino != NULL) {

                encontrou = 1;

                printf("\nOrigem: %s (%d)\n",
                       origem->nome, origem->codigo);

                printf("Destino: %s (%d)\n",
                       destino->nome, destino->codigo);

                printf("Distancia: %.2f km\n", atual->distancia);
                printf("Tempo: %.2f minutos\n", atual->tempo);

                if (atual->sentidoDuplo == 1) {
                    printf("Sentido: Mao dupla\n");
                } else {
                    printf("Sentido: Mao unica\n");
                }

                if (atual->interditada == 1) {
                    printf("Situacao: Interditada\n");
                } else {
                    printf("Situacao: Liberada\n");
                }

                printf("---------------------------------------\n");
            }

            atual = atual->prox;
        }

        origem = origem->prox;
    }

    if (!encontrou) {
        printf("Nenhuma conexao cadastrada!\n");
    }
}

// Monta um vetor com os enderecos dos locais
static Local **montarVetorLocais(Grafo *grafo) {

    if (grafo->quantidade <= 0) {
        return NULL;
    }

    Local **vetor = malloc(
        grafo->quantidade * sizeof(Local *)
    );

    if (vetor == NULL) {
        return NULL;
    }

    Local *atual = grafo->locais;
    int i = 0;

    while (atual != NULL && i < grafo->quantidade) {

        vetor[i] = atual;
        i++;

        atual = atual->prox;
    }

    return vetor;
}

// Retorna o indice de um codigo no vetor de locais
static int indiceLocal(Local **locais, int quantidade, int codigo) {

    for (int i = 0; i < quantidade; i++) {

        if (locais[i]->codigo == codigo) {
            return i;
        }
    }

    return -1;
}

// Verifica se existe percurso utilizando busca em largura (BFS)
void verificarPercurso(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade == 0) {
        printf("\nNao existem locais cadastrados!\n");
        return;
    }

    printf("\n========== LOCAL DE ORIGEM ==========\n");

    Local *origem = selecionarLocal(grafo, arvore);

    if (origem == NULL) {
        return;
    }

    printf("\n========== LOCAL DE DESTINO ==========\n");

    Local *destino = selecionarLocal(grafo, arvore);

    if (destino == NULL) {
        return;
    }

    Local **locais = montarVetorLocais(grafo);

    if (locais == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    int quantidade = grafo->quantidade;

    int *fila = malloc(quantidade * sizeof(int));
    int *visitados = calloc(quantidade, sizeof(int));

    if (fila == NULL || visitados == NULL) {
        printf("Erro ao alocar memoria!\n");
        free(locais);
        free(fila);
        free(visitados);
        return;
    }

    int inicio = 0;
    int fim = 0;

    int indiceOrigem = indiceLocal(
        locais, quantidade, origem->codigo
    );

    int indiceDestino = indiceLocal(
        locais, quantidade, destino->codigo
    );

    fila[fim++] = indiceOrigem;
    visitados[indiceOrigem] = 1;

    while (inicio < fim) {

        int atual = fila[inicio++];

        if (atual == indiceDestino) {
            break;
        }

        Conexao *conexao = locais[atual]->conexoes;

        while (conexao != NULL) {

            if (conexao->interditada == 0) {

                int indiceDestinoConexao = indiceLocal(
                    locais, quantidade, conexao->destino
                );

                if (indiceDestinoConexao != -1 &&
                    !visitados[indiceDestinoConexao]) {

                    visitados[indiceDestinoConexao] = 1;
                    fila[fim++] = indiceDestinoConexao;
                }
            }

            conexao = conexao->prox;
        }
    }

    if (visitados[indiceDestino]) {
        printf("\nExiste percurso entre %s e %s!\n",
               origem->nome, destino->nome);
    } else {
        printf("\nNao existe percurso entre %s e %s!\n",
               origem->nome, destino->nome);
    }

    free(fila);
    free(visitados);
    free(locais);
}

// Calcula o percurso de menor distancia utilizando Dijkstra
void calcularMenorCaminho(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade == 0) {
        printf("\nNao existem locais cadastrados!\n");
        return;
    }

    printf("\n========== LOCAL DE ORIGEM ==========\n");

    Local *origem = selecionarLocal(grafo, arvore);

    if (origem == NULL) {
        return;
    }

    printf("\n========== LOCAL DE DESTINO ==========\n");

    Local *destino = selecionarLocal(grafo, arvore);

    if (destino == NULL) {
        return;
    }

    int quantidade = grafo->quantidade;

    Local **locais = montarVetorLocais(grafo);

    if (locais == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    float *distancias = malloc(quantidade * sizeof(float));
    int *visitados = calloc(quantidade, sizeof(int));
    int *anterior = malloc(quantidade * sizeof(int));

    if (distancias == NULL || visitados == NULL ||
        anterior == NULL) {

        printf("Erro ao alocar memoria!\n");

        free(locais);
        free(distancias);
        free(visitados);
        free(anterior);

        return;
    }

    int indiceOrigem = indiceLocal(
        locais, quantidade, origem->codigo
    );

    int indiceDestino = indiceLocal(
        locais, quantidade, destino->codigo
    );

    for (int i = 0; i < quantidade; i++) {

        distancias[i] = FLT_MAX;
        anterior[i] = -1;
    }

    distancias[indiceOrigem] = 0;

    for (int i = 0; i < quantidade; i++) {

        int menor = -1;

        for (int j = 0; j < quantidade; j++) {

            if (!visitados[j] &&
                (menor == -1 ||
                 distancias[j] < distancias[menor])) {

                menor = j;
            }
        }

        if (menor == -1 || distancias[menor] == FLT_MAX) {
            break;
        }

        visitados[menor] = 1;

        if (menor == indiceDestino) {
            break;
        }

        Conexao *conexao = locais[menor]->conexoes;

        while (conexao != NULL) {

            if (conexao->interditada == 0) {

                int destinoIndice = indiceLocal(
                    locais, quantidade, conexao->destino
                );

                if (destinoIndice != -1 &&
                    !visitados[destinoIndice]) {

                    float novaDistancia =
                        distancias[menor] + conexao->distancia;

                    if (novaDistancia < distancias[destinoIndice]) {

                        distancias[destinoIndice] = novaDistancia;
                        anterior[destinoIndice] = menor;
                    }
                }
            }

            conexao = conexao->prox;
        }
    }

    if (distancias[indiceDestino] == FLT_MAX) {

        printf("\nNao existe percurso entre os locais!\n");

    } else {

        int *caminho = malloc(quantidade * sizeof(int));

        if (caminho == NULL) {

            printf("Erro ao alocar memoria para o caminho!\n");

        } else {

            int tamanho = 0;
            int atual = indiceDestino;

            while (atual != -1 && tamanho < quantidade) {

                caminho[tamanho++] = atual;
                atual = anterior[atual];
            }

            printf("\n========== MENOR CAMINHO ==========\n");

            for (int i = tamanho - 1; i >= 0; i--) {

                printf("%s (%d)",
                       locais[caminho[i]]->nome,
                       locais[caminho[i]]->codigo);

                if (i > 0) {
                    printf(" -> ");
                }
            }

            printf("\nDistancia total: %.2f km\n",
                   distancias[indiceDestino]);

            free(caminho);
        }
    }

    free(locais);
    free(distancias);
    free(visitados);
    free(anterior);
}

// Interdita uma conexao
void interditarConexao(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade == 0) {
        printf("\nNao existem locais cadastrados!\n");
        return;
    }

    printf("\n========== LOCAL DE ORIGEM ==========\n");

    Local *origem = selecionarLocal(grafo, arvore);

    if (origem == NULL) {
        return;
    }

    printf("\n========== LOCAL DE DESTINO ==========\n");

    Local *destino = selecionarLocal(grafo, arvore);

    if (destino == NULL) {
        return;
    }

    Conexao *atual = buscarConexao(origem, destino->codigo);

    if (atual == NULL) {
        printf("\nConexao nao encontrada!\n");
        return;
    }

    if (atual->interditada == 1) {
        printf("\nEssa conexao ja esta interditada!\n");
        return;
    }

    atual->interditada = 1;

    if (atual->sentidoDuplo == 1) {

        Conexao *inversa = buscarConexao(
            destino, origem->codigo
        );

        if (inversa != NULL) {
            inversa->interditada = 1;
        }
    }

    printf("\nConexao interditada com sucesso!\n");
}

// Libera uma conexao interditada
void liberarConexao(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade == 0) {
        printf("\nNao existem locais cadastrados!\n");
        return;
    }

    printf("\n========== LOCAL DE ORIGEM ==========\n");

    Local *origem = selecionarLocal(grafo, arvore);

    if (origem == NULL) {
        return;
    }

    printf("\n========== LOCAL DE DESTINO ==========\n");

    Local *destino = selecionarLocal(grafo, arvore);

    if (destino == NULL) {
        return;
    }

    Conexao *atual = buscarConexao(origem, destino->codigo);

    if (atual == NULL) {
        printf("\nConexao nao encontrada!\n");
        return;
    }

    if (atual->interditada == 0) {
        printf("\nEssa conexao ja esta liberada!\n");
        return;
    }

    atual->interditada = 0;

    if (atual->sentidoDuplo == 1) {

        Conexao *inversa = buscarConexao(
            destino, origem->codigo
        );

        if (inversa != NULL) {
            inversa->interditada = 0;
        }
    }

    printf("\nConexao liberada com sucesso!\n");
}

// Libera toda a memoria utilizada pelo grafo
void liberarGrafo(Grafo *grafo) {

    Local *atual = grafo->locais;

    while (atual != NULL) {

        Local *proximoLocal = atual->prox;

        Conexao *conexao = atual->conexoes;

        while (conexao != NULL) {

            Conexao *proximaConexao = conexao->prox;

            free(conexao);

            conexao = proximaConexao;
        }

        free(atual);

        atual = proximoLocal;
    }

    grafo->locais = NULL;
    grafo->quantidade = 0;

    printf("\nMemoria do grafo liberada!\n");
}

// Cadastra um local da base de testes no grafo e na ABB
static int adicionarLocalTeste(Grafo *grafo, ArvoreBusca *arvore,
                               int codigo, const char nome[]) {

    Local *novo = malloc(sizeof(Local));

    if (novo == NULL) {
        return 0;
    }

    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->conexoes = NULL;
    novo->prox = NULL;

    if (!inserirABB(arvore, novo)) {
        free(novo);
        return 0;
    }

    novo->prox = grafo->locais;
    grafo->locais = novo;
    grafo->quantidade++;

    return 1;
}

// Cadastra uma conexao da base de testes
static int adicionarConexaoTeste(Grafo *grafo, int origem,
                                 int destino, float distancia,
                                 float tempo, int sentidoDuplo) {

    Local *localOrigem = buscarLocalPorCodigo(grafo, origem);
    Local *localDestino = buscarLocalPorCodigo(grafo, destino);

    if (localOrigem == NULL || localDestino == NULL) {
        return 0;
    }

    Conexao *ida = malloc(sizeof(Conexao));

    if (ida == NULL) {
        return 0;
    }

    ida->destino = destino;
    ida->distancia = distancia;
    ida->tempo = tempo;
    ida->sentidoDuplo = sentidoDuplo;
    ida->interditada = 0;
    ida->prox = localOrigem->conexoes;

    Conexao *volta = NULL;

    if (sentidoDuplo == 1) {

        volta = malloc(sizeof(Conexao));

        if (volta == NULL) {
            free(ida);
            return 0;
        }

        volta->destino = origem;
        volta->distancia = distancia;
        volta->tempo = tempo;
        volta->sentidoDuplo = 1;
        volta->interditada = 0;
        volta->prox = localDestino->conexoes;
    }

    localOrigem->conexoes = ida;

    if (volta != NULL) {
        localDestino->conexoes = volta;
    }

    return 1;
}

// Carrega automaticamente os dados iniciais para os testes
void carregarDadosTeste(Grafo *grafo, ArvoreBusca *arvore) {

    if (grafo->quantidade > 0) {
        return;
    }

    printf("\nCarregando base de testes...\n");

    // Cadastra os 15 locais
    if (!adicionarLocalTeste(grafo, arvore, 1, "Mackenzie") ||
        !adicionarLocalTeste(grafo, arvore, 2, "Hospital Central") ||
        !adicionarLocalTeste(grafo, arvore, 3, "Shopping Center") ||
        !adicionarLocalTeste(grafo, arvore, 4, "Estacao de Metro") ||
        !adicionarLocalTeste(grafo, arvore, 5, "Terminal de Onibus") ||
        !adicionarLocalTeste(grafo, arvore, 6, "Parque Municipal") ||
        !adicionarLocalTeste(grafo, arvore, 7, "Universidade") ||
        !adicionarLocalTeste(grafo, arvore, 8, "Biblioteca") ||
        !adicionarLocalTeste(grafo, arvore, 9, "Centro Esportivo") ||
        !adicionarLocalTeste(grafo, arvore, 10, "Praca Central") ||
        !adicionarLocalTeste(grafo, arvore, 11, "Aeroporto") ||
        !adicionarLocalTeste(grafo, arvore, 12, "Rodoviaria") ||
        !adicionarLocalTeste(grafo, arvore, 13, "Museu") ||
        !adicionarLocalTeste(grafo, arvore, 14, "Centro Comercial") ||
        !adicionarLocalTeste(grafo, arvore, 15, "Posto de Saude")) {

        printf("Erro ao carregar os locais da base de testes!\n");
        return;
    }

    // Cadastra 25 conexoes
    // 1 = mao dupla; 0 = mao unica

    if (!adicionarConexaoTeste(grafo, 1, 2, 2.5, 8, 1) ||
        !adicionarConexaoTeste(grafo, 1, 3, 3.0, 10, 1) ||
        !adicionarConexaoTeste(grafo, 1, 4, 1.8, 6, 0) ||
        !adicionarConexaoTeste(grafo, 2, 5, 4.2, 12, 1) ||
        !adicionarConexaoTeste(grafo, 2, 6, 3.5, 11, 0) ||
        !adicionarConexaoTeste(grafo, 3, 4, 2.0, 7, 1) ||
        !adicionarConexaoTeste(grafo, 3, 7, 5.0, 15, 1) ||
        !adicionarConexaoTeste(grafo, 4, 5, 1.2, 5, 1) ||
        !adicionarConexaoTeste(grafo, 4, 8, 2.8, 9, 0) ||
        !adicionarConexaoTeste(grafo, 5, 9, 3.7, 13, 1) ||
        !adicionarConexaoTeste(grafo, 6, 7, 2.2, 8, 1) ||
        !adicionarConexaoTeste(grafo, 6, 10, 4.0, 14, 0) ||
        !adicionarConexaoTeste(grafo, 7, 8, 1.5, 6, 1) ||
        !adicionarConexaoTeste(grafo, 7, 11, 8.0, 20, 0) ||
        !adicionarConexaoTeste(grafo, 8, 9, 2.6, 9, 1) ||
        !adicionarConexaoTeste(grafo, 8, 13, 3.3, 11, 0) ||
        !adicionarConexaoTeste(grafo, 9, 10, 1.7, 6, 1) ||
        !adicionarConexaoTeste(grafo, 9, 12, 6.5, 18, 0) ||
        !adicionarConexaoTeste(grafo, 10, 13, 2.4, 8, 1) ||
        !adicionarConexaoTeste(grafo, 10, 14, 3.1, 10, 1) ||
        !adicionarConexaoTeste(grafo, 11, 12, 5.5, 16, 1) ||
        !adicionarConexaoTeste(grafo, 11, 14, 7.0, 19, 0) ||
        !adicionarConexaoTeste(grafo, 12, 15, 2.9, 9, 1) ||
        !adicionarConexaoTeste(grafo, 13, 15, 3.8, 12, 1) ||
        !adicionarConexaoTeste(grafo, 14, 15, 2.1, 7, 1)) {

        printf("Erro ao carregar as conexoes da base de testes!\n");
        return;
    }

    printf("\nBase de testes carregada com sucesso!\n");
    printf("Locais cadastrados: %d\n", grafo->quantidade);
    printf("Conexoes cadastradas: 25\n");
}
