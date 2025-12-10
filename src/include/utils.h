#include "stdio.h"

typedef struct {
    int id;
    int tipo;
    double valor;
    char horario[9];
    double alpha;
} t_registro_sensor;
t_registro_sensor* ler_sensor(FILE* medidas);

// Funções utilitárias
char* extrair_hora(char* horario);
double calcular_mme(double media_anterior, double medicao_atual, double alpha);
//fundir o id e a hora em um único inteiro
int gerar_chave_int(int id_sensor, char* hora);

//Estrutura unificada da chave de consolidação e estado MME
typedef struct{
    int chave_consolidacao; //chave gerada a partir do id do sensor e da hora
    double mme_suavizada; //estado atual da MME
    int id_sensor;
    int hora; //hora extraída do horário completo
}t_info_consolidada;

//Funções para AVL: impressão e comparação de t_info_consolidada
int comparar_mme(void* chave1, void* chave2);
void imprimir_mme(void* info);