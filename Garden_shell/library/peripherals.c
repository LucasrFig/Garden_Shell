#include "peripherals.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

void gpio_init_button(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio,GPIO_IN);
    gpio_pull_up(gpio);
}

void gpio_init_led(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio,GPIO_OUT);
}
