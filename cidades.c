#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"  // Usa a estrutura e funções já declaradas no .h

// Função para comparar cidades pela posição (para usar no qsort)
int comparar(const void *a, const void *b) {
    Cidade *c1 = (Cidade *)a;
    Cidade *c2 = (Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

// Lê o arquivo e retorna o ponteiro para a estrutura Estrada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) return NULL;

    int T, N;
    if (fscanf(arq, "%d", &T) != 1 || fscanf(arq, "%d", &N) != 1) {
        fclose(arq);
        return NULL;
    }

    // Restrições de T e N
    if (T < 3 || T > 1000000 || N < 2 || N > 10000) {
        fclose(arq);
        return NULL;
    }

    Estrada *e = malloc(sizeof(Estrada));
    if (!e) {
        fclose(arq);
        return NULL;
    }

    e->T = T;
    e->N = N;
    e->C = malloc(N * sizeof(Cidade));
    if (!e->C) {
        free(e);
        fclose(arq);
        return NULL;
    }

    int *posicoes = malloc(N * sizeof(int)); // Para checar duplicatas
    if (!posicoes) {
        free(e->C);
        free(e);
        fclose(arq);
        return NULL;
    }

    int valido = 1;

    for (int i = 0; i < N; i++) {
        if (fscanf(arq, "%d", &e->C[i].Posicao) != 1) {
            valido = 0;
            break;
        }

        fgetc(arq); // Consome o espaço entre número e nome
        if (!fgets(e->C[i].Nome, 256, arq)) {
            valido = 0;
            break;
        }

        e->C[i].Nome[strcspn(e->C[i].Nome, "\n")] = '\0';

        // Restrições: 0 < Xi < T
        if (e->C[i].Posicao <= 0 || e->C[i].Posicao >= T) {
            valido = 0;
            break;
        }

        // Verifica se Xi é único
        for (int j = 0; j < i; j++) {
            if (e->C[i].Posicao == posicoes[j]) {
                valido = 0;
                break;
            }
        }

        posicoes[i] = e->C[i].Posicao;
    }

    free(posicoes);

    if (!valido) {
        free(e->C);
        free(e);
        fclose(arq);
        return NULL;
    }

    fclose(arq);
    return e;
}


// Calcula a menor vizinhança entre todas as cidades
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1;

    qsort(e->C, e->T, sizeof(Cidade), comparar); // ordena cidades por posição

    double menor = e->N;
    for (int i = 0; i < e->T; i++) {
        double esquerda = (i == 0) ? 0 : (e->C[i].Posicao + e->C[i - 1].Posicao) / 2.0;
        double direita = (i == e->T - 1) ? e->N : (e->C[i].Posicao + e->C[i + 1].Posicao) / 2.0;
        double vizinhanca = direita - esquerda;

        if (vizinhanca < menor) {
            menor = vizinhanca;
        }
    }

    free(e->C);
    free(e);
    return menor;
}

// Retorna o nome da cidade com a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    qsort(e->C, e->T, sizeof(Cidade), comparar); // ordena cidades por posição

    double menor = e->N;
    int indice = 0;

    for (int i = 0; i < e->T; i++) {
        double esquerda = (i == 0) ? 0 : (e->C[i].Posicao + e->C[i - 1].Posicao) / 2.0;
        double direita = (i == e->T - 1) ? e->N : (e->C[i].Posicao + e->C[i + 1].Posicao) / 2.0;
        double vizinhanca = direita - esquerda;

        if (vizinhanca < menor) {
            menor = vizinhanca;
            indice = i;
        }
    }

    char *nome = malloc(strlen(e->C[indice].Nome) + 1);
    strcpy(nome, e->C[indice].Nome);

    free(e->C);
    free(e);
    return nome;
}
