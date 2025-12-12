#include "stdio.h"
#include "stdlib.h"
#include "./include/hash.h"
#include "./include/avl.h"
#include "./include/utils.h"
#include "./include/metrics.h"
/*
const char* instancias[] = {
    "medidas_2_05.in",
    "medidas_2_10.in",
    "medidas_2_15.in",
    "medidas_2_20.in"
};*/

const char* instancias[] = {
    "3testehash.in"
};
//A main (loop for) irá iterar sobre as 4 instâncias, executando o experimento: inicialização, inserção, consolidação e coleta de métricas para cada arquivo de medidas.
//A iteração para cada linha do arquivo de medidas será feito duas vezes: uma para a hash e outra para a avl (2 loops while).
int main(){
    for(int i=0;i<1;i++){
        printf("----- Iniciando experimento -----\n");

        const char* instancia_atual = instancias[i];
        printf("Processando instância: %s\n", instancia_atual);

        //1. Inicialização da Hash, Avl e Metricas
        Metrics* hash_metrics = init_metrics();
        Metrics* avl_metrics = init_metrics();

        t_hash* hash = hash_criar(hash_metrics);
        t_avl* avl = avl_criar(imprimir_mme, comparar_mme, avl_metrics); 

        //2. Leitura do arquivo e inserção na Hash
         
        FILE* medidas = fopen(instancia_atual, "r");
        if(medidas == NULL){
            perror("Error ao abrir o arquivo de medidas");
            exit(EXIT_FAILURE);
        }
        /*
        //Loop de inserção dos dados na Hash
        long long n_bruto_hash = 0; //conta linha para definir tamanho da instância (n)
        t_registro_sensor* sensor_hash;
        t_info_consolidada* info = NULL;
        printf("Iniciando inserção na Hash...\n");
        while ((sensor_hash = ler_sensor(medidas))!=NULL){//Enquanto houver registros a serem lidos
            n_bruto_hash++;
            //Geração da chave hash
            int hora = extrair_hora(sensor_hash->horario);
            
            int chave = gerar_chave_int(sensor_hash->id, hora);
            
            /*Busca do MME
            Se a busca retornar NUll, significa que é a primeira medição para aquele sensor_hash na hora
                - add a medição atual como MME na hash
            Se a busca retornar um valor válido, usamos a MME retornada para calcular a nova MME
                - atualizar a hash com a nova MME
            info = (t_info_consolidada*)hash_buscar(hash, chave);//a hash geralmente é mais rapida
            
            if(info == NULL){
                info = criar_info_consolidada(chave, sensor_hash->valor, sensor_hash->id, hora, sensor_hash->alpha);
                hash_inserir(hash, chave, info);
            }else{
                double nova_mme = calcular_mme(info->mme_suavizada, sensor_hash->valor, info->alpha);
                info->mme_suavizada = nova_mme;
            }
            free(sensor_hash);
            printf("Inserção na Hash concluída para chave %d.\n", chave);
        }
        //3. Consolidação e calculo MME
        printf("\n--- CONTEÚDO FINAL CONSOLIDADO DA HASH ---\n");
        imprimir_hash_consolidado(hash);
        /*
        //4. Coleta de metricas
        c_calcular(hash);
        printf("\nMétricas da Hash:\n");
        printf("Custo Agregado: %lld\n", hash_metrics->custo_agregado);
        printf("Número de Elementos: %lld\n", hash_metrics->numero_elementos);
        printf("Número de Movimentações: %lld\n", hash_metrics->hash_movimentacoes);
        printf("Fator de Carga: %.2lf\n", hash_metrics->hash_fator_carga);
        printf("Clusterização: %.2lf\n", hash_metrics->hash_clusterizacao);
        printf("Número de Redimensionamentos: %lld\n", hash_metrics->hash_redimensionamentos);

        //5. Liberação de memória e finalização do experimento
        */
        hash_destruir(hash);
        free_metrics(hash_metrics); 
        printf("-----------------------------------------\n");
        
        //Loop da avl
        //rewind(medidas);
        printf("Ponteiro do arquivo reiniciado para inserção na AVL.\n");
        printf("Iniciando inserção na AVL...\n");

        long long n_bruto_avl = 0;
        t_registro_sensor* sensor_avl;
        t_info_consolidada* info_avl = NULL;
        while((sensor_avl = ler_sensor(medidas))!=NULL){
            n_bruto_avl++; //contador de leituras brutas

            int hora = extrair_hora(sensor_avl->horario);
            int chave = gerar_chave_int(sensor_avl->id, hora);

            //variavel temporaria para busca na avl
            t_info_consolidada chave_temp;
            chave_temp.chave_consolidacao = chave;

            info_avl = (t_info_consolidada*)avl_buscar(avl, &chave_temp);
            if(info_avl == NULL){
                info_avl = criar_info_consolidada(chave, sensor_avl->valor, sensor_avl->id, hora, sensor_avl->alpha);
                avl_inserir(avl, info_avl);
                avl_metrics->numero_elementos++;
            }else{
                double nova_mme = calcular_mme(info_avl->mme_suavizada, sensor_avl->valor, info_avl->alpha);
                info_avl->mme_suavizada = nova_mme;
            }
            free(sensor_avl);

        }
        //3. Consolidação e calculo MME
        printf("\n--- CONTEÚDO FINAL CONSOLIDADO DA AVL ---\n");
        avl_em_ordem(avl);
        
        //4. Coleta de metricas
        avl_metrics->avl_altura = avl_altura(avl);
        printf("\nMétricas da Avl:\n");
        printf("Custo Agregado: %lld\n", avl_metrics->custo_agregado);
        printf("Número de Elementos: %lld\n", avl_metrics->numero_elementos);
        printf("Altura da arvore: %d\n", avl_metrics->avl_altura);
        printf("Rotações: %lld\n ", avl_metrics->avl_rotacoes);

        //5. Liberação de memória e finalização do experimento
        fclose(medidas);
        printf("Medidas fechadas.\n");
        avl_destruir(avl, liberar_info_consolidada);
        free_metrics(avl_metrics);
        printf("-----------------------------------------\n");
        printf("Experimento concluído para a instância: %s\n", instancia_atual);
        printf("----- Fim do experimento -----\n\n");

        //6. Coleta das métricas
        //7. Geração de gráficos
        //. Geração do Arquivo .cons

        //restante do código do experimento aqui
    }
    
    return 0;
}