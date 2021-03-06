/*
 * battery_supply_test.c
 *
 * Created: 8/5/2021 10:42:12 AM
 * 
 *  Author : RAJAT 
 */ 
#define  F_CPU	4000000UL
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "ADC_AVR128DA64 (1).h"
#include "UART_1_AVR128DA64.h"

float battery_Read(void);
float supply_Read(void);
void supplyconnected(void);
void supplydisconnected(void);


uint16_t ADC0_read(char pin);
float battery_voltage=0;
float supply_voltage=0;


int main(void)
{   ADC0_init();  //Initialize ADC
	ADC0_start(); //Start ADC
	USART1_init(9600);
    /* Replace with your application code */
   while(1)
   {  battery_voltage=battery_Read();
	  supply_voltage=supply_Read();
	 //USART1_sendFloat(battery_voltage,1);
	   if(supply_voltage > 11)
	   {
		   supplyconnected(); // Supply connected
	   }
	   else if(supply_voltage <= 10)
	   {
		 supplydisconnected(); // Supply disconnected
	   }
	   
	  
   }

}

float battery_Read(void)
{
uint16_t adc_value=ADC0_read(0x00);
float voltage_value=adc_value/1241.2121; 
return (voltage_value*3.636);

}
float supply_Read(void)
{
	uint16_t adc_value=ADC0_read(0x01);
	float voltage_value=0,final_result=0;
	voltage_value=(float)((adc_value)/1241.2121);
	final_result=voltage_value*3.636;
return (final_result);
}
void supplyconnected(void)
{
	USART1_sendString("Supply connected");
	
	
	if(battery_voltage>11)
	{
		USART1_sendString("Charging complete");
	}
	else if((battery_voltage<11)&&(supply_voltage>10)){
	
	
		USART1_sendString("Charging");
	
	}
	
	
	
}
void supplydisconnected(void)
{
	 USART1_sendString("Supply disconnected");
	
	 if(battery_voltage<8)
	 {
		 USART1_sendString("Low battery");
	 }
	else if(battery_voltage>9)
	{
	
	
		USART1_sendFloat(battery_voltage,1);
	
	
	}
}
uint16_t ADC0_read(char pin)
{
	ADC0.MUXPOS = pin;
	ADC0_start();
	/* Wait for ADC result to be ready */
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
	/* Clear the interrupt flag by reading the result */
	return ADC0.RES;
}
