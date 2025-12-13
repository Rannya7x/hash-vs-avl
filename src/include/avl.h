#include "./metrics.h"

typedef struct avl t_avl;
typedef void (*t_avl_imprimir)(void*);
typedef int (*t_avl_comparar)(void*, void*, Metrics*);
typedef void (*t_avl_destruir)(void*);
typedef void (*t_avl_visitar_com_ctx)(void* info, void* contexto);
t_avl* avl_criar(t_avl_imprimir imprimir, t_avl_comparar comparar, Metrics* metrics);
void avl_inserir(t_avl* avl, void* info);
void* avl_buscar(t_avl* avl, void* chave);
void* avl_remover(t_avl* avl, void* chave);
void avl_destruir(t_avl* avl, t_avl_destruir destruir);
void avl_em_ordem(t_avl* avl);
int avl_altura(t_avl* avl);
void avl_percorrer_em_ordem_com_ctx(t_avl* avl, t_avl_visitar_com_ctx visitar, void* contexto);


