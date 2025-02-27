#include "user_interface.h"
#include "display/ssd1306.h"
#include "display/font.h"
#include <string.h>

void interface_plant_init(planta *guardiao){
    for(uint i=0;i<max_entradas;i++){
        guardiao[i].active=0;
        guardiao[i].rega = false;
        guardiao[i].last_drop = 0;
    }
}

void interface_icon(ssd1306_t * ssd){
    ssd1306_fill(ssd,false);
    ssd1306_draw_48x48_image(ssd,bitmap_garden_shell_48x48,10,10);
    ssd1306_draw_string(ssd,"Garden",65,20);
    ssd1306_draw_string(ssd,"Shell",70,32);
    ssd1306_send_data(ssd);
}

void interface_initial_screen(ssd1306_t * ssd, uint * momento,bool * select,bool *reset){
    if(!*select){
    ssd1306_draw_40x40_image(ssd,bitmap_garden_shell_40x40,40,2);
    ssd1306_draw_string(ssd,"Aperte B para",10,40);
    ssd1306_draw_string(ssd,"iniciar",35,52);
    ssd1306_send_data(ssd);
}else{
    *momento = 2;
    *reset = true;
}

}

void interface_select_moment(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint*atual){
    ssd1306_fill(ssd,false);
    if(*reset){
        *reset = false;
        *select = false;
        *option = 1;
    }
    switch(*momento){

        case 1: interface_initial_screen(ssd,momento,select,reset);
        break;
        
        case 2: interface_option_screen(ssd,momento,option,select,reset,max);
        break;
        
        case 3: interface_register_input(ssd,momento,entrada,guardiao,option,select,reset,max,atual);
        break;
        
        case 4: interface_select_guardian(ssd,momento,guardiao,option,select,reset,max,atual);
        break;
        
        case 5: interface_guardian_screen(ssd,momento,guardiao,option,select,reset,max,atual);
        break;
        
        case 6: interface_register_specie(ssd,momento,guardiao,option,select,reset,max,atual);
        break;

    }
}

void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max){
    *max = 2;
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,"Tela inicial",3,2);
    ssd1306_draw_string(ssd,"Novo Guardiao",15,20);
    ssd1306_draw_string(ssd,"Ver Guardioes",15,32);
    
    switch (*option)
    {
        case 1:
            ssd1306_rect(ssd,20,2,8,8,true,true);
            ssd1306_rect(ssd,32,2,8,8,true,false);
            if(*select){
                * momento = 3;//Vai para tela de "Registrar entrada do guardião"
                * reset = true;
            }
        break;
        case 2:
            ssd1306_rect(ssd,20,2,8,8,true,false);
            ssd1306_rect(ssd,32,2,8,8,true,true);
            if(*select){
                * momento = 4;//Vai para tela de "Monitorar guardiões"
                * reset = true;
            }
        break;
    }
    ssd1306_send_data(ssd);
}

void interface_register_input(ssd1306_t * ssd, uint * momento,input *entrada, planta *guardiao, uint *option,bool *select,bool *reset, uint *max,uint *atual){
    *max = max_entradas;

    char buffer1[20] = "entrada ";
    char buffer2[20];
    sprintf(buffer2, "%d", *option-1);
    strcat(buffer1,buffer2);
    //Imprimir tela
    ssd1306_rect(ssd, 0, 0, 127, 12, true, false);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_left_16x16,10,25);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_right_16x16,107,25);
    ssd1306_draw_string(ssd, "entrada:", 3, 2);
    ssd1306_draw_string(ssd, buffer1, 30, 30);
    ssd1306_draw_string(ssd, "B confirmar:", 3, 53);
    ssd1306_send_data(ssd);

    if(*select){
        if(guardiao[*option - 1].active == false){
            printf("saida de agua:%d\n",entrada[*option - 1].bomba_dagua);//debug
            guardiao[*option - 1].entrada.bomba_dagua = entrada[*option - 1].bomba_dagua;
            guardiao[*option - 1].entrada.sensor_temperatura = entrada[*option - 1].sensor_temperatura;
            guardiao[*option - 1].entrada.sensor_umidade = entrada[*option - 1].sensor_umidade;
            *atual =  *option - 1;
            *momento = 6;//Vai selecionar o tipo de planta do guardião
            *reset = true;
            
        }else{
            ssd1306_draw_string(ssd,"entrada em uso", 3, 30);
            ssd1306_send_data(ssd);
            sleep_ms(1000);
            *momento = 2;
            *reset = 1;
        }
    } 
    
}

