#include "stdio.h"
#include "stdlib.h"
#include "./include/hash.h"
#include "./include/avl.h"
#include "./include/utils.h"
#include "./include/metrics.h"

const char* instancias[] = {
    "medidas_2_05.in",
    "medidas_2_10.in",
    "medidas_2_15.in",
    "medidas_2_20.in"
};
//A main irá iterar sobre as 4 instâncias, executando o experimento: inicialização, inserção, consolidação e coleta de métricas para cada arquivo de medidas.
int main(){
    for(int i=0;i<4;i++){
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
        //Loop de inserção dos dados na Hash
        t_registro_sensor* sensor_hash;
        t_info_consolidada* info = NULL;
        printf("Iniciando inserção na Hash...\n");
        while ((sensor_hash = ler_sensor(medidas))!=NULL){//Enquanto houver registros a serem lidos
            //Geração da chave hash
            int* hora_ptr = extrair_hora(sensor_hash->horario);
            int chave = gerar_chave_int(sensor_hash->id, hora_ptr);
            int hora = *hora_ptr; 
            free(hora_ptr);
            /*Busca do MME
            Se a busca retornar NUll, significa que é a primeira medição para aquele sensor_hash na hora
                - add a medição atual como MME na hash
            Se a busca retornar um valor válido, usamos a MME retornada para calcular a nova MME
                - atualizar a hash com a nova MME*/
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

        //Loop da avl
        rewind(medidas);
        printf("Ponteiro do arquivo reiniciado para inserção na AVL.\n");
        printf("Iniciando inserção na AVL...\n");

       /* while(true){

        }*/
        
        fclose(medidas);
        printf("Medidas fechadas.\n");
        //3. Consolidação das medições (Cálculo da MME)
        //4. Coleta e exibição das métricas
        //5. Liberação de memória e finalização do experimento
        //6. Geração do Arquivo .cons

        //restante do código do experimento aqui
    }
    return 0;
}