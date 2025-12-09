/* Utilitários para manipulação de registros de sensores

*/
#include "./include/utils.h"
#include "stdio.h"
#include "stdlib.h"

// Função para ler um registro de sensor de um arquivo
t_registro_sensor* ler_sensor(FILE* medidas){
    t_registro_sensor* novo_sensor = (t_registro_sensor*)malloc(sizeof(t_registro_sensor));

    int sucesso = fscanf(
        medidas,
        "%d\t%d\t%lf\t%8s\t%lf\n", // Nota: O gerador.py usa '\t' como separador
        &novo_sensor->id,
        &novo_sensor->tipo,
        &novo_sensor->valor,
        novo_sensor->horario,
        &novo_sensor->alpha
    );

    if (sucesso == 5){
        return novo_sensor;//5 campos lidos com sucesso
    }else{
        if(sucesso != EOF){
            printf("Erro na leitura do sensor. Apenas %d de 5 campos foram lidos\n"); //debug
        }
        free(novo_sensor);
        return NULL;
    }
    
}