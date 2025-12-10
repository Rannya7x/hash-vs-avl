#include "metrics.h"
#include "stdio.h"
#include "stdlib.h"

//Inciliazar metricas
Metrics* init_metrics(){
    Metrics* metrics = (Metrics*)malloc(sizeof(Metrics));
    if(metrics == NULL){
        printf("Erro de alocação de memória para Metrics\n");
        return NULL;
    }
    metrics->custo_agregado = 0;
    metrics->numero_elementos = 0;
    metrics->hash_movimentacoes = 0;
    metrics->hash_fator_carga = 0.0;
    metrics->hash_clusterizacao = 0.0;
    metrics->hash_redimensionamentos = 0;
    metrics->avl_rotacoes = 0;
    metrics->avl_altura = 0;
    return metrics;
}

//Liberar metricas
void free_metrics(Metrics* metrics){
    if(metrics==NULL){
        printf("Métricas já estão nulas ao tentar liberar memória\n");
        return;
    }
    free(metrics);
}