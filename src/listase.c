#include "stdio.h"
#include "stdlib.h"
#include "./include/listase.h"

typedef struct elem_lse t_lse_elemento;
struct lse{
    t_lse_elemento* primeiro;
    t_lse_elemento* ultimo;
    int tamanho;
    t_lse_imprimir impressora;
    t_lse_comparar comparar;
};

t_lse* lse_criar(t_lse_imprimir imp, t_lse_comparar comparar){
    t_lse* nova = malloc(sizeof(t_lse));
    nova->primeiro = NULL;
    nova->ultimo = NULL;
    nova->tamanho = 0;
    nova->impressora = imp;
    nova->comparar = comparar;
    return nova;
}

struct elem_lse
{
    void* carga_util;
    t_lse_elemento* prox;
};

t_lse_elemento* criar_elem_lse(void* carga){
    t_lse_elemento* novo = malloc(sizeof(t_lse_elemento));
    novo->carga_util = carga;
    novo->prox = NULL;

    return novo;
}
// Insere no inicio da lista
void lse_inserir(t_lse* lse, void* carga){
    t_lse_elemento * novo = criar_elem_lse(carga);
    
    novo->prox = lse->primeiro;
    lse->primeiro = novo;
    lse->tamanho++;
    if(lse->ultimo == NULL){
        lse->ultimo = novo;
    }
}

void lse_inserir_final(t_lse* lse, void* carga){
    t_lse_elemento* novo = criar_elem_lse(carga);
    if(lse->primeiro == NULL){ // lista vazia
        lse->primeiro = novo;
        lse->ultimo = novo;
    }else{
        lse->ultimo->prox = novo;
        lse->ultimo = novo;
    }
    lse->tamanho++;
}
// Remove do inicio da lista
void* lse_remover(t_lse* lse){
    void* carga=NULL;
    if (lse->ultimo == NULL){
        return carga;
    }else if (lse->ultimo == lse->primeiro){
        t_lse_elemento* deletado = lse->primeiro;
        lse->primeiro = NULL;
        lse->ultimo = NULL;
        carga = deletado->carga_util;
        free(deletado);
    }else{
        t_lse_elemento* deletado = lse->primeiro;
        lse->primeiro = lse->primeiro->prox;
        carga = deletado->carga_util;
        free(deletado);
    }
    lse->tamanho--;
    return carga;
}

void lse_imprimir(t_lse* lse){
    t_lse_elemento *cam = lse->primeiro;
    while(cam!=NULL){
        lse->impressora(cam->carga_util);
        cam = cam->prox;
    }
}

void* lse_buscar(t_lse* lse, void* chave){
    t_lse_elemento* cam = lse->primeiro;
    while((cam!=NULL) && (lse->comparar(cam->carga_util, chave)!=0)){
        cam = cam->prox;
    }
    return (cam==NULL?NULL:cam->carga_util);
}

void* lse_acessar(t_lse* lse, int pos){
    void* carga=NULL;
    int i=1;
    t_lse_elemento* cam = lse->primeiro;
    while((cam!=NULL) && (i<pos))
        cam = cam->prox;
    return (cam==NULL?NULL:cam->carga_util);
}
//inserir conteudo ordenado
void lse_inserir_conteudo(t_lse* lse, void* carga){
    
}

void* lse_remover_conteudo(t_lse* lse, void* carga){

}

void* lse_remover_final(t_lse* lse){

}
