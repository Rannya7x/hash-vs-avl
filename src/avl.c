#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "./include/avl.h"
#include "./include/metrics.h"

typedef struct no
{
    struct no *actral;
    struct no *sae;
    struct no *sad;
    int fb;
    void* info;
}t_no;

static t_no* no_criar(void* info, t_no* actral){
    t_no* novo = malloc(sizeof(t_no));
    novo->info = info;
    novo->actral = actral;
    novo->sae = NULL;
    novo->sad = NULL;
    novo->fb = 0;
    return novo;
}

typedef void (*t_avl_imprimir)(void*);
typedef int (*t_avl_comparar)(void*, void*);
typedef struct avl
{
    t_no* raiz;
    t_avl_imprimir imprimir;
    t_avl_comparar comparar;
}t_avl;

t_avl* avl_criar(t_avl_imprimir imprimir, t_avl_comparar comparar, Metrics* metrics){
    t_avl *nova = malloc(sizeof(t_avl));
    nova->raiz = NULL;
    nova->imprimir = imprimir;
    nova->comparar = comparar;

    return nova;
}

#define MAX(a, b) (a>b?a:b)

static int __altura_avl(t_no *no){
    if (no == NULL){
        return -1;
    }
    return MAX(__altura_avl(no->sae), __altura_avl(no->sad)) + 1;
}
static t_no* __rsd(t_no* A, t_no* B){
    A->sae = B->sad;
    B->sad = A;

    B->actral = A->actral;
    A->actral = B;
    B->fb = __altura_avl(B->sad) - __altura_avl(B->sae);
    A->fb = __altura_avl(A->sad) - __altura_avl(A->sae);

    return B;
}

static t_no* __rse(t_no* A, t_no* B){
    A->sad = B->sae;
    B->sae = A;

    B->actral = A->actral;
    A->actral = B;
    
    B->fb = __altura_avl(B->sad) - __altura_avl(B->sae);
    A->fb = __altura_avl(A->sad) - __altura_avl(A->sae);

    return B;
}

static t_no* __avl_inserir(t_no* raiz, void* info, t_no* actral, t_avl_comparar comparar){
    if(raiz == NULL){
        t_no* novo = no_criar(info,actral);
        return novo;
    }else{
        int rest = comparar(raiz->info, info);
        if(rest > 0){ // inserir sae
            raiz->sae = __avl_inserir(raiz->sae, info, raiz, comparar);
            t_no* sae = raiz->sae;
            raiz->fb = __altura_avl(raiz->sad) - __altura_avl(raiz->sae);
            if (raiz->fb == -2){
                if (sae->fb == -1){ // rotação simples
                    raiz = __rsd(raiz, sae);
                }else{ // rotação duplas
                    t_no* aux = sae->sad;
                    __rse(sae, aux);
                    raiz = __rsd(raiz, aux);
                }
            }
        }else{ // inserir sad
            raiz->sad = __avl_inserir(raiz->sad, info, raiz, comparar);
            t_no* sad = raiz->sad;
            raiz->fb = __altura_avl(raiz->sad) - __altura_avl(raiz->sae);
            if(raiz->fb == 2){
                if (sad->fb == 1){
                    raiz = __rse(raiz, sad);
                }else{
                    t_no* aux = sad->sae;
                    __rsd(sad, aux);
                    raiz = __rse(raiz, aux);
                }
            }
        }
        return raiz;
    }
}

void avl_inserir(t_avl* avl, void* info){
    avl->raiz = __avl_inserir(avl->raiz, info, NULL, avl->comparar);
}


static t_no* __avl_buscar(t_no* raiz, t_avl_comparar comparar, void* chave){
    if (raiz == NULL){
        return NULL;
    }
    int res = comparar(raiz->info, chave); 
    if( res == 0){
        return raiz;
    } else if (res > 0){
        return __avl_buscar(raiz->sae, comparar, chave);
    } else {
        return __avl_buscar(raiz->sad, comparar, chave);
    }
}

void* avl_buscar(t_avl* avl, void* chave){
    
    t_no* no = __avl_buscar(avl->raiz, avl->comparar, chave);
    if (no!=NULL){
        return no->info;
    }
    return NULL;
}

static void __trocar(t_no* i1, t_no* i2){
    void* aux = i1->info;
    i1->info = i2->info;
    i2->info = aux;
}

static t_no* __avl_remover(t_no* raiz, t_avl_comparar comparar, void* chave, void* *placeholder){
    if (raiz == NULL){
        *placeholder=NULL;
        return NULL;
    }else{
        int res = comparar(raiz->info, chave);
        if(res > 0){
            raiz->sae = __avl_remover(raiz->sae, comparar, chave, placeholder);
            t_no* sad = raiz->sad;
            raiz->fb = __altura_avl(raiz->sad) - __altura_avl(raiz->sae);
            if (raiz->fb == 2){
                if (sad->fb >= 0){ // rotação simples
                    raiz = __rse(raiz, sad);
                }else{ // rotação duplas
                    t_no* aux = sad->sae;
                    __rsd(sad, aux);
                    raiz = __rse(raiz, aux);
                }
            }    
            return raiz;
        } else if (res < 0){
            raiz->sad = __avl_remover(raiz->sad, comparar, chave, placeholder);
            t_no* sae = raiz->sae;
            raiz->fb = __altura_avl(raiz->sad) - __altura_avl(raiz->sae);
            if(raiz->fb == -2){
                if (sae->fb <= 0){
                    raiz = __rsd(raiz, sae);
                }else{
                    t_no* aux = sae->sad;
                    __rse(sae, aux);
                    raiz = __rsd(raiz, aux);
                }
            }    
            return raiz;

        } else {
            if ( (raiz->sae == NULL) && (raiz->sad == NULL) ){
               *placeholder = (raiz->info);

               free(raiz);
               return NULL;     
            }else if ( (raiz->sae == NULL) && (raiz->sad != NULL) ){
                t_no* sad = raiz->sad;
                sad->actral = raiz->actral;
                
                *placeholder = (raiz->info);
                free(raiz);

                return sad;

            } else if ( (raiz->sae != NULL) && (raiz->sad == NULL) ){
                t_no* sae = raiz->sae;
                sae->actral = raiz->actral;

                *placeholder = (raiz->info);
                free(raiz);

                return sae;
            } else {
                t_no* no = raiz->sad;
                while (no->sae!=NULL){
                    no = no->sae;
                } 
                __trocar(raiz, no);
                raiz->sad = __avl_remover(raiz->sad,comparar,chave, placeholder);
                return raiz;
            }
        
        }

    }

}

void* avl_remover(t_avl* avl, void* chave){
    void* info=NULL;
    avl->raiz = __avl_remover(avl->raiz, avl->comparar, chave, &info);
    

    return info;
}

