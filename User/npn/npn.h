#ifndef __NPN_H
#define	__NPN_H

#include "stm32f4xx_hal.h"

/* 接近开关结构体 */
typedef struct{
  GPIO_TypeDef *port;            //端口号
  uint16_t pin;                  //引脚序号
  uint32_t gpio_mode;            //触发模式
  IRQn_Type IRQn;                //中断源
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


//引脚定义
/*******************************************************/
//Y轴，最小限位PH2
#define SWITCH_Y_MIN_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_MIN_INT_GPIO_PIN                 GPIO_PIN_2
#define SWITCH_Y_MIN_INT_EXTI_IRQ                 EXTI2_IRQn
#define SWITCH_Y_MIN_IRQHandler                   EXTI2_IRQHandler

//Y轴，最大限位PH3
#define SWITCH_Y_MAX_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_MAX_INT_GPIO_PIN                 GPIO_PIN_3
#define SWITCH_Y_MAX_INT_EXTI_IRQ                 EXTI3_IRQn
#define SWITCH_Y_MAX_IRQHandler                   EXTI3_IRQHandler

//Y轴，0点，PH8
#define SWITCH_Y_ZERO_INT_GPIO_PORT                GPIOH
#define SWITCH_Y_ZERO_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define SWITCH_Y_ZERO_INT_GPIO_PIN                 GPIO_PIN_8
#define SWITCH_Y_ZERO_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define SWITCH_Y_ZERO_IRQHandler                   EXTI9_5_IRQHandler


//X轴，最小限位PF12
#define SWITCH_X_MIN_INT_GPIO_PORT                GPIOF
#define SWITCH_X_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_MIN_INT_GPIO_PIN                 GPIO_PIN_12
#define SWITCH_X_MIN_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_MIN_IRQHandler                   EXTI15_10_IRQHandler

//X轴，最大限位PF13
#define SWITCH_X_MAX_INT_GPIO_PORT                GPIOF
#define SWITCH_X_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_MAX_INT_GPIO_PIN                 GPIO_PIN_13
#define SWITCH_X_MAX_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_MAX_IRQHandler                   EXTI15_10_IRQHandler

//X轴，0点，PF14
#define SWITCH_X_ZERO_INT_GPIO_PORT                GPIOF
#define SWITCH_X_ZERO_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_X_ZERO_INT_GPIO_PIN                 GPIO_PIN_14
#define SWITCH_X_ZERO_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_X_ZERO_IRQHandler                   EXTI15_10_IRQHandler


//Z轴，最小限位PF15
#define SWITCH_Z_MIN_INT_GPIO_PORT                GPIOF
#define SWITCH_Z_MIN_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define SWITCH_Z_MIN_INT_GPIO_PIN                 GPIO_PIN_15
#define SWITCH_Z_MIN_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define SWITCH_Z_MIN_IRQHandler                   EXTI15_10_IRQHandler

//Z轴，最大限位PG0
#define SWITCH_Z_MAX_INT_GPIO_PORT                GPIOG
#define SWITCH_Z_MAX_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define SWITCH_Z_MAX_INT_GPIO_PIN                 GPIO_PIN_0
#define SWITCH_Z_MAX_INT_EXTI_IRQ                 EXTI0_IRQn
#define SWITCH_Z_MAX_IRQHandler                   EXTI0_IRQHandler


/***********************************************************************/
//左摇杆按下PG1
#define PS2_LEFT_PUSH_INT_GPIO_PORT                GPIOG
#define PS2_LEFT_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define PS2_LEFT_PUSH_INT_GPIO_PIN                 GPIO_PIN_1
#define PS2_LEFT_PUSH_INT_EXTI_IRQ                 EXTI1_IRQn
#define PS2_LEFT_PUSH_IRQHandler                   EXTI1_IRQHandler

//摇杆SWA按下PD11
#define PS2_SWA_PUSH_INT_GPIO_PORT                GPIOD
#define PS2_SWA_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define PS2_SWA_PUSH_INT_GPIO_PIN                 GPIO_PIN_11
#define PS2_SWA_PUSH_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define PS2_SWA_PUSH_IRQHandler                   EXTI15_10_IRQHandler

//摇杆SWB按下PG5
#define PS2_SWB_PUSH_INT_GPIO_PORT                GPIOG
#define PS2_SWB_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define PS2_SWB_PUSH_INT_GPIO_PIN                 GPIO_PIN_5
#define PS2_SWB_PUSH_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define PS2_SWB_PUSH_IRQHandler                   EXTI9_5_IRQHandler

//右摇杆按下PD7
#define PS2_RIGHT_PUSH_INT_GPIO_PORT                GPIOD
#define PS2_RIGHT_PUSH_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define PS2_RIGHT_PUSH_INT_GPIO_PIN                 GPIO_PIN_7
#define PS2_RIGHT_PUSH_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define PS2_RIGHT_PUSH_IRQHandler                   EXTI9_5_IRQHandler

#define safe_distance 200	//当接触到限位开关时，马上回弹100步，保证下次能接触到，不回走过

void NPN_Init(void);

extern __IO uint8_t platform_init_success;
void Platform_Init(void);

extern NPN_TypeDef npn_switch[12];
extern int key_state;

#endif
