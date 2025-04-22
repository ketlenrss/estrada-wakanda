#include <stdio.h>
#include <stdlib.h>
#include "cidades.h"

int main() {
    const char *nomeArquivo = "teste01.txt";

    Estrada *e = getEstrada(nomeArquivo);
    if (!e) {
        printf("Erro ao ler a estrada.\n");
        return 1;
    }

    printf("Comprimento da estrada: %d\n", e->N);
    printf("Número de cidades: %d\n", e->T);

    printf("Cidades lidas:\n");
    for (int i = 0; i < e->T; i++) {
        printf("- %s (Posição: %d)\n", e->C[i].Nome, e->C[i].Posicao);
    }

    // Testar menor vizinhança
    double menor = calcularMenorVizinhanca(nomeArquivo);
    printf("\nMenor vizinhança: %.2lf\n", menor);

    // Testar cidade com menor vizinhança
    char *nomeCidade = cidadeMenorVizinhanca(nomeArquivo);
    printf("Cidade com menor vizinhança: %s\n", nomeCidade);

    free(nomeCidade);
    free(e->C);
    free(e);

    return 0;
}
