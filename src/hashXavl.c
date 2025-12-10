#include "stdio.h"
#include "stdlib.h"
#include "./include/hash.h"
#include "./include/utils.h"

int main_teste_leitura(){
    /*Primeiro teste de leitura das arquivos.in*/

    FILE* medidas = fopen("../medidas_2_05.in", "r");
    
    if (medidas == NULL){
        perror("Erro ao abrir o arquivo de medidas");
        return 0;
    }
    t_registro_sensor* sensor;

    printf("Iniciando leitura dos sensores do arquivo...\n");

    int contador = 0;
    while((sensor = ler_sensor(medidas))!=NULL){
        contador++;

        //Validação simples da leitura
        printf(
            "Sensor %d: ID=%d, Tipo=%d, Valor=%.3f, Horario=%s, Alpha=%.2f\n",
            contador,
            sensor->id,
            sensor->tipo,
            sensor->valor,
            sensor->horario,
            sensor->alpha
        );
        free(sensor); //liberar memória se não for inserido na hash ou AVL    
    }
    fclose(medidas);
    printf("Leitura finalizada. Total de sensores lidos: %d\n", contador); 

    return 0;
}
int main_teste_extrair_hora(){
    /*Teste da função de extrair hora*/
    char* horario_teste[] = {
        "14:30:45",
        "09:15:00",
        "23:59:59",
        "00:00:00"
    };
    int num_testes = sizeof(horario_teste) / sizeof(horario_teste[0]);

    for(int i=0; i<num_testes; i++){
        char* hora_extraida = extrair_hora(horario_teste[i]);
        if(hora_extraida != NULL){
            printf("Horario original: %s -> Hora extraida: %s\n", horario_teste[i], hora_extraida);
            free(hora_extraida);
        }else{
            printf("Falha ao extrair hora do horario: %s\n", horario_teste[i]);
        }
    }
    printf("--------------------------------\n");
    return 0;
}
int main_teste_mme(){
    /*Teste de Calculo da Media Movel Exponencial*/

    //Ex:
    const double alpha = 0.40; //Fator de suavização

    double medicoes[] = {100.0, 120.0, 90.0, 150.0};

    //caso base: t=1
    double media_suavizada = 0.0;

    printf("Cálculo da Média Móvel Exponencial (MME) com alpha=%.2f\n", alpha);

    
    int t = 1;
    for(int i=0;i<sizeof(medicoes)/sizeof(medicoes[0]);i++){
        double medicao_atual = medicoes[i];//medição no tempo t
        double media_anterior = media_suavizada;
        media_suavizada = calcular_mme(media_anterior, medicao_atual, alpha);

        if(t==1){
            //Caso base: m_t = medicao_atual
            printf("t=%d: Medição=%.2f -> MME=%.2f (caso base)\n", t, medicao_atual, media_suavizada);
        } else{
            double esperado = alpha * medicao_atual + (1.0 - alpha) * media_anterior; 
            printf("t=%d Med. Atual=%.2f, MME_ant=%.4f. MME_nova = %.4f (Esperado: %.4f)\n", t, medicao_atual, media_anterior, media_suavizada, esperado);
        }
        t++;
    }
    printf("-----------------------------------------\n");
}


//Vetor de nomes das 4 instâncias de teste
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
        //2. Leitura do arquivo e inserção na Hash/AVL
        //3. Consolidação das medições (Cálculo da MME)
        //4. Coleta e exibição das métricas
        //5. Liberação de memória e finalização do experimento
        //6. Geração do Arquivo .cons

        //restante do código do experimento aqui
    }
    return 0;
}