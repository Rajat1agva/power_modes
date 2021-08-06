/*
 * battery_supply_test.c
 *
 * Created: 8/5/2021 10:42:12 AM
 * 
 *  Author : RAJAT 
 */ 
#define  F_CPU	4000000UL
#include <avr/io.h>
#include<math.h>
#include <util/delay.h>
#include "ADC_AVR128DA64 (1).h"
#include "UART_1_AVR128DA64.h"
float battery_Read(void);
float supply_Read(void);
void supplyconnected(void);
void supplydisconnected(void);
float battery_voltage=0;
float supply_voltage=0;
int main(void)
{   ADC0_init();  //Initialize ADC
	ADC0_start(); //Start ADC
	USART1_init(9600);
    /* Replace with your application code */
   while(1)
   {
	   // USART1_sendFloat(battery_Read(),1);
		//USART1_sendFloat(supply_Read(),1);    
		
	   if(supply_Read()>10)
	   {
		   supplyconnected();
	   }
	   else if(supply_Read()<=10)
	   {
		 supplydisconnected();
	   }
	   
	  
   }

}

float battery_Read(void)
{
uint16_t adc_value=ADC0_read(1);
float voltage_value=adc_value/1241.2121; 
return (voltage_value*3.636);
//USART1_sendInt(adc_value);
}
float supply_Read(void)
{
	uint16_t adc_value=ADC0_read(0);
	float voltage_value=0,final_result=0;
	voltage_value=(float)((adc_value)/1241.2121);
	//USART1_sendInt(adc_value);
	final_result=voltage_value*3.636;
return (final_result);
}
void supplyconnected(void)
{
	USART1_sendString("Supply connected");
	_delay_ms(100);
	//USART1_sendFloat(battery_Read(),1);
	if(battery_Read()>11)
	{
		USART1_sendString("Charging complete");
	}
	else if((battery_Read()<11)&&(supply_Read()>10)){
	
	
		USART1_sendString("Charging");
	
	}
	
	
	
}
void supplydisconnected(void)
{
	 USART1_sendString("Supply disconnected");
	 USART1_sendFloat(supply_Read(),1); 
	 if(battery_Read()<9)
	 {
		 USART1_sendString("Low battery");
	 }
	else
	{
	while((battery_Read()<11)&&(supply_Read()<10))
	{
		USART1_sendFloat(battery_Read(),1);
	}
	
	}
} 
