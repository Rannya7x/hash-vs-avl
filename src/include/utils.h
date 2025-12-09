#include "stdio.h"

typedef struct {
    int id;
    int tipo;
    double valor;
    char horario[9];
    double alpha;
} t_registro_sensor;

t_registro_sensor* ler_sensor(FILE* medidas);
char* extrair_hora(char* horario);
double calcular_mme(double media_anterior, double medicao_atual, double alpha);