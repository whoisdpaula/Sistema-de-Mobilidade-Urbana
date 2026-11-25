/* Base de locais do sistema */
const locais = [
    { codigo: 1, nome: "Mackenzie" },
    { codigo: 2, nome: "Hospital Central" },
    { codigo: 3, nome: "Shopping Center" },
    { codigo: 4, nome: "Estacao de Metro" },
    { codigo: 5, nome: "Terminal de Onibus" },
    { codigo: 6, nome: "Parque Municipal" },
    { codigo: 7, nome: "Universidade" },
    { codigo: 8, nome: "Biblioteca" },
    { codigo: 9, nome: "Centro Esportivo" },
    { codigo: 10, nome: "Praca Central" },
    { codigo: 11, nome: "Aeroporto" },
    { codigo: 12, nome: "Rodoviaria" },
    { codigo: 13, nome: "Museu" },
    { codigo: 14, nome: "Centro Comercial" },
    { codigo: 15, nome: "Posto de Saude" }
];

/* Base de conexoes do sistema
   Formato: origem, destino, distancia e sentido duplo */
const conexoes = [
    [1, 2, 2.5, true],
    [1, 3, 3.0, true],
    [1, 4, 1.8, false],
    [2, 5, 4.2, true],
    [2, 6, 3.5, false],
    [3, 4, 2.0, true],
    [3, 7, 5.0, true],
    [4, 5, 1.2, true],
    [4, 8, 2.8, false],
    [5, 9, 3.7, true],
    [6, 7, 2.2, true],
    [6, 10, 4.0, false],
    [7, 8, 1.5, true],
    [7, 11, 8.0, false],
    [8, 9, 2.6, true],
    [8, 13, 3.3, false],
    [9, 10, 1.7, true],
    [9, 12, 6.5, false],
    [10, 13, 2.4, true],
    [10, 14, 3.1, true],
    [11, 12, 5.5, true],
    [11, 14, 7.0, false],
    [12, 15, 2.9, true],
    [13, 15, 3.8, true],
    [14, 15, 2.1, true]
];

/* Posições fixas dos locais no mapa */
const posicoes = {
    1:  { x: 100, y: 130 },
    2:  { x: 300, y: 65 },
    3:  { x: 300, y: 230 },
    4:  { x: 500, y: 145 },
    5:  { x: 700, y: 65 },
    6:  { x: 100, y: 390 },
    7:  { x: 300, y: 390 },
    8:  { x: 500, y: 350 },
    9:  { x: 700, y: 260 },
    10: { x: 700, y: 450 },
    11: { x: 900, y: 65 },
    12: { x: 900, y: 260 },
    13: { x: 500, y: 545 },
    14: { x: 900, y: 465 },
    15: { x: 700, y: 570 }
};

/* Carrega os locais nos campos de seleção */
function carregarSeletores() {

    const origem = document.getElementById("origem");
    const destino = document.getElementById("destino");

    locais.forEach(local => {

        const opcaoOrigem = document.createElement("option");

        opcaoOrigem.value = local.codigo;
        opcaoOrigem.textContent = local.codigo + " - " + local.nome;

        const opcaoDestino = document.createElement("option");

        opcaoDestino.value = local.codigo;
        opcaoDestino.textContent = local.codigo + " - " + local.nome;

        origem.appendChild(opcaoOrigem);
        destino.appendChild(opcaoDestino);
    });
}

/* Preenche a tabela de locais */
function carregarTabela() {

    const tabela = document.getElementById("locations-table");

    tabela.innerHTML = "";

    locais.forEach(local => {

        const linha = document.createElement("tr");

        linha.innerHTML = `
            <td>${local.codigo}</td>
            <td>${local.nome}</td>
            <td>
                <span class="location-status">Ativo</span>
            </td>
        `;

        tabela.appendChild(linha);
    });
}

