/* Estrutura para armazenar metricas da hash e da AVL
    hash:
    - Movimentações
    - Fator de Carga (α)
    - Clusterização (C)
    - Numero de redimensionamentos
    AVL:
    - Rotações
    - Altura da árvore
    Metrica compartilhada:
    - numero de elementos na estrutura
    - Custo agregado das operações de busca, inserção e remoção (comparações)
*/
typedef struct {
    // Métrica geral
    long long custo_agregado;
    long long numero_elementos;

    // Métricas da Hash
    long long hash_movimentacoes;
    double hash_fator_carga;
    double hash_clusterizacao;
    long long hash_redimensionamentos;

    // Métricas da AVL
    long long avl_rotacoes;
    int avl_altura;

    
}Metrics;

Metrics* init_metrics();
void free_metrics(Metrics* metrics);