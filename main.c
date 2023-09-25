#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <limits.h>

#define MAX 200

typedef struct {
    char origem;
    char destino;
    int peso;
} Aresta;

typedef struct {
    Aresta arestas[MAX];
    int numArestas;
} Grafo;

void adicionarAresta(Grafo *grafo, char origem, char destino, int peso) {
    Aresta aresta;
    aresta.origem = origem;
    aresta.destino = destino;
    aresta.peso = peso;

    printf("Adicionando Aresta -  %c -> %c Peso: %d,\n", origem, destino, peso);
    usleep(100000); 
    grafo->arestas[grafo->numArestas] = aresta;
    grafo->numArestas++;
}

void ImprimeCaminho(int pred[], int i, Grafo *grafo) {
    char arestaInicial = grafo->arestas[0].origem;

    if (pred[i] == -1)
        printf("\033[38;2;255;165;0m%c\033[0m", i + arestaInicial);
    else {
        int u = pred[i];
        int v = i;
        int peso = 0;
        for (int j = 0; j < grafo->numArestas; j++) {
            if (grafo->arestas[j].origem - arestaInicial == u && grafo->arestas[j].destino - arestaInicial == v) {
                peso = grafo->arestas[j].peso;
                break;
            }
        }
        ImprimeCaminho(pred, pred[i], grafo);
        printf(" \033[31m-%d->\033[0m \033[38;2;255;165;0m%c\033[0m", peso, i + arestaInicial);
    }
}



void calcularCaminhoCritico(Grafo *grafo) {
    int dist[MAX];
    int pred[MAX];
    int n = grafo->numArestas;
    
    char arestaInicial = grafo->arestas[0].origem;
    
    for (int i = 0; i < n; i++)
        dist[i] = INT_MIN, pred[i] = -1;
    
    dist[arestaInicial - arestaInicial] = 0;
 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Aresta aresta = grafo->arestas[j];
            int u = aresta.origem - arestaInicial;
            int v = aresta.destino - arestaInicial;
            if (dist[u] != INT_MIN && dist[u] + aresta.peso > dist[v]) {
                dist[v] = dist[u] + aresta.peso;
                pred[v] = u;
            }
        }
    }
 
    int maxDistIndex = 0;
    for (int i = 1; i < n; i++)
        if (dist[i] > dist[maxDistIndex])
            maxDistIndex = i;
            
	printf("\n");
	printf("\n");
    printf("Caminho crítico: ");
    ImprimeCaminho(pred, maxDistIndex, grafo);
    printf("\n");
    printf("\n");
}


void lerArestasDeArquivo(Grafo *grafo, char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    char origem, destino;
    int peso;
    while (fscanf(arquivo, " Origem: %c, Destino: %c, Peso: %d, ", &origem, &destino, &peso) == 3) {
        adicionarAresta(grafo, origem, destino, peso);
    }

    fclose(arquivo);
}


int main() {
    Grafo grafo;
    grafo.numArestas = 0;
    setlocale(LC_ALL, "Portuguese");

    lerArestasDeArquivo(&grafo, "input.txt");
    
    calcularCaminhoCritico(&grafo);

    return 0;
}
