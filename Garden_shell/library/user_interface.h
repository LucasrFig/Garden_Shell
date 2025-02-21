#ifndef user_interface_h
#define user_interface_h
#include "display/ssd1306.h"
#include <stdio.h>

#define max_entradas 2


typedef struct{
    uint sensor_luz;
    uint sensor_temperatura;
    uint sensor_umidade;
}input;

typedef struct{
    input entrada;
    uint tipo;
    bool rega;
    bool active;
}planta;

typedef struct{
    bool set_active;
    uint hour;
    uint minutes;
}time;

static char *lista_de_especies[] = {
    "Cacto",
    "Roseira",
    "raiz"
};

static time time_now;
static planta guardiao[6]={};
static input entrada[max_entradas];
static uint guardian_count = 0;

void interface_plant_init(planta * guardian);
void interface_set_specie();


void interface_icon(ssd1306_t * ssd);
void interface_initial_screen(ssd1306_t * ssd, uint * momento,bool * select,bool *reset);
void interface_select_moment(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint *atual);
void interface_set_hour(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max);
void interface_set_minute(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max);
void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max);
void interface_register_input(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_register_specie(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint*atual);
void interface_select_guardian(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_guardian_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint*atual);
void interface_print_guardian_data(uint atual);










#endif//user_interface_h