void interface_register_specie(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option, bool *select, bool *reset,uint *max,uint*atual) {
    *max = 5;
    char buffer[20];
    // Usando strcpy para copiar a string da lista para o buffer
    strcpy(buffer, lista_de_especies[*option - 1]);
    ssd1306_rect(ssd, 0, 0, 127, 12, true, false);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_left_16x16,15,35);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_right_16x16,95,35);
    ssd1306_draw_string(ssd, "especie:", 3, 2);
    ssd1306_draw_string(ssd, buffer, 30, 15);

    interface_print_especie(ssd,*option - 1, 40,22);

    if(*select){
        guardiao[*atual].tipo = *option - 1;
        interface_especification_init(*atual,guardiao[*atual].tipo,guardiao);
        *momento = 2;
        *reset = 1;
        guardiao[*atual].active = 1;
        guardiao[*atual].last_drop = to_us_since_boot(get_absolute_time());
        printf("%d - %s\n",*atual,guardiao[*atual].active?"guardiao ativo":"guardiao inativo");
        guardian_count ++;
    }
    
    ssd1306_send_data(ssd);
}

void interface_select_guardian(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual){
    *max = max_entradas+1;
    
    if(guardian_count){//Se houverem guardiões registrados, entra na tela de seleção
        ssd1306_rect(ssd, 0, 0, 127, 12, true, false);
        ssd1306_draw_string(ssd, "Monitoramento", 3, 2);
        ssd1306_draw_string(ssd, "B confirmar:", 3, 53);

        if(!guardiao[*option-1].active){
            *option+=1;
        }

        if(*option<=max_entradas){//Caso seja uma planta
            char buffer1[20];
            char buffer2[20];
                strcpy(buffer1,lista_de_especies[(guardiao[*option-1].tipo)]);
                sprintf(buffer2, ":E%d", *option-1);
                strcat(buffer1,buffer2);
                ssd1306_draw_string(ssd, buffer1, 15, 25);
            

        }else{//Caso seja a opção "voltar"
            ssd1306_draw_string(ssd,"Voltar", 40, 25);
        }

        if(*select){
            if(*option<=max_entradas){//Caso seja uma planta
                *momento = 5;
                *reset = 1;
                *atual = *option - 1;
            }else{//Caso seja a opção "voltar"
                *momento = 2;
                *reset = 1;
            }
        }
    }else{//caso não haja guardiões registrados
        ssd1306_draw_string(ssd, "Sem guardioes", 15, 25);
        ssd1306_draw_string(ssd, "registrados", 25, 36);
        ssd1306_send_data(ssd);
        sleep_ms(1000);
        *momento = 2;
        *reset = 1;
    }
    ssd1306_send_data(ssd);
}

