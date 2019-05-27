#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

//pwm power level

extern volatile uint16_t ConnectionEstablished;

extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

void userReceive64(uint8_t* Buf, uint32_t *Len);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle);

#define ADCBufferLength 3200
extern uint32_t ADCBuffer[ADCBufferLength];

#endif
