#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "display/ssd1306.h"
#include "display/font.h"
#include "library/user_interface.h"
#include "library/peripherals.h"

//Definição das macros:
#define BUT_A 5
#define BUT_B 6
#define BUT_J 22//Vai representar um sensor de luminosidade

#define JOY_X 27//Vai Representar um sensor de temperatura
#define JOY_Y 26//Vai Representar um sensor de umidade


//Definição de variáveis globais:
uint32_t last_timeB = 0;
uint32_t last_timeA = 0;

ssd1306_t ssd;

uint momento = 1;//Serve pra definir a tela exibida no display
uint current_option=1;
bool select_op =false;
bool reset=false;
uint max=1;
uint atual;





static void interface_irq_handler(uint gpio,uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_timeB > 300000){ //debouncing
        last_timeB = current_time;
        if(gpio==BUT_B){
            select_op = true;
        }
    }

    if (current_time - last_timeA > 300000){ //debouncing
        last_timeA = current_time;
        if(gpio==BUT_A){
            if(current_option<max){
                current_option++;
            }else{
                current_option = 1;
            }
        }
    }

}

//Função principal
int main()
{
    //Inicializa sistema de entrada e saída padrão
    stdio_init_all();
    //Inicializa o i2c, inicializa a estrutura do display, limpa o display
    ssd1306_init_config_clean(&ssd, I2C_SCL,I2C_SDA,I2C_PORT,I2C_LINK);
    //Inicializa o ADC para os sensores
    adc_init();
    //inicializa botões
    gpio_init_button(BUT_A);
    gpio_init_button(BUT_B);
    gpio_init_button(BUT_J);

    //Habilita interrupções
    gpio_set_irq_enabled_with_callback(BUT_A, GPIO_IRQ_EDGE_FALL, 1, &interface_irq_handler);
    gpio_set_irq_enabled_with_callback(BUT_B, GPIO_IRQ_EDGE_FALL, 1, &interface_irq_handler);
    gpio_set_irq_enabled_with_callback(BUT_J, GPIO_IRQ_EDGE_FALL, 1, &interface_irq_handler);

    //Inicializa vetor dos guardiões
    interface_plant_init(&guardiao);
    
    //Inicializa portas da entrada 0
    entrada[0].sensor_luz = 13;//led vermelho
    entrada[0].sensor_temperetura = JOY_Y;//eixo y do joystick
    entrada[0].sensor_umidade = JOY_X; //eixo x do joystick

    //Apresenta ícone e nome do projeto
    interface_icon(&ssd);
    sleep_ms(3000);

    
    while (true) {
        printf("momento:%d\n",momento);
        printf("select:%d\n",select_op);
        interface_select_moment(&ssd,&momento,&current_option,&select_op,&reset,&max,&atual);
        sleep_ms(10);
    }
}

