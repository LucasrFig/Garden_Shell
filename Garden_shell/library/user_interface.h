#ifndef user_interface_h
#define user_interface_h
#include "display/ssd1306.h"
#include "process_data.h"
#include <stdio.h>

#define max_entradas 1

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
    double sensor_temperatura;
    double sensor_umidade;
    uint bomba_dagua;
}input;

typedef struct{
    input entrada;
    especie especificacoes;
    uint tipo;
    bool rega;
    bool active;
    uint32_t last_drop;//registra a última vez que foi regada
}planta;

typedef struct{
    bool set_active;
    uint hour;
    uint minutes;
}time;

static especie especificacoes[max_entradas];
static time time_now;
static uint guardian_count = 0;

void interface_plant_init(planta *guardiao);
void interface_set_specie();


void interface_icon(ssd1306_t * ssd);
void interface_initial_screen(ssd1306_t * ssd, uint * momento,bool * select,bool *reset);
void interface_select_moment(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint *atual);
void interface_set_hour(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max);
void interface_set_minute(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max);
void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max);
void interface_register_input(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_register_specie(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_especification_init(uint atual, uint tipo, planta *guardiao);
void interface_select_guardian(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_guardian_screen(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_print_guardian_data(ssd1306_t * ssd, planta *guardiao, uint atual);


void process_guardian_data(uint atual, planta *guardiao);







#endif//user_interface_h