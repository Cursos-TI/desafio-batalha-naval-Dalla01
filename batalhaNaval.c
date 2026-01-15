#include <stdio.h>
#include <stdlib.h>

#define TAM_TABULEIRO 10
#define TAM_NAVIO     3
#define AGUA          0
#define NAVIO_VALOR   3

/* Função para verificar se é possível posicionar um navio horizontalmente */
int pode_posicionar_horizontal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                               int linha, int coluna_inicial, int tamanho)
{
    int i;

    /* Verifica se está dentro dos limites */
    if (linha < 0 || linha >= TAM_TABULEIRO)
        return 0;
    if (coluna_inicial < 0 || coluna_inicial + tamanho > TAM_TABULEIRO)
        return 0;

    /* Verifica se não há sobreposição */
    for (i = 0; i < tamanho; i++) {
        if (tabuleiro[linha][coluna_inicial + i] != AGUA)
            return 0;
    }

    return 1;
}

/* Função para verificar se é possível posicionar um navio verticalmente */
int pode_posicionar_vertical(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                             int linha_inicial, int coluna, int tamanho)
{
    int i;

    /* Verifica se está dentro dos limites */
    if (coluna < 0 || coluna >= TAM_TABULEIRO)
        return 0;
    if (linha_inicial < 0 || linha_inicial + tamanho > TAM_TABULEIRO)
        return 0;

    /* Verifica se não há sobreposição */
    for (i = 0; i < tamanho; i++) {
        if (tabuleiro[linha_inicial + i][coluna] != AGUA)
            return 0;
    }

    return 1;
}

/* Função para exibir o tabuleiro */
void exibir_tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO])
{
    int i, j;

    /* Cabeçalho de colunas */
    printf("   ");
    for (j = 0; j < TAM_TABULEIRO; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (i = 0; i < TAM_TABULEIRO; i++) {
        /* Índice da linha */
        printf("%2d ", i);
        for (j = 0; j < TAM_TABULEIRO; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

int main(void)
{
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    int i, j;

    /* ==============================
       1) Inicializa tabuleiro com 0
       ============================== */
    for (i = 0; i < TAM_TABULEIRO; i++) {
        for (j = 0; j < TAM_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }

    /* ==============================
       2) Navios representados por vetores
       ============================== */
    int navio_horizontal[TAM_NAVIO] = { NAVIO_VALOR, NAVIO_VALOR, NAVIO_VALOR };
    int navio_vertical[TAM_NAVIO]   = { NAVIO_VALOR, NAVIO_VALOR, NAVIO_VALOR };

    /* Coordenadas iniciais (definidas no código)
       Você pode alterar esses valores para testar outros posicionamentos.
    */
    int linha_h = 2;  /* linha do navio horizontal */
    int col_h   = 1;  /* coluna inicial do navio horizontal */

    int linha_v = 5;  /* linha inicial do navio vertical */
    int col_v   = 7;  /* coluna do navio vertical */

    /* ==============================
       3) Validação e posicionamento
       ============================== */

    /* Verifica se o navio horizontal pode ser posicionado */
    if (!pode_posicionar_horizontal(tabuleiro, linha_h, col_h, TAM_NAVIO)) {
        printf("Erro: nao foi possivel posicionar o navio horizontal nas coordenadas (%d, %d).\n",
               linha_h, col_h);
        return 1; /* encerra o programa com erro */
    }

    /* Posiciona o navio horizontal copiando os valores do vetor para a matriz */
    for (i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha_h][col_h + i] = navio_horizontal[i];
    }

    /* Verifica se o navio vertical pode ser posicionado
       (inclui checagem de sobreposicao com o navio horizontal) */
    if (!pode_posicionar_vertical(tabuleiro, linha_v, col_v, TAM_NAVIO)) {
        printf("Erro: nao foi possivel posicionar o navio vertical nas coordenadas (%d, %d).\n",
               linha_v, col_v);
        return 1; /* encerra o programa com erro */
    }

    /* Posiciona o navio vertical copiando os valores do vetor para a matriz */
    for (i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha_v + i][col_v] = navio_vertical[i];
    }

    /* ==============================
       4) Exibir o tabuleiro completo
       ============================== */
    printf("Tabuleiro com navios posicionados (0 = agua, 3 = navio):\n\n");
    exibir_tabuleiro(tabuleiro);

    return 0;
}
