#include "user_interface.h"
#include "display/ssd1306.h"
#include "display/font.h"
#include <string.h>

void interface_plant_init(planta * guardian){
    for(uint i=0;i<max_entradas;i++){
        guardian[i].active=0;
        guardian[i].rega = false;
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
    *momento = 0;
    *reset = true;
}

}

void interface_select_moment(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint*atual){
    ssd1306_fill(ssd,false);
    if(*reset){
        *reset = false;
        *select = false;
        *option = 1;
    }
    switch(*momento){
        case 0: interface_set_hour(ssd,momento,option,select,reset,max);
        break;

        case 1: interface_initial_screen(ssd,momento,select,reset);
        break;
        
        case 2: interface_option_screen(ssd,momento,option,select,reset,max);
        break;
        
        case 3: interface_register_input(ssd,momento,option,select,reset,max,atual);
        break;
        
        case 4: interface_select_guardian(ssd,momento,option,select,reset,max,atual);
        break;
        
        case 5: interface_guardian_screen(ssd,momento,option,select,reset,max,atual);
        break;
        
        case 6: interface_register_specie(ssd,momento,option,select,reset,max,atual);
        break;

        case 7:interface_set_minute(ssd,momento,option,select,reset,max);
        break;
    }
}

void interface_set_hour(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max){
    *max = 24;
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,"Definir hora",3,2);
    
    char buffer2[20] = ":00";
    char buffer1[20];
    sprintf(buffer1,"%2d",*option - 1);
    strcat(buffer1,buffer2);
    ssd1306_draw_string(ssd, &buffer1, 40, 35);

    if(*select){
        time_now.hour = *option - 1;
        *momento = 7;//escolher minutos
        *reset = 1;
    }
    ssd1306_send_data(ssd);
}

void interface_set_minute(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max){
    *max = 6;
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,"Definir hora",3,2);
    
    char buffer1[20];
    char buffer2[20];
    char buffer3[20] ="0";
    sprintf(buffer1,"%2d:",time_now.hour);
    sprintf(buffer2,"%d",*option-1);
    strcat(buffer1,buffer2);
    strcat(buffer1,buffer3);
    ssd1306_draw_string(ssd, &buffer1, 40, 35);
    
    if(*select){
        time_now.minutes = (*option - 1)*10;
        time_now.set_active = true;//Ativa o contador de tempo
        ssd1306_draw_string(ssd, "hora definida:", 10, 35);
        ssd1306_draw_string(ssd, &buffer1, 40, 45);
        ssd1306_send_data(ssd);
        sleep_ms(1500);
        *momento = 2;//vai para a tela de opções
        *reset = 1;
    }
    ssd1306_send_data(ssd);
}


void interface_option_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max){
    *max = 2;
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,"Tela inicial",3,2);
    ssd1306_draw_string(ssd,"Registrar",15,16);
    ssd1306_draw_string(ssd,"guardiao",15,28);
    ssd1306_draw_string(ssd,"Monitorar",15,42);
    ssd1306_draw_string(ssd,"guardioes",15,54);
    
    switch (*option)
    {
        case 1:
            ssd1306_rect(ssd,20,2,8,8,true,true);
            ssd1306_rect(ssd,46,2,8,8,true,false);
            if(*select){
                * momento = 3;//Vai para tela de "Registrar entrada do guardião"
                * reset = true;
            }
        break;
        case 2:
            ssd1306_rect(ssd,20,2,8,8,true,false);
            ssd1306_rect(ssd,46,2,8,8,true,true);
            if(*select){
                * momento = 4;//Vai para tela de "Monitorar guardiões"
                * reset = true;
            }
        break;
    }
    ssd1306_send_data(ssd);
}

void interface_register_input(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset, uint *max,uint *atual){
    *max = max_entradas;

    char buffer1[20] = "entrada ";
    char buffer2[20];
    sprintf(buffer2, "%d", *option-1);
    strcat(buffer1,buffer2);
    //Imprimir tela
    ssd1306_rect(ssd, 0, 0, 127, 12, true, false);
    ssd1306_draw_string(ssd, "entrada:", 3, 2);
    ssd1306_draw_string(ssd, &buffer1, 30, 30);
    ssd1306_draw_string(ssd, "B confirmar:", 3, 53);
    ssd1306_send_data(ssd);

    if(*select){
        if(guardiao[*option - 1].active == false){
            guardiao[*option - 1].active = true;
            guardiao[*option - 1].entrada = entrada[*option - 1];
            *atual =  *option - 1;
            *momento = 6;//Vai selecionar o tipo de planta do guardião
            *reset = true;
            
        }else{
            ssd1306_draw_string(ssd,"entrada em uso", 3, 30);
            ssd1306_send_data(ssd);
            sleep_ms(1000);
            *select = 0;
        }
    } 
    
}

void interface_register_specie(ssd1306_t * ssd, uint * momento, uint *option, bool *select, bool *reset,uint *max,uint*atual) {
    *max = 3;
    char buffer[20];
    // Usando strcpy para copiar a string da lista para o buffer
    strcpy(buffer, lista_de_especies[*option - 1]);
    ssd1306_rect(ssd, 0, 0, 127, 12, true, false);
    ssd1306_draw_string(ssd, "especie:", 3, 2);
    ssd1306_draw_string(ssd, &buffer, 30, 35);
    
    if(*select){
        guardiao[*atual].tipo = *option - 1;
        *momento = 2;
        *reset = 1;
        guardian_count ++;
    }
    
    ssd1306_send_data(ssd);
}

void interface_select_guardian(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint*atual){
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
                sprintf(buffer2, ": E%d", *option-1);
                strcat(buffer1,buffer2);
                ssd1306_draw_string(ssd, &buffer1, 20, 30);
            

        }else{//Caso seja a opção "voltar"
            ssd1306_draw_string(ssd,"Voltar", 20, 30);
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

void interface_guardian_screen(ssd1306_t * ssd, uint * momento, uint *option,bool *select,bool *reset,uint *max,uint*atual){
    *max = 3;
    char buffer1[20];
    char buffer2[20];
    strcpy(buffer1,lista_de_especies[(guardiao[*atual].tipo)]);
    sprintf(buffer2, ": E%d", *atual);
    strcat(buffer1,buffer2);
    
    ssd1306_rect(ssd,0,0,127,12,true,false);
    ssd1306_draw_string(ssd,buffer1,3,2);//Escrever nome e entrada do guardião
    
    interface_print_guardian_data(*atual);//envia dados do guardião exibido para o display.
    
    //Apresentar opções
    switch (*option)
    {
        case 1://voltar
            ssd1306_draw_string(ssd,"Voltar",40,55);
            break;
        case 2://Regar
            ssd1306_draw_string(ssd,"Regar",40,55);
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

void interface_print_guardian_data(uint atual){
    guardiao[atual].entrada.sensor_luz;
    guardiao[atual].entrada.sensor_temperatura;
    guardiao[atual].entrada.sensor_umidade;
    guardiao[atual].rega;
}