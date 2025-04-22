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
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    Estrada *e = malloc(sizeof(Estrada));
    fscanf(arq, "%d %d", &e->N, &e->T);

    e->C = malloc(e->T * sizeof(Cidade));

    for (int i = 0; i < e->T; i++) {
        fscanf(arq, "%d", &e->C[i].Posicao);
        fgetc(arq); // lê o espaço
        fgets(e->C[i].Nome, 256, arq);
        e->C[i].Nome[strcspn(e->C[i].Nome, "\n")] = '\0'; // remove \n do final
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
