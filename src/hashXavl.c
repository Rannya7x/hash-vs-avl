#include "stdio.h"
#include "stdlib.h"
#include "./include/hash.h"
#include "./include/avl.h"
#include "./include/utils.h"
#include "./include/metrics.h"



const char* instancias[] = {
    "../medidas_2_05.in",
    "../medidas_2_10.in",
    "../medidas_2_15.in",
    "../medidas_2_20.in"
};

//A main (loop for) irá iterar sobre as 4 instâncias, executando o experimento: inicialização, inserção, consolidação e coleta de métricas para cada arquivo de medidas.
//A iteração para cada linha do arquivo de medidas será feito duas vezes: uma para a hash e outra para a avl (2 loops while).
int main(){
    //0. Preparar o csv das métricas
    FILE* csv = fopen("metricas.csv", "w");
    if(csv) {
        // Escrever cabeçalho
        fprintf(csv, "tamanho_n,hash_comparacoes,hash_fator_carga,hash_clusterizacao,hash_redimensionamentos,hash_movimentacoes,avl_comparacoes,avl_altura,avl_rotacoes\n");
    }

    for(int i=0;i<4;i++){
        //1. Inicialização da Hash, Avl e Metricas e outras variaveis
        Metrics* hash_metrics = init_metrics();
        Metrics* avl_metrics = init_metrics();

        t_hash* hash = hash_criar(hash_metrics);
        t_avl* avl = avl_criar(imprimir_mme, comparar_mme, avl_metrics);

        t_registro_sensor* sensor_hash;
        t_registro_sensor* sensor_avl;

        t_info_consolidada* info_hash = NULL;
        t_info_consolidada* info_avl = NULL;
        
        char nome_consolidado[256]; //para o aruivo.cons
        const char* instancia_atual = instancias[i];
        long long n; //tamanho da intancia

        //2. Leitura do arquivo e inserção na Hash
         
        FILE* medidas = fopen(instancia_atual, "r");
        if(medidas == NULL){
            perror("Error ao abrir o arquivo de medidas");
            exit(EXIT_FAILURE);
        }
        
        printf("----- Iniciando experimento -----\n");
        printf("Processando instância: %s\n", instancia_atual);

        //Loop de inserção dos dados na Hash
        printf("Iniciando inserção na Hash...\n");
        while ((sensor_hash = ler_sensor(medidas))!=NULL){//Enquanto houver registros a serem lidos
            n++;//conta linhas (tamanho da instancia)
            //Geração da chave hash
            int hora = extrair_hora(sensor_hash->horario);
            int chave = gerar_chave_int(sensor_hash->id, hora);      
            /*Busca do MME
            Se a busca retornar NUll, significa que é a primeira medição para aquele sensor_hash na hora
                - add a medição atual como MME na hash
            Se a busca retornar um valor válido, usamos a MME retornada para calcular a nova MME
                - atualizar a hash com a nova MME*/
            info_hash = (t_info_consolidada*)hash_buscar(hash, chave);
            if(info_hash == NULL){
                info_hash = criar_info_consolidada(chave, sensor_hash->valor, sensor_hash->id, hora, sensor_hash->alpha, sensor_hash->tipo);
                hash_inserir(hash, chave, info_hash);
            }else{
                double nova_mme = calcular_mme(info_hash->mme_suavizada, sensor_hash->valor, info_hash->alpha);
                info_hash->mme_suavizada = nova_mme;
            }
            free(sensor_hash);
        }
        //3. Consolidação e calculo MME
        //printf("\n--- CONTEÚDO FINAL CONSOLIDADO DA HASH ---\n");
        //imprimir_hash_consolidado(hash); descomentar para ver a consolidação no terminal
        
        //4. Coleta de metricas
        hash_metrics->n = n;
        c_calcular(hash);
        printf("\nMétricas da Hash:\n");
        printf("Tamanho: %lld\n", hash_metrics->n);
        printf("Custo Agregado: %lld\n", hash_metrics->custo_agregado);
        printf("Número de Elementos: %lld\n", hash_metrics->numero_elementos);
        printf("Número de Movimentações: %lld\n", hash_metrics->hash_movimentacoes);
        printf("Fator de Carga: %.2lf\n", hash_metrics->hash_fator_carga);
        printf("Clusterização: %.2lf\n", hash_metrics->hash_clusterizacao);
        printf("Número de Redimensionamentos: %lld\n", hash_metrics->hash_redimensionamentos);
        printf("-----------------------------------------\n");
        
        //Loop da avl
        //2. Reler o arquivo do inicio
        rewind(medidas);
        printf("Iniciando inserção na AVL...\n");
        while((sensor_avl = ler_sensor(medidas))!=NULL){
            int hora = extrair_hora(sensor_avl->horario);
            int chave = gerar_chave_int(sensor_avl->id, hora);

            //variavel temporaria para busca na avl
            t_info_consolidada chave_temp;
            chave_temp.chave_consolidacao = chave;

            info_avl = (t_info_consolidada*)avl_buscar(avl, &chave_temp);
            if(info_avl == NULL){
                info_avl = criar_info_consolidada(chave, sensor_avl->valor, sensor_avl->id, hora, sensor_avl->alpha, sensor_avl->tipo);
                avl_inserir(avl, info_avl);
                avl_metrics->numero_elementos++;
            }else{
                double nova_mme = calcular_mme(info_avl->mme_suavizada, sensor_avl->valor, info_avl->alpha);
                info_avl->mme_suavizada = nova_mme;
            }
            free(sensor_avl);

        }
        //3. Consolidação e calculo MME
        //printf("\n--- CONTEÚDO FINAL CONSOLIDADO DA AVL ---\n");
        //avl_em_ordem(avl); --> descomentar para ver a consolidação no terminal
        
        //4. Coleta de metricas
        avl_metrics->n = n;
        avl_metrics->avl_altura = avl_altura(avl);
        printf("\nMétricas da Avl:\n");
        printf("Tamanho: %lld\n", avl_metrics->n);
        printf("Custo Agregado: %lld\n", avl_metrics->custo_agregado);
        printf("Número de Elementos: %lld\n", avl_metrics->numero_elementos);
        printf("Altura da arvore: %d\n", avl_metrics->avl_altura);
        printf("Rotações: %lld\n ", avl_metrics->avl_rotacoes);

        printf("-----------------------------------------\n");
        printf("Experimento concluído para a instância: %s\n", instancia_atual);
        printf("----- Fim do experimento -----\n\n");

        //. Geração do Arquivo .cons
        gerar_nome_cons(instancia_atual, nome_consolidado, sizeof(nome_consolidado));
        FILE* arquivo_cons = fopen(nome_consolidado, "w");

        if(arquivo_cons){
            avl_percorrer_em_ordem_com_ctx(avl, escrever_no_arquivo, arquivo_cons);

            fclose(arquivo_cons);
            printf("Arquivo %s gerado com sucesso\n", nome_consolidado);
        }
        fclose(medidas);

        //6. Geração de csv das metricas
        escrever_csv(hash_metrics, avl_metrics, csv);

        //7. Liberação de memória
        hash_destruir(hash);
        avl_destruir(avl, liberar_info_consolidada);
        free_metrics(avl_metrics);
        free_metrics(hash_metrics); 
        
    }
    
    fclose(csv);
    return 0;
}