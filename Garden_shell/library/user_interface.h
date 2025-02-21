#ifndef user_interface_h
#define user_interface_h
#include "display/ssd1306.h"
#include <stdio.h>

#define max_entradas 1


typedef struct{
    uint sensor_luz;
    uint sensor_temperetura;
    uint sensor_umidade;
}input;

typedef struct{
    input entrada;
    uint tipo;
    bool rega;
    bool active;
}planta;

static char *lista_de_especies[] = {
    "Cacto",
    "Roseira",
    "raiz"
};

static planta guardiao[6]={};
static input entrada[max_entradas];
static uint guardian_count = 0;

void interface_plant_init(planta * guardian);
void interface_set_specie();


void interface_icon(ssd1306_t * ssd);
void interface_initial_screen(ssd1306_t * ssd, uint * momento,bool * select,bool *reset);
void interface_select_moment(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint *atual);
void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max);
void interface_register_input(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_register_specie(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_select_guardian(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max);











#endif//user_interface_h