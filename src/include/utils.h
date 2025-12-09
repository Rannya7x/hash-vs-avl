#include "stdio.h"

typedef struct {
    int id;
    int tipo;
    double valor;
    char horario[9];
    double alpha;
} t_registro_sensor;

t_registro_sensor* ler_sensor(FILE* medidas);
