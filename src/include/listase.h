typedef void(*t_lse_imprimir)(void*);
typedef int(*t_lse_comparar)(void* arg_1, void* arg_2);

typedef struct lse t_lse;
t_lse* lse_criar(t_lse_imprimir impressora, t_lse_comparar comparar);
void lse_inserir(t_lse* lse, void* carga);
void lse_inserir_final(t_lse* lse, void* carga);
void* lse_remover(t_lse* lse);
void* lse_remover_final(t_lse* lse);
void* lse_acessar(t_lse* lse, int pos);
void lse_imprimir(t_lse* lse);
void* lse_buscar(t_lse* lse, void* buscado);
void lse_inserir_conteudo(t_lse* lse, void* carga);
void* lse_remover_conteudo(t_lse* lse, void* carga);
