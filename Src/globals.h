#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

//pwm power level

extern volatile uint16_t ConnectionEstablished;

extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim1;

extern volatile uint16_t MAXPOWER;
extern volatile uint16_t POWER;

void userReceive64(uint8_t* Buf, uint32_t *Len);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void setPWM(uint16_t p1Val, uint16_t p1En, uint16_t p2Val, uint16_t p2En, uint16_t p3Val, uint16_t p3En);

#define ADCBufferLength 3200
extern uint32_t ADCBuffer[ADCBufferLength];

#endif
