#include "user_interface.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

void process_guardian_data(uint atual, planta *guardiao){

    if(guardiao[atual].active){//Se o guardião estiver ativo, atualiza os dados, executa os processos
        double aux;
        uint aux2;

        //ATUALIZAR DADOS:////////////////////////////////////////////////////////////
        //Ler sensor umidade
        adc_select_input(0); 
        uint16_t umidade_value = adc_read();
        aux = (umidade_value*100)/4095;//Lê porcentagem do sensor
        guardiao[atual].entrada.sensor_umidade = aux;
        //printf("voltagem: %2lf||ADC:%d||umidade: %lf%%\n",aux,umidade_value,guardiao[atual].entrada.sensor_umidade);//debug


        //Ler sensor temperatura:
        adc_select_input(1); 
        uint16_t temperature_value = adc_read();

        if(temperature_value>=1800 && temperature_value<=2300){//Se estiver no centro 21graus
            aux2 = 800;
        }else if(temperature_value > 2300){//Caso aumente pra direita até 70 graus
            aux2 = 800+temperature_value - 2300;
        }else if(temperature_value <1800){//Caso aumente pra esquerda até 70 graus
            aux2 = 1800 - temperature_value+800;
        }
        aux = (aux2*(1.1/4095));//converte em voltagem
        guardiao[atual].entrada.sensor_temperatura = (double) aux/0.01;//converte em temperatura
        //printf("voltagem: %2lf||ADC:%d||temperatura: %lf\n",aux,temperature_value,guardiao[atual].entrada.sensor_temperatura);//debug
        
        

        //guarda tempo atual
        uint32_t current_time = to_us_since_boot(get_absolute_time());


        //PROCESSAR DADOS LIDOS://////////////////////////////////////////////////////
        
        //verificar se nível de umidade está abaixo do mínimo especificado:
        if(guardiao[atual].entrada.sensor_umidade<guardiao[atual].especificacoes.umidade_min){
            guardiao[atual].rega = true;
            guardiao[atual].last_drop = current_time;
        }

        /*verifica se o tempo desde a última rega atingiu o especificado de acordo com a temperatura e umidade.
        Temperatura alta siginifica uma frequência de rega maior. A bomba d'água só ativa caso o tempo atinja
        a frequencia especificada e a umidade esteja abaixo do nível especificado + 20% e desliga a bomba quando 
        a umidade atingir o nível especificado + 30%*/

        //Verifica nível de umidade é "especifiado + 20%"
        if(guardiao[atual].entrada.sensor_umidade < (guardiao[atual].especificacoes.umidade_min + 20)){
            
            //Se a temperatura for maior que a especificada, usar frequencia de calor:
            if(guardiao[atual].entrada.sensor_temperatura >= guardiao[atual].especificacoes.temperatura){
                if((current_time - guardiao[atual].last_drop) > guardiao[atual].especificacoes.frequencia_quente){
                    guardiao[atual].rega = true;//liga bomba
                    guardiao[atual].last_drop = current_time;
                } 
                
                //Se a temperatura for menor que a especificada, usar frequencia normal:
            }else{
                if((current_time - guardiao[atual].last_drop) > guardiao[atual].especificacoes.frequencia){
                    guardiao[atual].rega = true;//liga bomba
                    guardiao[atual].last_drop = current_time;
                }
            }
        }
        
        //Caso esteja sendo regado:
        if(guardiao[atual].rega){
            //Verifica se nivel de umidade passou o "especificado + 30%"
            if(guardiao[atual].entrada.sensor_umidade >= (guardiao[atual].especificacoes.umidade_min + 30)){
                guardiao[atual].rega = false;//desliga bomba
            }
        }

        //liga ou desliga a bomba do módulo de acordo com os dados acima
        gpio_put(guardiao[atual].entrada.bomba_dagua,guardiao[atual].rega);
    }else{//Se não estiver ativo, desliga a bomba d'água do módulo caso esteja ligada
        gpio_put(guardiao[atual].entrada.bomba_dagua,0);
    }
}