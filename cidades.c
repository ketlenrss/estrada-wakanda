#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Função auxiliar para comparar cidades por posição
static int compararCidades(const void *a, const void *b) {
    const Cidade *ca = (const Cidade *)a;
    const Cidade *cb = (const Cidade *)b;
    return ca->Posicao - cb->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) return NULL;

    Estrada *e = malloc(sizeof(Estrada));
    if (!e) {
        fclose(arq);
        return NULL;
    }

    // Lê T (comprimento da estrada) e N (número de cidades)
    if (fscanf(arq, "%d", &e->T) != 1 || fscanf(arq, "%d", &e->N) != 1) {
        fclose(arq);
        free(e);
        return NULL;
    }

    // Verifica restrições básicas
    if (e->T < 3 || e->T > 1000000 || e->N < 2 || e->N > 10000) {
        fclose(arq);
        free(e);
        return NULL;
    }

    e->C = malloc(e->N * sizeof(Cidade));
    if (!e->C) {
        fclose(arq);
        free(e);
        return NULL;
    }

    // Lê cada cidade
    for (int i = 0; i < e->N; i++) {
        if (fscanf(arq, "%d", &e->C[i].Posicao) != 1) {
            free(e->C);
            free(e);
            fclose(arq);
            return NULL;
        }
        
        // Lê o nome (até o final da linha)
        char nome[256];
        int c;
        int j = 0;
        
        // Consome espaços em branco
        while ((c = fgetc(arq)) == ' ' && c != '\n' && c != EOF);
        
        // Lê o nome
        while (c != '\n' && c != EOF && j < 255) {
            nome[j++] = c;
            c = fgetc(arq);
        }
        nome[j] = '\0';
        
        strncpy(e->C[i].Nome, nome, 255);
        e->C[i].Nome[255] = '\0';

        // Verifica posição válida
        if (e->C[i].Posicao <= 0 || e->C[i].Posicao >= e->T) {
            free(e->C);
            free(e);
            fclose(arq);
            return NULL;
        }
    }

    fclose(arq);

    // Verifica posições duplicadas
    qsort(e->C, e->N, sizeof(Cidade), compararCidades);
    for (int i = 1; i < e->N; i++) {
        if (e->C[i].Posicao == e->C[i-1].Posicao) {
            free(e->C);
            free(e);
            return NULL;
        }
    }

    return e;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e || e->N < 1) return -1;

    qsort(e->C, e->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = e->T; // Inicializa com o valor máximo possível

    for (int i = 0; i < e->N; i++) {
        double inicio, fim;
        
        // Calcula início da vizinhança
        if (i == 0) {
            inicio = 0.0;
        } else {
            inicio = (e->C[i].Posicao + e->C[i-1].Posicao) / 2.0;
        }
        
        // Calcula fim da vizinhança
        if (i == e->N - 1) {
            fim = e->T;
        } else {
            fim = (e->C[i].Posicao + e->C[i+1].Posicao) / 2.0;
        }
        
        double vizinhanca = fim - inicio;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(e->C);
    free(e);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e || e->N < 1) return NULL;

    qsort(e->C, e->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = e->T;
    int indiceMenor = 0;

    for (int i = 0; i < e->N; i++) {
        double inicio, fim;
        
        if (i == 0) {
            inicio = 0.0;
        } else {
            inicio = (e->C[i].Posicao + e->C[i-1].Posicao) / 2.0;
        }
        
        if (i == e->N - 1) {
            fim = e->T;
        } else {
            fim = (e->C[i].Posicao + e->C[i+1].Posicao) / 2.0;
        }
        
        double vizinhanca = fim - inicio;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            indiceMenor = i;
        }
    }

    char *nome = malloc(strlen(e->C[indiceMenor].Nome) + 1);
    if (!nome) {
        free(e->C);
        free(e);
        return NULL;
    }
    strcpy(nome, e->C[indiceMenor].Nome);

    free(e->C);
    free(e);
    return nome;
}