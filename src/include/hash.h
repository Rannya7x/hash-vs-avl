typedef struct hash t_hash;

t_hash* hash_criar(Metrics* metrics);
void hash_inserir(t_hash* hash, int chave, void* carga );
void* hash_buscar(t_hash* hash, int chave);
void* hash_remover(t_hash* hash, int chave);