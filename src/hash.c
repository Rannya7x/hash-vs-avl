#include "./include/hash.h"
#include "stdio.h"
#include "stdlib.h"
#include "./include/listase.h"
#include "assert.h"
#include "./include/metrics.h"
#include "./include/utils.h"

typedef struct t_info_hash{
    int chave;
    void* info;
}t_info_hash;

t_info_hash* e_hash_criar(int chave, void* info){
    t_info_hash* novo = malloc(sizeof(t_info_hash));
    novo->chave = chave;
    novo->info = info;
    return novo;
}

void e_hash_destroy(t_info_hash* e){
    free(e->info); // potencial memory leak
    free(e);
}

void* e_hash_get(t_info_hash* e){
    assert(e!=NULL);    
    return e->info;
}

static int e_hash_comparar(void* c1, void* c2, Metrics* metrics){
    if(metrics) metrics->custo_agregado++;
    t_info_hash* e1 = c1;
    t_info_hash* e2 = c2;
    return e1->chave - e2->chave;
}

struct hash{
    int M;
    int N; //ocupacao
    double fc;
    t_lse* *vetor;
    Metrics* metrics;
};

static int hashing(t_hash *hash, int chave){
    return (chave%hash->M);
}

t_hash* hash_criar(Metrics* metrics){
    t_hash* novo = malloc(sizeof(t_hash));
    novo->M = 7;
    novo->N = 0;
    novo->fc = 0.85;
    novo->vetor = malloc(sizeof(t_lse*)*novo->M);
    for(int i=0;i<novo->M;i++){
        novo->vetor[i] = lse_criar(imprimir_hash_adapt,e_hash_comparar, metrics);
    }
    novo->metrics = metrics;
    return novo;
}

void rehashing(t_hash*hash){
    int novo_M = hash->M*2; // dobro do tamanho atual

    //salvar o dados antigos
    t_lse** vetor_antigo = hash->vetor;
    int M_antigo = hash->M;

    //novo vetor
    hash->vetor = malloc(sizeof(t_lse*)*novo_M);
    hash->M = novo_M;

    //inicializar as listas
    for(int i=0;i<novo_M;i++){
        hash->vetor[i] = lse_criar(imprimir_hash_adapt,e_hash_comparar, hash->metrics);
    }
    //reinserir os elementos
    for(int i=0;i<M_antigo;i++){
        t_lse* lista_atual = vetor_antigo[i];
        t_info_hash* elemento;
        while((elemento = lse_remover(lista_atual))!=NULL){
            //reinserir na nova hash
            int nova_k = hashing(hash, elemento->chave);
            t_lse* nova_lista = hash->vetor[nova_k];
            lse_inserir_final(nova_lista, elemento);
            hash->metrics->hash_movimentacoes++;
        }
        lse_destruir(lista_atual, NULL); //não destrói os elementos, apenas a lista
    }
    free(vetor_antigo);
    hash->metrics->hash_redimensionamentos++;
    c_calcular(hash);
}

//C = Σ(x_i²/N) - α
void c_calcular(t_hash* hash){
    double soma_quadrados = 0;
    for(int i=0;i<hash->M;i++){
        int tamanho_lista = lse_tamanho(hash->vetor[i]);
        //x_i²
        soma_quadrados += (tamanho_lista*tamanho_lista);
    }
    //(x_i²/N) - α
    double C = (soma_quadrados / hash->N) - hash->metrics->hash_fator_carga;
    hash->metrics->hash_clusterizacao = C;
}

void hash_inserir(t_hash* hash, int chave, void* carga ){
    if(hash->metrics->hash_fator_carga > 0.7){
        rehashing(hash);
    }

    t_info_hash* novo = e_hash_criar(chave, carga);

    int k = hashing(hash, chave);
    t_lse* l = hash->vetor[k];
    
    lse_inserir_final(l, novo);
    hash->metrics->numero_elementos++; 
    hash->N++;
    hash->metrics->hash_fator_carga = (double)hash->N / (double)hash->M;
}

void* hash_buscar(t_hash* hash, int chave){
    int k = hashing(hash, chave);

    t_lse* l = hash->vetor[k];

    t_info_hash* e = lse_buscar(l,&chave);
    return (e!=NULL?e->info:NULL);
}

void* hash_remover(t_hash* hash, int chave){
    int k = hashing(hash, chave);

    t_lse* l = hash->vetor[k];

    t_info_hash* e = lse_remover_conteudo(l, &chave);
    if(e!=NULL){
        void* info = e->info;
        free(e);
        hash->N--;

        return info;
    }
    return NULL;
}

void hash_destruir(t_hash* hash){
    if (hash == NULL) return;
    for(int i=0;i<hash->M;i++){
        t_lse* lista_atual = hash->vetor[i];
        lse_destruir(lista_atual, (t_lse_destruir)e_hash_destroy);
    }
    free(hash->vetor);
    free(hash);
}




//FUNÇÕES DE TESTE PARA DEPURAÇÃO E VALIDAÇÃO DA HASH
//TEST: Adaptador de Impressão para a Carga Útil (t_info_hash -> t_info_consolidada)
void imprimir_hash_adapt(void* carga_hash){
    t_info_hash* info_hash = (t_info_hash*)carga_hash;
    void* info_consolidada = e_hash_get(info_hash);
    imprimir_mme(info_consolidada);
}
//TEST: Função para imprimir todo o conteúdo da Hash de medições consolidadas
void imprimir_hash_consolidado(t_hash* hash){
    if (hash == NULL){
        printf("Hash é NULL em imprimir_hash_consolidada\n");
        return;
    }
    printf("Imprimindo conteúdo da Hash de medições consolidadas:\n");
    for(int i=0; i<hash->M; i++){
        t_lse* lista_atual = hash->vetor[i];
        
        lse_imprimir(lista_atual);
    }
}