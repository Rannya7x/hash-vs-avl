typedef struct avl t_avl;
typedef void (*t_avl_imprimir)(void*);
typedef int (*t_avl_comparar)(void*, void*);
t_avl* avl_criar(t_avl_imprimir imprimir, t_avl_comparar comparar);
void avl_inserir(t_avl* avl, void* info);
void* avl_buscar(t_avl* avl, void* chave);
void* avl_remover(t_avl* avl, void* chave);

