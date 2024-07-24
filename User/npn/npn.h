#ifndef __NPN_H
#define	__NPN_H

#include "stm32f4xx_hal.h"

/* �ӽ����ؽṹ�� */
typedef struct{
  GPIO_TypeDef *port;            //�˿ں�
  uint16_t pin;                  //�������
  uint32_t gpio_mode;            //����ģʽ
  IRQn_Type IRQn;                //�ж�Դ
}NPN_TypeDef;


enum{
	all_ok = 0,
	only_X = 1,
	only_Y,
	only_Z,
	only_pump,

};
extern uint8_t control_state;
enum{
	manual_control = 0,
	auto_control,
};


//���Ŷ���
/*******************************************************/
//Y�ᣬ��С��λPH2
#define SWITCH_Y_MIN_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_MIN_INT_GPIO_PIN                 GPIO_PIN_2
#define SWITCH_Y_MIN_INT_EXTI_IRQ                 EXTI2_IRQn
#define SWITCH_Y_MIN_IRQHandler                   EXTI2_IRQHandler

//Y�ᣬ�����λPH3
#define SWITCH_Y_MAX_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_MAX_INT_GPIO_PIN                 GPIO_PIN_3
#define SWITCH_Y_MAX_INT_EXTI_IRQ                 EXTI3_IRQn
#define SWITCH_Y_MAX_IRQHandler                   EXTI3_IRQHandler

//Y�ᣬ0�㣬PH8
#define SWITCH_Y_ZERO_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_ZERO_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_ZERO_INT_GPIO_PIN                 GPIO_PIN_8
#define SWITCH_Y_ZERO_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define SWITCH_Y_ZERO_IRQHandler                   EXTI9_5_IRQHandler


//X�ᣬ��С��λPF12
#define SWITCH_X_MIN_INT_GPIO_PORT                GPIOF
#define SWITCH_X_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_MIN_INT_GPIO_PIN                 GPIO_PIN_12
#define SWITCH_X_MIN_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_MIN_IRQHandler                   EXTI15_10_IRQHandler

//X�ᣬ�����λPF13
#define SWITCH_X_MAX_INT_GPIO_PORT                GPIOF
#define SWITCH_X_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_MAX_INT_GPIO_PIN                 GPIO_PIN_13
#define SWITCH_X_MAX_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_MAX_IRQHandler                   EXTI15_10_IRQHandler

//X�ᣬ0�㣬PF14
#define SWITCH_X_ZERO_INT_GPIO_PORT                GPIOF
#define SWITCH_X_ZERO_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_ZERO_INT_GPIO_PIN                 GPIO_PIN_14
#define SWITCH_X_ZERO_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_ZERO_IRQHandler                   EXTI15_10_IRQHandler


//Z�ᣬ��С��λPF15
#define SWITCH_Z_MIN_INT_GPIO_PORT                GPIOF
#define SWITCH_Z_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_Z_MIN_INT_GPIO_PIN                 GPIO_PIN_15
#define SWITCH_Z_MIN_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_Z_MIN_IRQHandler                   EXTI15_10_IRQHandler

//Z�ᣬ�����λPG0
#define SWITCH_Z_MAX_INT_GPIO_PORT                GPIOG
#define SWITCH_Z_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define SWITCH_Z_MAX_INT_GPIO_PIN                 GPIO_PIN_0
#define SWITCH_Z_MAX_INT_EXTI_IRQ                 EXTI0_IRQn
#define SWITCH_Z_MAX_IRQHandler                   EXTI0_IRQHandler


/***********************************************************************/
//��ҡ�˰���PG1
#define PS2_LEFT_PUSH_INT_GPIO_PORT                GPIOG
#define PS2_LEFT_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define PS2_LEFT_PUSH_INT_GPIO_PIN                 GPIO_PIN_1
#define PS2_LEFT_PUSH_INT_EXTI_IRQ                 EXTI1_IRQn
#define PS2_LEFT_PUSH_IRQHandler                   EXTI1_IRQHandler

//ҡ��SWA����PD11
#define PS2_SWA_PUSH_INT_GPIO_PORT                GPIOD
#define PS2_SWA_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define PS2_SWA_PUSH_INT_GPIO_PIN                 GPIO_PIN_11
#define PS2_SWA_PUSH_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define PS2_SWA_PUSH_IRQHandler                   EXTI15_10_IRQHandler

//ҡ��SWB����PG5
#define PS2_SWB_PUSH_INT_GPIO_PORT                GPIOG
#define PS2_SWB_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define PS2_SWB_PUSH_INT_GPIO_PIN                 GPIO_PIN_5
#define PS2_SWB_PUSH_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define PS2_SWB_PUSH_IRQHandler                   EXTI9_5_IRQHandler

//��ҡ�˰���PD7
#define PS2_RIGHT_PUSH_INT_GPIO_PORT                GPIOD
#define PS2_RIGHT_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define PS2_RIGHT_PUSH_INT_GPIO_PIN                 GPIO_PIN_7
#define PS2_RIGHT_PUSH_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define PS2_RIGHT_PUSH_IRQHandler                   EXTI9_5_IRQHandler

#define safe_distance 200	//���Ӵ�����λ����ʱ�����ϻص�100������֤�´��ܽӴ����������߹�

void NPN_Init(void);

extern __IO uint8_t platform_init_success;
void Platform_Init(void);

extern NPN_TypeDef npn_switch[12];
extern int key_state;

#endif
