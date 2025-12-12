/* Utilitários para manipulação de registros de sensores*/
#include "./include/utils.h"
#include "./include/avl.h"
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
int extrair_hora(char* horario){
    char hora_str[3];

    strncpy(hora_str, horario, 2);
    hora_str[2] = '\0'; // Adiciona o terminador nulo
    int hora_int = atoi(hora_str);//
    
    return hora_int;
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
int gerar_chave_int(int id_sensor, int hora){
    return (id_sensor*100) + hora;
}

// Criação e liberação de t_info_consolidada
t_info_consolidada* criar_info_consolidada(int chave, double mme, int id_sensor, int hora, double alpha){
    t_info_consolidada* nova_info = (t_info_consolidada*)malloc(sizeof(t_info_consolidada));
    if(nova_info == NULL){
        printf("Erro de alocação de memória em criar_info_consolidada\n");
        return NULL;
    }else{
        nova_info->chave_consolidacao = chave;
        nova_info->mme_suavizada = mme;
        nova_info->id_sensor = id_sensor;
        nova_info->hora = hora;
        nova_info->alpha = alpha;
        return nova_info;
    }
}

void liberar_info_consolidada(void* info){
    t_info_consolidada* nova_info = (t_info_consolidada*)info; 
    if(info==NULL){
        perror("Info já é NULL ao liberar_info_consolidada\n");
        return;
    }
    free(info);
}

// Função para comparar duas estruturas t_info_consolidada com base na MME suavizada
int comparar_mme(void* chave1, void* chave2, Metrics* metrics){
    t_info_consolidada* info1 = (t_info_consolidada*)chave1;
    t_info_consolidada* info2 = (t_info_consolidada*)chave2;

    
    if(metrics) metrics->custo_agregado++; 

    if(info1->chave_consolidacao > info2->chave_consolidacao){
        return 1;
    }else if(info1->chave_consolidacao < info2->chave_consolidacao){
        return -1;
    }else{
        return 0;
    }
}

void imprimir_mme(void* info){
    t_info_consolidada* estado = (t_info_consolidada*)info;
    printf("Sensor ID: %d | Hora: %d | Chave: %d | MME Suavizada: %.3f\n",
        estado->id_sensor,
        estado->hora,
        estado->chave_consolidacao,
        estado->mme_suavizada
    );
}