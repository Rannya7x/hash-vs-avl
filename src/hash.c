#include "./include/hash.h"
#include "stdio.h"
#include "stdlib.h"
#include "./include/listase.h"
#include "assert.h"
#include "./include/metrics.h"

typedef struct {
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

static int e_hash_comparar(void* c1, void* c2){
    t_info_hash* e1 = c1;
    t_info_hash* e2 = c2;
    return e1->chave - e2->chave;
}

struct hash{
    int M;
    int N; //ocupacao
    double fc;
    t_lse* *vetor;
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
    for(int i=0;novo->M;i++){
        novo->vetor[i] = lse_criar(NULL,e_hash_comparar);
    }
    return novo;
}

void hash_inserir(t_hash* hash, int chave, void* carga ){
    t_info_hash* novo = e_hash_criar(chave, carga);

    int k = hashing(hash, chave);
    
    t_lse* l = hash->vetor[k];
    
    lse_inserir_final(l, novo); 
    hash->N++;

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