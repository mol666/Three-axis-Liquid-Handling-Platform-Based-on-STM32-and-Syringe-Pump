#ifndef __JOYSTICK_H
#define	__JOYSTICK_H

#include "stm32f4xx_hal.h"

/*=====================ͨ��9 IO======================*/
// PF3
// ADC IO�궨��
#define JOYSTICK_ADC_GPIO_PORT_PUMP             GPIOF
#define JOYSTICK_ADC_GPIO_PIN_PUMP             	GPIO_PIN_3
#define JOYSTICK_ADC_GPIO_CLK_PUMP_ENABLE()     __GPIOF_CLK_ENABLE()
#define JOYSTICK_ADC_CHANNEL_PUMP               ADC_CHANNEL_9
/*=====================ͨ��14 IO ======================*/
// PF4 
// ADC IO�궨��
#define JOYSTICK_ADC_GPIO_PORT_Z             GPIOF
#define JOYSTICK_ADC_GPIO_PIN_Z              GPIO_PIN_4
#define JOYSTICK_ADC_GPIO_CLK_Z_ENABLE()     __GPIOF_CLK_ENABLE()
#define JOYSTICK_ADC_CHANNEL_Z               ADC_CHANNEL_14
/*=====================ͨ��15 IO ======================*/
// PF5 
// ADC IO�궨��
#define JOYSTICK_ADC_GPIO_PORT_X             GPIOF
#define JOYSTICK_ADC_GPIO_PIN_X              GPIO_PIN_5
#define JOYSTICK_ADC_GPIO_CLK_X_ENABLE()     __GPIOF_CLK_ENABLE()
#define JOYSTICK_ADC_CHANNEL_X               ADC_CHANNEL_15
/*=====================ͨ��4 IO ======================*/
// PF6 
// ADC IO�궨��
#define JOYSTICK_ADC_GPIO_PORT_Y             GPIOF
#define JOYSTICK_ADC_GPIO_PIN_Y               GPIO_PIN_6
#define JOYSTICK_ADC_GPIO_CLK_Y_ENABLE()     __GPIOF_CLK_ENABLE()
#define JOYSTICK_ADC_CHANNEL_Y                ADC_CHANNEL_4   
   
   
// ADC ��ź궨��
#define JOYSTICK_ADC                        ADC3
#define JOYSTICK_ADC_CLK_ENABLE()           __ADC3_CLK_ENABLE()

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define JOYSTICK_ADC_DR_ADDR                ((uint32_t)ADC3+0x4c)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����,DMA2ͨ��2������1
#define JOYSTICK_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define JOYSTICK_ADC_DMA_CHANNEL            DMA_CHANNEL_2
#define JOYSTICK_ADC_DMA_STREAM             DMA2_Stream1

void JOYSTICK_ADC_GPIO_Config(void);
void JOYSTICK_ADC_Mode_Config(void);
void JOYSTICK_Init(void);
void ADC_get_value(void);
void Go_with_joystick(void);
void TIM2_Init(void);

extern __IO uint16_t ADC_ConvertedValue[4];
extern float ADC_ConvertedValueLocal[4]; 
extern int correct_value[4];
extern TIM_HandleTypeDef TIM2_TimeBaseStructure_for_joystick;
#endif
