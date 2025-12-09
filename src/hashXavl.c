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