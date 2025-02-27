#ifndef user_interface_h
#define user_interface_h
#include "display/ssd1306.h"
#include <stdio.h>

#define max_entradas 1
#define plant_types 5
typedef struct{
    uint umidade_min;//regar quando abaixo desse valor
    uint temperatura;//Indica a temperatura considerada calor, para aumentar a frequencia de rega da planta
    uint32_t frequencia;//frequência de rega
    uint32_t frequencia_quente;//frequência de rega no calor
}especie;

//Somente de exemplo
static char *lista_de_especies[] = {
    "Suculenta",
    "Samambaia",
    "Tomateiro",
    "Orquidea",
    "Hortela"
};

typedef struct{
    double sensor_temperatura;//Guarda porta do sensor de temperatura
    double sensor_umidade;//Guarda porta do sensor de umidade
    uint bomba_dagua;//Guarda porta ligada a bomba d'água
}input;

typedef struct{
    input entrada;
    especie especificacoes;
    uint tipo;//número que indica a especie do jardim
    bool rega;//indica se a planta está sendo regada
    bool active;//Indica se o guardião do vetor de guardiões está ativo
    uint32_t last_drop;//registra a última vez que foi regada
}planta;


static uint guardian_count = 0;

void interface_plant_init(planta *guardiao);
void interface_set_specie();

//user_interface.c
void interface_icon(ssd1306_t * ssd);
void interface_initial_screen(ssd1306_t * ssd, uint * momento,bool * select,bool *reset);
void interface_select_moment(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint *atual);

void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max);
void interface_register_input(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_register_specie(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_print_especie(ssd1306_t * ssd, uint tipo,uint x,uint y);
void interface_especification_init(uint atual, uint tipo, planta *guardiao);
void interface_select_guardian(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_guardian_screen(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_print_guardian_data(ssd1306_t * ssd, planta *guardiao, uint atual);

//process_data.c
void process_guardian_data(uint atual, planta *guardiao);







#endif//user_interface_h