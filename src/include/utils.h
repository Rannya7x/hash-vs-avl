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

//Estrutura da chave de consolidação
typedef struct {
    int id_sensor;
    char hora[3]; // "HH" + '\0'
} t_chave_mme;

//Já que a hash trabalha com inteiros, vamos fundir o id e a hora em um único inteiro
int gerar_chave_hash(int id_sensor, char* hora);

typedef struct{
    double media_suavizada;
}t_estado_mme;