#include <stdio.h>
#include <stdlib.h>

#define TAM_TABULEIRO 10
#define TAM_NAVIO     3
#define AGUA          0
#define NAVIO_VALOR   3

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

/* Verifica se (linha, coluna) está dentro do tabuleiro */
int dentro_limites(int linha, int coluna)
{
    return (linha >= 0 && linha < TAM_TABULEIRO &&
            coluna >= 0 && coluna < TAM_TABULEIRO);
}

/* Função genérica para verificar se é possível posicionar um navio
   em qualquer direção (horizontal, vertical ou diagonal). 
   dlinha, dcoluna definem a direção:
   - horizontal: (dlinha = 0, dcoluna = 1)
   - vertical:   (dlinha = 1, dcoluna = 0)
   - diag desc:  (dlinha = 1, dcoluna = 1)  -> tipo tabuleiro[i][i]
   - diag asc:   (dlinha = 1, dcoluna = -1) -> tipo tabuleiro[i][9-i]
*/
int pode_posicionar(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                    int linha_ini, int col_ini,
                    int dlinha, int dcoluna,
                    int tamanho)
{
    int i;
    int linha, coluna;

    /* Verifica se a última posição do navio cai dentro do tabuleiro */
    int linha_fim = linha_ini + (tamanho - 1) * dlinha;
    int col_fim   = col_ini   + (tamanho - 1) * dcoluna;

    if (!dentro_limites(linha_ini, col_ini))
        return 0;
    if (!dentro_limites(linha_fim, col_fim))
        return 0;

    /* Verifica se todas as posições estão livres (AGUA) */
    for (i = 0; i < tamanho; i++) {
        linha = linha_ini + i * dlinha;
        coluna = col_ini + i * dcoluna;

        if (tabuleiro[linha][coluna] != AGUA) {
            return 0; /* Há sobreposição */
        }
    }

    return 1; /* Pode posicionar */
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

    /* Vetor base para os navios (todos de mesmo tamanho) */
    int navio[TAM_NAVIO] = { NAVIO_VALOR, NAVIO_VALOR, NAVIO_VALOR };

    /* ====================================================
       2) Definição das posições dos 4 navios
          - 2 navios horizontais/verticais
          - 2 navios diagonais
       As coordenadas aqui podem ser alteradas à vontade,
       desde que passem na validação.
       ==================================================== */

    /* Navio 1: horizontal (linha fixa, coluna aumenta) */
    int n1_linha = 0;
    int n1_col   = 0;
    int n1_dl    = 0;  /* dlinha */
    int n1_dc    = 1;  /* dcoluna */

    /* Navio 2: vertical (coluna fixa, linha aumenta) */
    int n2_linha = 5;
    int n2_col   = 5;
    int n2_dl    = 1;
    int n2_dc    = 0;

    /* Navio 3: diagonal descendente (linha e coluna aumentam) */
    int n3_linha = 2;
    int n3_col   = 7;
    int n3_dl    = 1;
    int n3_dc    = 1;

    /* Navio 4: diagonal ascendente (linha aumenta, coluna diminui) */
    int n4_linha = 7;
    int n4_col   = 2;
    int n4_dl    = 1;
    int n4_dc    = -1;

    /* ==============================
       3) Validação e posicionamento
       ============================== */

    /* Navio 1 - horizontal */
    if (!pode_posicionar(tabuleiro, n1_linha, n1_col, n1_dl, n1_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 1 (horizontal) em (%d, %d).\n",
               n1_linha, n1_col);
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n1_linha, n1_col, n1_dl, n1_dc, TAM_NAVIO);

    /* Navio 2 - vertical */
    if (!pode_posicionar(tabuleiro, n2_linha, n2_col, n2_dl, n2_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 2 (vertical) em (%d, %d).\n",
               n2_linha, n2_col);
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n2_linha, n2_col, n2_dl, n2_dc, TAM_NAVIO);

    /* Navio 3 - diagonal descendente */
    if (!pode_posicionar(tabuleiro, n3_linha, n3_col, n3_dl, n3_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 3 (diagonal desc) em (%d, %d).\n",
               n3_linha, n3_col);
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n3_linha, n3_col, n3_dl, n3_dc, TAM_NAVIO);

    /* Navio 4 - diagonal ascendente */
    if (!pode_posicionar(tabuleiro, n4_linha, n4_col, n4_dl, n4_dc, TAM_NAVIO)) {
        printf("Erro ao posicionar Navio 4 (diagonal asc) em (%d, %d).\n",
               n4_linha, n4_col);
        return 1;
    }
    posicionar_navio(tabuleiro, navio, n4_linha, n4_col, n4_dl, n4_dc, TAM_NAVIO);

    /* ==============================
       4) Exibir o tabuleiro completo
       ============================== */
    printf("Tabuleiro com 4 navios (0 = agua, 3 = navio):\n\n");
    exibir_tabuleiro(tabuleiro);

    return 0;
}
