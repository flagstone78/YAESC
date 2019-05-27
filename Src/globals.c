#include "globals.h"

//currently disabled
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(ConnectionEstablished){ //check if usb communication established
		//uint32_t data = HAL_ADC_GetValue(hadc);
		//uint8_t Buf[2] = {(data >> 8), data};
		CDC_Transmit_FS((uint8_t*)ADCBuffer, ADCBufferLength);
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle){
	if(ConnectionEstablished){
		//CDC_Transmit_FS((uint8_t*)(ADCBuffer+ADCBufferLength/2), ADCBufferLength);
	}
}

volatile uint16_t ConnectionEstablished = 0;

uint16_t MAXPOWER = 1770;

void userReceive64(uint8_t* Buf, uint32_t *Len){
	uint16_t power = (Buf[0] << 8) + Buf[1];
	if(power > MAXPOWER){power=MAXPOWER;}
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,power);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,power);
	ConnectionEstablished = 1;
}

uint32_t ADCBuffer[ADCBufferLength];
