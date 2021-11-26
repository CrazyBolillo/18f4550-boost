#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include "lcd.h"
#include "adc.h"

#define ADC_TARGET_CHANNEL 0

uint16_t adc_read_value;
float adc_voltage;

char buffer_one[17];
char buffer_two[17];

void set_duty_cycle(uint16_t duty) {
    CCPR1L = (uint8_t) (duty >> 2);
    CCP1CON &= 0xCF;
    CCP1CON |= ((duty & 0x03) << 4);
}

void main(void) {
    OSCCON = 0x73; // Using internal oscillator as system clock @8M Hz
    
    
    PORTC = 0x00;
    TRISC = 0x00;
    T2CON = 0x00;
    PR2 = 0xFF;
    set_duty_cycle(128);
    
    ADCON0bits.ADON = 1;
    ADCON1 = 0x0B; // AN0:4 as analog inputs
    ADCON2 = 0x36; // Left justified. 16 TAD. FOSC/64
    
    lcd_init(true, false, false);
    
    while (1) {
        lcd_clear_display();
        adc_set_channel(ADC_TARGET_CHANNEL);
        adc_read_voltage(&adc_read_value, &adc_voltage);
        sprintf(buffer_one, "Voltaje: %.2f", adc_voltage);
        lcd_write_string(buffer_one);
        __delay_ms(100);
    }
}
