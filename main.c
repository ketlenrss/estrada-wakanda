#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int main() {
    // Inicializa variáveis
    int Soma = 0;
    int NumTeste = 0;
    
    // Abre arquivo de resultado
    FILE *Resposta = fopen("Resultado.txt", "w");
    if (!Resposta) {
        printf("Erro ao criar arquivo Resultado.txt\n");
        return 1;
    }

    // Carrega dados da estrada
    Estrada *T1 = getEstrada("Teste01.txt");
    if (!T1) {
        fprintf(Resposta, "Erro: Nao foi possivel carregar os dados da estrada\n");
        fclose(Resposta);
        return 1;
    }

    // Verifica estrutura da estrada
    if (!T1->C) {
        fprintf(Resposta, "Erro: Vetor de cidades nao alocado\n");
        free(T1);
        fclose(Resposta);
        return 1;
    }

    // Calcula valores necessários
    double D1 = calcularMenorVizinhanca("Teste01.txt");
    char *C1 = cidadeMenorVizinhanca("Teste01.txt");

    // Realiza testes
    if (T1->T == 10) Soma++;
    NumTeste++;

    if (T1->N == 2) Soma++;
    NumTeste++;

    if (D1 == 3.5) Soma++;
    NumTeste++;

    if (C1 && strcmp(C1, "Birnin Zana") == 0) Soma++;
    NumTeste++;

    // Escreve resultados
    fprintf(Resposta, "Resultados:\n");
    fprintf(Resposta, "Tamanho da estrada: %d\n", T1->T);
    fprintf(Resposta, "Numero de cidades: %d\n", T1->N);
    fprintf(Resposta, "Menor vizinhanca: %.2f\n", D1);
    if (C1) {
        fprintf(Resposta, "Cidade com menor vizinhanca: %s\n", C1);
    }
    fprintf(Resposta, "Pontuacao final: %d/%d\n", Soma, NumTeste);

    // Libera memória
    free(T1->C);
    free(T1);
    if (C1) free(C1);
    fclose(Resposta);

    return 0;
}