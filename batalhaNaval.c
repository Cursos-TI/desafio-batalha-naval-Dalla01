#include <stdio.h>
#include <stdlib.h>

#define TAM_TABULEIRO 10
#define TAM_NAVIO     3
#define TAM_HAB       5   /* Tamanho das matrizes de habilidade (5x5) */

#define AGUA          0
#define NAVIO_VALOR   3
#define HABILIDADE    5

/* ------------------------------
   Função para exibir o tabuleiro
   ------------------------------ */
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

/* Verifica se (linha, coluna) está dentro do tabuleiro */
int dentro_limites(int linha, int coluna)
{
    return (linha >= 0 && linha < TAM_TABULEIRO &&
            coluna >= 0 && coluna < TAM_TABULEIRO);
}

/* -----------------------------------------------
   Verificação genérica de posicionamento de navio
   ----------------------------------------------- */
/*
   dlinha, dcoluna:
   - horizontal: (0, 1)
   - vertical:   (1, 0)
   - diag desc:  (1, 1)
   - diag asc:   (1, -1)
*/
int pode_posicionar(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                    int linha_ini, int col_ini,
                    int dlinha, int dcoluna,
                    int tamanho)
{
    int i;
    int linha, coluna;

    int linha_fim = linha_ini + (tamanho - 1) * dlinha;
    int col_fim   = col_ini   + (tamanho - 1) * dcoluna;

    /* Limites da primeira e da última posição */
    if (!dentro_limites(linha_ini, col_ini))
        return 0;
    if (!dentro_limites(linha_fim, col_fim))
        return 0;

    /* Verifica sobreposição */
    for (i = 0; i < tamanho; i++) {
        linha  = linha_ini + i * dlinha;
        coluna = col_ini   + i * dcoluna;

        if (tabuleiro[linha][coluna] != AGUA) {
            return 0;
        }
    }

    return 1;
}

/* Posiciona o navio copiando os valores do vetor unidimensional */
void posicionar_navio(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                      int navio[TAM_NAVIO],
                      int linha_ini, int col_ini,
                      int dlinha, int dcoluna,
                      int tamanho)
{
    int i;
    int linha, coluna;

    for (i = 0; i < tamanho; i++) {
        linha  = linha_ini + i * dlinha;
        coluna = col_ini   + i * dcoluna;
        tabuleiro[linha][coluna] = navio[i];
    }
}

/* ------------------------------------------------
   Construção dinâmica das matrizes de habilidades
   ------------------------------------------------ */

/* Cone apontando para baixo.
   Exemplo conceitual (5x5, usando 3 linhas de cone):

   0 0 1 0 0
   0 1 1 1 0
   1 1 1 1 1
   0 0 0 0 0
   0 0 0 0 0
*/
void construir_cone(int hab[TAM_HAB][TAM_HAB])
{
    int i, j;
    int centro = TAM_HAB / 2;
    int altura = (TAM_HAB + 1) / 2; /* para TAM_HAB=5, altura=3 */

    for (i = 0; i < TAM_HAB; i++) {
        for (j = 0; j < TAM_HAB; j++) {
            /* Por padrão, posição não afetada */
            hab[i][j] = 0;

            /* Condicional: linhas de 0 até altura-1 formam o cone */
            if (i < altura) {
                if (abs(j - centro) <= i) {
                    hab[i][j] = 1;
                }
            }
        }
    }
}

/* Cruz com centro no meio da matriz.
   Exemplo 5x5:

   0 0 1 0 0
   0 0 1 0 0
   1 1 1 1 1
   0 0 1 0 0
   0 0 1 0 0
*/
void construir_cruz(int hab[TAM_HAB][TAM_HAB])
{
    int i, j;
    int centro = TAM_HAB / 2;

    for (i = 0; i < TAM_HAB; i++) {
        for (j = 0; j < TAM_HAB; j++) {
            if (i == centro || j == centro) {
                hab[i][j] = 1;
            } else {
                hab[i][j] = 0;
            }
        }
    }
}

/* Octaedro (losango) com centro no meio da matriz.
   Usando distância Manhattan:

   abs(i - centro) + abs(j - centro) <= raio

   Para TAM_HAB = 5, raio = 2:

   0 0 1 0 0
   0 1 1 1 0
   1 1 1 1 1
   0 1 1 1 0
   0 0 1 0 0
*/
void construir_octaedro(int hab[TAM_HAB][TAM_HAB])
{
    int i, j;
    int centro = TAM_HAB / 2;
    int raio   = TAM_HAB / 2;

    for (i = 0; i < TAM_HAB; i++) {
        for (j = 0; j < TAM_HAB; j++) {
            int dist = abs(i - centro) + abs(j - centro);
            if (dist <= raio) {
                hab[i][j] = 1;
            } else {
                hab[i][j] = 0;
            }
        }
    }
}