void interface_guardian_screen(ssd1306_t * ssd, uint * momento, planta *guardiao, uint *option,bool *select,bool *reset,uint *max,uint*atual){
    *max = 3;
    char buffer1[20];
    char buffer2[20];
    strcpy(buffer1,lista_de_especies[(guardiao[*atual].tipo)]);
    sprintf(buffer2, ": E%d", *atual);
    strcat(buffer1,buffer2);
    
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,buffer1,3,2);//Escrever nome e entrada do guardião
    
    //envia dados do guardião exibido para o display
    //temperatura
    sprintf(buffer1,"%d",(int)guardiao[*atual].entrada.sensor_temperatura);
    ssd1306_draw_string(ssd,buffer1,20,40);
    ssd1306_draw_16x16_image(ssd,bitmap_thermometer_16x16,0,35);

    //Umidade
    ssd1306_rect(ssd,20,25,60,10,true,false);
    uint valor_display = (int)guardiao[*atual].entrada.sensor_umidade;
    valor_display *= 60;
    valor_display /= 100;
    ssd1306_rect(ssd,20,25,valor_display,10,true,true);
    ssd1306_draw_16x16_image(ssd,bitmap_waterdrop_16x16,0,16);
    ssd1306_draw_16x16_image(ssd,bitmap_PERCENTAGE_16x16,105,18);
    ssd1306_draw_16x16_image(ssd,bitmap_PERCENTAGE_16x16,105,18);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_left_16x16,20,48);
    ssd1306_draw_16x16_image(ssd,bitmap_arrow_right_16x16,100,48);

    sprintf(buffer1,"%d",(int)guardiao[*atual].entrada.sensor_umidade);
    ssd1306_draw_string(ssd,buffer1,90,22);
    
    //Água on/off

    if(guardiao[*atual].rega){
        ssd1306_draw_string(ssd,"agua: ON",50,40);
    }else{
        ssd1306_draw_string(ssd,"agua: OFF",50,40);
    }

    //Apresentar opções
    switch (*option)
    {
        case 1://voltar
            ssd1306_draw_string(ssd,"Voltar",40,55);
            break;
        case 2://Regar
            ssd1306_draw_string(ssd,"Regar",45,55);
            break;
        case 3://Excluir
            ssd1306_draw_string(ssd,"Excluir",40,55);
            break;
    }

    //Caso alguma seja selecionada
    if(*select){
        switch (*option)
        {
            case 1://voltar
                *momento = 4;
                *reset = 1;
                break;
            case 2://Regar
                guardiao[*atual].rega = true;
                gpio_put(guardiao[*atual].entrada.bomba_dagua,guardiao[*atual].rega);//liga a bomba
                guardiao[*atual].last_drop = to_us_since_boot(get_absolute_time());
                *select = 0;
                break;
            case 3://Excluir
                guardiao[*atual].rega = false;
                guardiao[*atual].active = false;
                guardian_count --;
                *momento = 4;
                *reset = 1;
                break;
        }
    }

    ssd1306_send_data(ssd);
}

void interface_print_especie(ssd1306_t * ssd, uint tipo,uint x,uint y){
    switch (tipo)
    {
        case 0://suculenta
            ssd1306_draw_40x40_image(ssd,bitmap_suculenta_40x40,x,y);
        break;

        case 1://samambaia
            ssd1306_draw_40x40_image(ssd,bitmap_samambaia_40x40,x,y);
        break;

        case 2://tomateiro
            ssd1306_draw_40x40_image(ssd,bitmap_tomateiro_40x40,x,y);
        break;

        case 3://Orquídea
            ssd1306_draw_40x40_image(ssd,bitmap_orquidea_40x40,x,y);
        break;

        case 4://Hortelã
            ssd1306_draw_40x40_image(ssd,bitmap_hortela_40x40,x,y);
        break;
    }
    ssd1306_send_data(ssd);
}
void interface_especification_init(uint atual, uint tipo, planta *guardiao){
    switch (tipo)
    {
        case 0://suculenta
            guardiao[atual].especificacoes.umidade_min = 20;
            guardiao[atual].especificacoes.temperatura = 30;
            guardiao[atual].especificacoes.frequencia = 240000000;//240 seg = 10 dias na simulação
            guardiao[atual].especificacoes.frequencia_quente = 120000000;//120 seg = 5 dias na simulação
        break;

        case 1://samambaia
            guardiao[atual].especificacoes.umidade_min = 50;
            guardiao[atual].especificacoes.temperatura = 25;
            guardiao[atual].especificacoes.frequencia = 48000000;//48 seg = 2 dias na simulação
            guardiao[atual].especificacoes.frequencia_quente = 24000000;//24 seg = 1 dia na simulação
        break;

        case 2://tomateiro
            guardiao[atual].especificacoes.umidade_min = 40;
            guardiao[atual].especificacoes.temperatura = 28;
            guardiao[atual].especificacoes.frequencia = 48000000;//48 seg = 2 dias na simulação
            guardiao[atual].especificacoes.frequencia_quente = 24000000;//24 seg = 1 dia na simulação
        break;

        case 3://Orquídea
            guardiao[atual].especificacoes.umidade_min = 30;
            guardiao[atual].especificacoes.temperatura = 25;
            guardiao[atual].especificacoes.frequencia = 120000000;//120 seg = 5 dias na simulação
            guardiao[atual].especificacoes.frequencia_quente = 60000000;//60 seg = 2.5 dias na simulação
        break;

        case 4://Hortelã
            guardiao[atual].especificacoes.umidade_min = 50; 
            guardiao[atual].especificacoes.temperatura = 25;
            guardiao[atual].especificacoes.frequencia = 48000000;//48 seg = 2 dias na simulação
            guardiao[atual].especificacoes.frequencia_quente = 24000000;//24 seg = 1 dia na simulação
        break;
    }
}