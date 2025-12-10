/* Utilitários para manipulação de registros de sensores

*/
#include "./include/utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

// Função para extrair a hora de uma string no formato "HH:MM:SS"
char* extrair_hora(char* horario){
    char* hora = (char*)malloc(sizeof(char)*3);//Aloca espaço para "HH" + '\0'

    if(hora==NULL){
        printf("Erro de alocação de memória em extrair_hora\n");
        return NULL;
    }
    strncpy(hora, horario, 2);
    hora[2] = '\0'; // Adiciona o terminador nulo
    return hora;
}

// Função para calcular a Média Móvel Exponencial (MME)
double calcular_mme(double media_anterior, double medicao_atual, double alpha){
    // Se media_anterior for 0 (t=1), a nova média é simplesmente a medição atual
   
    if(media_anterior == 0.0){
        return medicao_atual;
    }

    //O cálculo da MME para t>1 é dado por: MME_t = alpha * medicao_atual + (1 - alpha) * media_anterior
    return alpha * medicao_atual + (1.0 - alpha) * media_anterior;
}

// Função para gerar uma chave hash a partir do id do sensor e da hora
int gerar_chave_hash(int id_sensor, char* hora){
    int hora_int = atoi(hora); // Converte a hora "HH" para um inteiro
    return (id_sensor*100) + hora_int; // Gera a chave combinando id e hora
}