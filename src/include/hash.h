#include "./metrics.h"
typedef struct hash t_hash;
typedef struct t_info_hash t_info_hash;
void imprimir_hash_consolidado(t_hash* hash);
void imprimir_hash_adapt(void* carga_hash);

t_hash* hash_criar(Metrics* metrics);
void hash_inserir(t_hash* hash, int chave, void* carga );
void* hash_buscar(t_hash* hash, int chave);
void* hash_remover(t_hash* hash, int chave);
void e_hash_destroy(t_info_hash* e);
void hash_destruir(t_hash* hash);
void c_calcular(t_hash* hash);
