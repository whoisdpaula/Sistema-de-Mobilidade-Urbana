# Sistema de Mobilidade Urbana

## 1. Sobre o projeto

O Sistema de Mobilidade Urbana é um programa desenvolvido em linguagem C com o objetivo de representar e gerenciar a conexão entre diferentes locais de uma cidade.

O sistema permite cadastrar locais, estabelecer conexões entre eles, consultar informações, verificar a existência de percursos e calcular o menor caminho entre dois pontos.

O projeto utiliza conceitos de estruturas de dados, como grafos, listas encadeadas e árvore binária de busca, além de algoritmos de busca e caminho mínimo.

## 2. Funcionalidades

O sistema oferece as seguintes funcionalidades:

- Cadastro de locais com código e nome.
- Consulta dos locais cadastrados.
- Cadastro de conexões entre locais.
- Conexões de mão única e mão dupla.
- Consulta das conexões existentes.
- Verificação de percurso entre dois locais utilizando BFS.
- Cálculo do menor caminho utilizando o algoritmo de Dijkstra.
- Pesquisa de locais pelo nome utilizando ABB.
- Interdição de conexões.
- Liberação de conexões interditadas.
- Pesquisa de locais pelo código ou pelo nome.
- Validação das entradas fornecidas pelo usuário.
- Carregamento automático da base inicial de testes.

## 3. Estruturas de dados utilizadas

### Grafo

O grafo representa os locais da cidade e as conexões existentes entre eles.

Cada local corresponde a um vértice, enquanto cada conexão representa uma aresta. O grafo é implementado por meio de listas de adjacência.

### Lista encadeada

As listas encadeadas são utilizadas para armazenar os locais e suas respectivas conexões, permitindo a inclusão dinâmica de novos elementos.

### Árvore Binária de Busca (ABB)

A ABB é utilizada para realizar pesquisas de locais pelo nome, organizando os registros de acordo com a ordem alfabética.

## 4. Algoritmos utilizados

- **BFS (Busca em Largura):** verifica se existe um percurso entre dois locais.
- **Dijkstra:** calcula o caminho de menor distância entre dois locais, considerando apenas conexões disponíveis.
- **ABB:** permite pesquisar locais pelo nome.

## 5. Base de testes

O programa carrega automaticamente uma base inicial contendo:

- 15 locais cadastrados.
- 25 conexões entre os locais.

A base permite realizar testes das funcionalidades sem a necessidade de cadastrar todos os dados manualmente.

Também é possível adicionar novos locais e conexões durante a execução.

## 6. Interdição de conexões

O sistema permite interditar e liberar conexões existentes.

As conexões interditadas são desconsideradas pelos algoritmos BFS e Dijkstra, possibilitando a identificação de rotas alternativas.

Nas conexões de mão dupla, a alteração também é aplicada ao sentido inverso.

## 7. Linguagem e recursos utilizados

- Linguagem C.
- Alocação dinâmica de memória.
- Ponteiros.
- Listas encadeadas.
- Grafos.
- Árvore Binária de Busca.
- Algoritmos de busca e menor caminho.

## 8. Organização dos arquivos

- `main.c`: contém a função principal e o menu do sistema.
- `grafo.c`: contém as operações relacionadas ao grafo.
- `grafo.h`: declarações das funções do grafo.
- `abb.c`: implementação da Árvore Binária de Busca.
- `abb.h`: declarações das funções da ABB.
- `estruturas.h`: definição das estruturas utilizadas no projeto.

## 9. Como executar o projeto

### Pré-requisitos

- Visual Studio Code.
- Compilador GCC instalado e configurado no PATH.
- Git Bash ou outro terminal compatível.

### 9.1 Abrir o projeto

Abra a pasta do projeto no Visual Studio Code.

No menu superior, selecione:

**Terminal → Novo Terminal**

Caso esteja utilizando Git Bash, os comandos abaixo poderão ser executados diretamente nele.

### 9.2 Compilar o programa

No terminal, execute:

```bash
gcc main.c grafo.c abb.c -o mobilidade.exe
```

Esse comando compila os arquivos do projeto e gera o executável `mobilidade.exe`.

### 9.3 Executar o programa

Após a compilação, execute:

```bash
./mobilidade.exe
```

O menu do Sistema de Mobilidade Urbana será apresentado no terminal.

### 9.4 Executar novamente após alterações

Sempre que houver alterações no código, compile novamente:

```bash
gcc main.c grafo.c abb.c -o mobilidade.exe
```

Em seguida, execute:

```bash
./mobilidade.exe
```

## 10. Como atualizar o projeto no GitHub

Após realizar alterações no código e testar o funcionamento, utilize os seguintes comandos no terminal:

### Adicionar as alterações

```bash
git add .
```

### Criar um commit

```bash
git commit -m "Atualiza Sistema de Mobilidade Urbana"
```

### Enviar para o GitHub

```bash
git push
```

Esses comandos permitem registrar as alterações realizadas e atualizar o repositório remoto no GitHub.

## 11. Considerações finais

O desenvolvimento do Sistema de Mobilidade Urbana permitiu aplicar conceitos importantes de Estrutura de Dados e Algoritmos em uma situação prática.

A utilização de grafos possibilitou representar as conexões entre diferentes locais, enquanto a ABB facilitou as pesquisas pelo nome.

Os algoritmos BFS e Dijkstra permitiram realizar operações de análise de percursos e cálculo de menores distâncias.

O projeto também demonstrou a importância da alocação dinâmica de memória, dos ponteiros e das listas encadeadas no desenvolvimento de programas em linguagem C.
