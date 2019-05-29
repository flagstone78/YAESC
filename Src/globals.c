#include "globals.h"

volatile uint16_t ConnectionEstablished = 0;

volatile uint16_t MAXPOWER = 3559; //1770;
volatile uint16_t POWER = 0; 


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


enum PhaseStateEnum{
	AB,
	CB,
	CA,
	BA,
	BC,
	AC
} PHASESTATE = AB;

void userReceive64(uint8_t* Buf, uint32_t *Len){
	POWER = (Buf[0] << 8) + Buf[1];
	if(POWER > MAXPOWER){POWER = MAXPOWER;}
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,POWER);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,POWER);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,POWER);
	ConnectionEstablished = 1;
	PHASESTATE = (PHASESTATE+1)%6;
}

uint32_t ADCBuffer[ADCBufferLength];

uint16_t testpower = 0;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	testpower += 50;
	testpower = testpower % MAXPOWER;
	//setPWM(testpower, 1, (testpower+MAXPOWER/3)%MAXPOWER,1,(testpower+2*MAXPOWER/3)%MAXPOWER,1);
	
	//phase state machine
	switch(PHASESTATE){
		case AB:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x055); //TODO: need to synchronise with shadow ccr registers
			//turn off phases
			TIM1->CCR2 = 0;
			//turn on phases
			TIM1->CCR1 = POWER;
			TIM1->CCR3 = POWER;
			break;
		case CB:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x550);
			//turn off phases
			TIM1->CCR2 = 0;
			//turn on phases
			TIM1->CCR1 = POWER;
			TIM1->CCR3 = POWER;
			break;
		case CA:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x505);
			//turn off phases
			TIM1->CCR1 = 0;
			//turn on phases
			TIM1->CCR2 = POWER;
			TIM1->CCR3 = POWER;
			break;
		case BA:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x055);
			//turn off phases
			TIM1->CCR1 = 0;
			//turn on phases
			TIM1->CCR2 = POWER;
			TIM1->CCR3 = POWER;
			break;
		case BC:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x550);
			//turn off phases
			TIM1->CCR3 = 0;
			//turn on phases
			TIM1->CCR1 = POWER;
			TIM1->CCR2 = POWER;
			break;
		case AC:
			//disable and enable phases
			TIM1->CCER ^= (TIM1->CCER^0x505);
			//turn off phases
			TIM1->CCR3 = 0;
			//turn on phases
			TIM1->CCR1 = POWER;
			TIM1->CCR2 = POWER;
			break;
	default:
		PHASESTATE = AB;
		POWER=0;
		break;
	}
}

//? Use DMA to write to pwm registers????
//? Use MOE bit to disable pwm phase?? 
void setPWM(uint16_t p1Val, uint16_t p1En, uint16_t p2Val, uint16_t p2En, uint16_t p3Val, uint16_t p3En){
	//clamp inputs to valid rande
	if(p1Val > MAXPOWER){ p1Val = MAXPOWER; }
	if(p2Val > MAXPOWER){ p2Val = MAXPOWER; }
	if(p3Val > MAXPOWER){ p3Val = MAXPOWER; }
	
	
	if(p1En){
		
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,p1Val);
	} else {
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0); //off
	}
	
	if(p2En){
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,p2Val);
	} else {
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	}
	
	if(p2En){
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,p3Val);
	} else {
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	}
	
}
