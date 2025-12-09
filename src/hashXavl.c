#include "stdio.h"
#include "stdlib.h"
#include "./include/hash.h"
#include "./include/utils.h"

int main(){
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