/* Desenha o mapa de conexões */
function desenharRede() {

    const area = document.getElementById("network-area");

    area.innerHTML = "";

    const svgNS = "http://www.w3.org/2000/svg";

    const svg = document.createElementNS(svgNS, "svg");

    svg.setAttribute("viewBox", "0 0 1000 650");
    svg.setAttribute("preserveAspectRatio", "xMidYMid meet");
    svg.setAttribute("class", "network-svg");

    /* Define as setas das conexoes de sentido unico */
    const defs = document.createElementNS(svgNS, "defs");

    defs.innerHTML = `
        <marker
            id="seta-conexao"
            markerWidth="8"
            markerHeight="8"
            refX="6"
            refY="3"
            orient="auto"
        >
            <path
                d="M0,0 L0,6 L6,3 z"
                fill="#a9b7ca"
            />
        </marker>
    `;

    svg.appendChild(defs);

    /* Cria a camada das conexoes */
    const camadaConexoes = document.createElementNS(
        svgNS,
        "g"
    );

    conexoes.forEach(conexao => {

        const origem = posicoes[conexao[0]];
        const destino = posicoes[conexao[1]];

        const linha = document.createElementNS(
            svgNS,
            "line"
        );

        linha.setAttribute("x1", origem.x);
        linha.setAttribute("y1", origem.y);
        linha.setAttribute("x2", destino.x);
        linha.setAttribute("y2", destino.y);

        linha.setAttribute("stroke", "#a9b7ca");
        linha.setAttribute("stroke-width", "3");
        linha.setAttribute("stroke-linecap", "round");

        /* Adiciona seta nas conexoes de sentido unico */
        if (conexao[3] === false) {

            linha.setAttribute(
                "marker-end",
                "url(#seta-conexao)"
            );
        }

        camadaConexoes.appendChild(linha);
    });

    svg.appendChild(camadaConexoes);

    /* Cria a camada dos locais */
    const camadaLocais = document.createElementNS(
        svgNS,
        "g"
    );

    locais.forEach(local => {

        const posicao = posicoes[local.codigo];

        const grupo = document.createElementNS(
            svgNS,
            "g"
        );

        /* Cria o circulo azul que representa o local */
        const circulo = document.createElementNS(
            svgNS,
            "circle"
        );

        circulo.setAttribute("cx", posicao.x);
        circulo.setAttribute("cy", posicao.y);
        circulo.setAttribute("r", "19");
        circulo.setAttribute("fill", "#2563eb");
        circulo.setAttribute("stroke", "#93c5fd");
        circulo.setAttribute("stroke-width", "3");

        /* Adiciona o codigo do local */
        const codigo = document.createElementNS(
            svgNS,
            "text"
        );

        codigo.setAttribute("x", posicao.x);
        codigo.setAttribute("y", posicao.y + 5);
        codigo.setAttribute("text-anchor", "middle");
        codigo.setAttribute("fill", "#ffffff");
        codigo.setAttribute("font-size", "14");
        codigo.setAttribute("font-weight", "bold");
        codigo.textContent = local.codigo;

        /* Adiciona o nome do local */
        const nome = document.createElementNS(
            svgNS,
            "text"
        );

        nome.setAttribute("x", posicao.x);
        nome.setAttribute("y", posicao.y + 38);
        nome.setAttribute("text-anchor", "middle");
        nome.setAttribute("fill", "#ffffff");
        nome.setAttribute("font-size", "12");
        nome.setAttribute("font-weight", "500");
        nome.textContent = local.nome;

        grupo.appendChild(circulo);
        grupo.appendChild(codigo);
        grupo.appendChild(nome);

        camadaLocais.appendChild(grupo);
    });

    svg.appendChild(camadaLocais);

    area.appendChild(svg);
}

/* Atualiza os indicadores da tela */
function atualizarEstatisticas() {

    document.getElementById("total-locais").textContent =
        locais.length;

    document.getElementById("total-conexoes").textContent =
        conexoes.length;

    document.getElementById("total-interditadas").textContent =
        "0";
}

/* Inicializa os dados da interface */
function iniciarSistema() {

    atualizarEstatisticas();

    carregarSeletores();

    carregarTabela();

    desenharRede();
}

/* Inicia o sistema quando a pagina termina de carregar */
document.addEventListener("DOMContentLoaded", iniciarSistema);