/* --------------------------------------
   Aplica a matriz de habilidade no tabuleiro
   -------------------------------------- */
/*
   origem_linha, origem_coluna: coordenadas do centro da habilidade no tabuleiro.
   A matriz TAM_HAB x TAM_HAB é "centrada" nessa origem.

   - Onde hab[i][j] == 1 e estiver dentro do tabuleiro:
       - Se for AGUA (0), vira HABILIDADE (5)
       - Se for NAVIO (3), mantemos o navio (3) para ainda enxergar o navio.
*/
void aplicar_habilidade(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                        int hab[TAM_HAB][TAM_HAB],
                        int origem_linha, int origem_coluna)
{
    int i, j;
    int centro = TAM_HAB / 2;

    for (i = 0; i < TAM_HAB; i++) {
        for (j = 0; j < TAM_HAB; j++) {
            if (hab[i][j] == 1) { /* Condicional: só aplica onde há efeito */
                int linha_tab = origem_linha + (i - centro);
                int col_tab   = origem_coluna + (j - centro);

                /* Garante que está dentro do tabuleiro */
                if (dentro_limites(linha_tab, col_tab)) {
                    /* Só marcamos como habilidade se ainda for água */
                    if (tabuleiro[linha_tab][col_tab] == AGUA) {
                        tabuleiro[linha_tab][col_tab] = HABILIDADE;
                    }
                    /* Se for NAVIO (3), mantemos como 3 para diferenciar bem */
                }
            }
        }
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

    /* ---------------------------
       2) Posicionamento dos navios
       --------------------------- */
    int navio[TAM_NAVIO] = { NAVIO_VALOR, NAVIO_VALOR, NAVIO_VALOR };

    /* Navio 1: horizontal */
    int n1_linha = 0;
    int n1_col   = 0;
    int n1_dl    = 0;  /* dlinha */
    int n1_dc    = 1;  /* dcoluna */

    /* Navio 2: vertical */
    int n2_linha = 5;
    int n2_col   = 5;
    int n2_dl    = 1;
    int n2_dc    = 0;

    /* Navio 3: diagonal descendente */
    int n3_linha = 2;
    int n3_col   = 7;
    int n3_dl    = 1;
    int n3_dc    = 1;

    /* Navio 4: diagonal ascendente */
    int n4_linha = 7;
    int n4_col   = 2;
    int n4_dl    = 1;
    int n4_dc    = -1;

    /* Validação e posicionamento */

    if (!pode_posicionar(tabuleiro, n1_linha, n1_col, n1_dl, n1_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 1.\n");
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n1_linha, n1_col, n1_dl, n1_dc, TAM_NAVIO);

    if (!pode_posicionar(tabuleiro, n2_linha, n2_col, n2_dl, n2_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 2.\n");
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n2_linha, n2_col, n2_dl, n2_dc, TAM_NAVIO);

    if (!pode_posicionar(tabuleiro, n3_linha, n3_col, n3_dl, n3_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 3.\n");
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n3_linha, n3_col, n3_dl, n3_dc, TAM_NAVIO);

    if (!pode_posicionar(tabuleiro, n4_linha, n4_col, n4_dl, n4_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 4.\n");
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n4_linha, n4_col, n4_dl, n4_dc, TAM_NAVIO);

    /* ---------------------------------------------
       3) Construção das matrizes de habilidades (5x5)
       --------------------------------------------- */
    int hab_cone[TAM_HAB][TAM_HAB];
    int hab_cruz[TAM_HAB][TAM_HAB];
    int hab_octa[TAM_HAB][TAM_HAB];

    construir_cone(hab_cone);
    construir_cruz(hab_cruz);
    construir_octaedro(hab_octa);

    /* ---------------------------------------------
       4) Aplicar habilidades no tabuleiro
       --------------------------------------------- */
    /* Pontos de origem no tabuleiro (centro das matrizes) */
    int origem_cone_linha = 3, origem_cone_col = 3;
    int origem_cruz_linha = 6, origem_cruz_col = 1;
    int origem_octa_linha = 4, origem_octa_col = 8;

    aplicar_habilidade(tabuleiro, hab_cone, origem_cone_linha, origem_cone_col);
    aplicar_habilidade(tabuleiro, hab_cruz, origem_cruz_linha, origem_cruz_col);
    aplicar_habilidade(tabuleiro, hab_octa, origem_octa_linha, origem_octa_col);

    /* ---------------------------------------------
       5) Exibir o tabuleiro final
       --------------------------------------------- */
    printf("Legenda: 0 = agua, 3 = navio, 5 = area de habilidade\n\n");
    exibir_tabuleiro(tabuleiro);

    return 0;
}
