#include "npn.h"
#include "stm32f4xx_it.h"
#include "./stepper/bsp_stepper_init.h"
#include "./led/bsp_led.h"   
#include "./usart/bsp_debug_usart.h"
#include "joystick.h"
#include "./lcd/bsp_nt35510_lcd.h"
#include "./delay/core_delay.h"
#include "platform_order.h"
/* 接近开关数组，可用来增减接近开关的数量 */
NPN_TypeDef npn_switch[12] = 
{
	//X轴限位开关
	{SWITCH_X_MIN_INT_GPIO_PORT, SWITCH_X_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_X_MIN_INT_EXTI_IRQ},
  {SWITCH_X_MAX_INT_GPIO_PORT, SWITCH_X_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_X_MAX_INT_EXTI_IRQ},
  {SWITCH_X_ZERO_INT_GPIO_PORT, SWITCH_X_ZERO_INT_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, SWITCH_X_ZERO_INT_EXTI_IRQ},
	//Y轴限位开关
    {SWITCH_Y_MIN_INT_GPIO_PORT, SWITCH_Y_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Y_MIN_INT_EXTI_IRQ},
  {SWITCH_Y_MAX_INT_GPIO_PORT, SWITCH_Y_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Y_MAX_INT_EXTI_IRQ},
  {SWITCH_Y_ZERO_INT_GPIO_PORT, SWITCH_Y_ZERO_INT_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, SWITCH_Y_ZERO_INT_EXTI_IRQ},
	//Z轴限位开关
    {SWITCH_Z_MIN_INT_GPIO_PORT, SWITCH_Z_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Z_MIN_INT_EXTI_IRQ},
	{SWITCH_Z_MAX_INT_GPIO_PORT, SWITCH_Z_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Z_MAX_INT_EXTI_IRQ},
	//PS2四个按键
    {PS2_LEFT_PUSH_INT_GPIO_PORT, PS2_LEFT_PUSH_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, PS2_LEFT_PUSH_INT_EXTI_IRQ},
	{PS2_SWA_PUSH_INT_GPIO_PORT, PS2_SWA_PUSH_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, PS2_SWA_PUSH_INT_EXTI_IRQ},
	{PS2_SWB_PUSH_INT_GPIO_PORT, PS2_SWB_PUSH_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, PS2_SWB_PUSH_INT_EXTI_IRQ},
	{PS2_RIGHT_PUSH_INT_GPIO_PORT, PS2_RIGHT_PUSH_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, PS2_RIGHT_PUSH_INT_EXTI_IRQ},

};




void NPN_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	
  SWITCH_X_MIN_INT_GPIO_CLK_ENABLE();
	SWITCH_X_MAX_INT_GPIO_CLK_ENABLE();
	SWITCH_X_ZERO_INT_GPIO_CLK_ENABLE(); 
	
  SWITCH_Y_MIN_INT_GPIO_CLK_ENABLE();
	SWITCH_Y_MAX_INT_GPIO_CLK_ENABLE();
	SWITCH_Y_ZERO_INT_GPIO_CLK_ENABLE();
	
	SWITCH_Z_MIN_INT_GPIO_CLK_ENABLE();
	SWITCH_Z_MAX_INT_GPIO_CLK_ENABLE(); 
	
	PS2_LEFT_PUSH_INT_GPIO_CLK_ENABLE();
	PS2_SWA_PUSH_INT_GPIO_CLK_ENABLE();
	PS2_SWB_PUSH_INT_GPIO_CLK_ENABLE();
	PS2_RIGHT_PUSH_INT_GPIO_CLK_ENABLE();
	
	

  uint8_t size = sizeof(npn_switch)/sizeof(NPN_TypeDef);

	
  for(uint8_t i = 0; i < size; i++)
  {
    /* 选择接近开关的引脚 */ 
    GPIO_InitStructure.Pin = npn_switch[i].pin;
    /* 设置引脚为下降沿触发模式 */ 
    GPIO_InitStructure.Mode = npn_switch[i].gpio_mode;
    /* 设置引脚不上拉也不下拉 */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* 使用上面的结构体初始化引脚 */
    HAL_GPIO_Init(npn_switch[i].port, &GPIO_InitStructure);
    
    /* 配置 EXTI 中断源 到引脚、配置中断优先级*/
    HAL_NVIC_SetPriority(npn_switch[i].IRQn, 0, 0);
    /* 使能中断 */
    HAL_NVIC_EnableIRQ(npn_switch[i].IRQn);
  }
}

__IO uint8_t platform_init_success = 0;
void Platform_Init(void)
{
	if(HAL_GPIO_ReadPin(SWITCH_Z_MIN_INT_GPIO_PORT, SWITCH_Z_MIN_INT_GPIO_PIN) == GPIO_PIN_RESET)
	MOTOR_DIR(step_motor[Z].dir_port, step_motor[Z].dir_pin, PUSH);

	if(HAL_GPIO_ReadPin(SWITCH_X_MIN_INT_GPIO_PORT, SWITCH_X_MIN_INT_GPIO_PIN) == GPIO_PIN_RESET)
		MOTOR_DIR(step_motor[X].dir_port, step_motor[X].dir_pin, PUSH);
	
	if(HAL_GPIO_ReadPin(SWITCH_Y_MIN_INT_GPIO_PORT, SWITCH_Y_MIN_INT_GPIO_PIN) == GPIO_PIN_RESET)
		MOTOR_DIR(step_motor[Y].dir_port, step_motor[Y].dir_pin, PUSH);
	
//	if(HAL_GPIO_ReadPin(SWITCH_Z_MAX_INT_GPIO_PORT, SWITCH_Z_MAX_INT_GPIO_PIN) == GPIO_PIN_RESET)
//		MOTOR_DIR(step_motor[Z].dir_port, step_motor[Z].dir_pin, PULL);

//	if(HAL_GPIO_ReadPin(SWITCH_X_MAX_INT_GPIO_PORT, SWITCH_X_MAX_INT_GPIO_PIN) == GPIO_PIN_RESET)
//		MOTOR_DIR(step_motor[X].dir_port, step_motor[X].dir_pin, PULL);
//	
//	if(HAL_GPIO_ReadPin(SWITCH_Y_MAX_INT_GPIO_PORT, SWITCH_Y_MAX_INT_GPIO_PIN) == GPIO_PIN_RESET)
//		MOTOR_DIR(step_motor[Y].dir_port, step_motor[Y].dir_pin, PULL);
	
	MOTOR_EN(step_motor[Z].en_port, step_motor[Z].en_pin, ON);
	while(HAL_GPIO_ReadPin(step_motor[Z].en_port, step_motor[Z].en_pin) == ON);
	MOTOR_EN(step_motor[X].en_port, step_motor[X].en_pin, ON);
	MOTOR_EN(step_motor[Y].en_port, step_motor[Y].en_pin, ON);
	

}



/**
  * @brief  外部中断15_10服务函数
	*	@note   中断线15——10、9——5分别共用一个中断服务函数
  *         如果是外部中断4——0必须每个中断线对应独立的中断服务函数
  * @retval 无
  */
void EXTI0_IRQHandler(void)//中短线0，PG0
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Z_MAX_INT_GPIO_PIN);
}

void EXTI1_IRQHandler(void)	//中断线1,PG1
{
	HAL_GPIO_EXTI_IRQHandler(PS2_LEFT_PUSH_INT_GPIO_PIN);
}

void EXTI2_IRQHandler(void)	//中断线2，PH2
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Y_MIN_INT_GPIO_PIN);
}

void EXTI3_IRQHandler(void)//中断线3，PH3
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Y_MAX_INT_GPIO_PIN);
}

void EXTI9_5_IRQHandler(void) //中断线5-9，PG5,PD7
{

	HAL_GPIO_EXTI_IRQHandler(PS2_SWB_PUSH_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(PS2_RIGHT_PUSH_INT_GPIO_PIN);
}

void EXTI15_10_IRQHandler(void)	//中断线10-15，PF12,PF13,PF15,PD11
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_X_MIN_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(SWITCH_X_MAX_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Z_MIN_INT_GPIO_PIN);
	
	HAL_GPIO_EXTI_IRQHandler(PS2_SWA_PUSH_INT_GPIO_PIN);
}


/**
  * @brief  外部中断回调函数
  * @param  GPIO_Pin：触发外部中断的IO引脚
  * @retval 无
  */
int key_state = 0; 
uint8_t control_state;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(platform_init_success < 3)
	{
		switch(GPIO_Pin)
		{
		  case SWITCH_X_MAX_INT_GPIO_PIN:
			  MOTOR_DIR(step_motor[X].dir_port, step_motor[X].dir_pin, PULL);
		  //printf("初始化X限位\r\n");	
			  break;
		  case SWITCH_X_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[X].dir_port, step_motor[X].dir_pin) == PULL)
			  {
			  MOTOR_EN(step_motor[X].en_port, step_motor[X].en_pin, OFF);
				platform_init_success++;
				step_motor[X].should_count_step_nums = 1000;
				  //printf("初始化X零点\r\n");	
			  }
				break;
			  
		  case SWITCH_Y_MAX_INT_GPIO_PIN:
			  MOTOR_DIR(step_motor[Y].dir_port, step_motor[Y].dir_pin, PULL);
		  //printf("初始化Y限位\r\n");	
			  break;
		  case SWITCH_Y_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[Y].dir_port, step_motor[Y].dir_pin) == PULL)
			  {
			  MOTOR_EN(step_motor[Y].en_port, step_motor[Y].en_pin, OFF);
				platform_init_success++;
				  //printf("初始化Y零点\r\n");	
				  step_motor[Y].should_count_step_nums = 1000;
			  }
				break;
			  
		  case SWITCH_Z_MAX_INT_GPIO_PIN:
			  MOTOR_DIR(step_motor[Z].dir_port, step_motor[Z].dir_pin, PULL);
				//printf("初始化Z限位\r\n");	
			  break;
		  case SWITCH_Z_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[Z].dir_port, step_motor[Z].dir_pin) == PULL)
			  {
				MOTOR_EN(step_motor[Z].en_port, step_motor[Z].en_pin, OFF);
				platform_init_success++;
				  //printf("初始化Z零点\r\n");	
				  step_motor[Z].should_count_step_nums = 1000;
			  }
				break;
		  default:break;
		}
	  
	}else 
	switch(GPIO_Pin)
	{
	  
	  //X轴电机限位
	  case SWITCH_X_MIN_INT_GPIO_PIN:
		step_motor[X].real_count_step_nums = 0;
	  	step_motor[X].should_count_step_nums = safe_distance;
		//printf("X零点\r\n");

		break;
	  case SWITCH_X_MAX_INT_GPIO_PIN:
			step_motor[X].should_count_step_nums = step_motor[X].real_count_step_nums - safe_distance;
			//printf("X限位\r\n");

		  break;
	  
	  //Y轴电机限位
	  case SWITCH_Y_MAX_INT_GPIO_PIN:
		step_motor[Y].should_count_step_nums = step_motor[Y].real_count_step_nums - safe_distance;
		//printf("Y限位\r\n");

	  break;
	  case SWITCH_Y_MIN_INT_GPIO_PIN:
		step_motor[Y].real_count_step_nums = 0;
		step_motor[Y].should_count_step_nums = safe_distance;
		//printf("Y零点\r\n");

			break;
	  
		//Z轴电机限位
		case SWITCH_Z_MIN_INT_GPIO_PIN:
			step_motor[Z].real_count_step_nums = 0;
			step_motor[Z].should_count_step_nums = safe_distance;
			//printf("Z零点\r\n");	

			break;
		case SWITCH_Z_MAX_INT_GPIO_PIN:
			step_motor[Z].should_count_step_nums = step_motor[Z].real_count_step_nums - safe_distance;
			//printf("Z限位\r\n");

		break;
		
		//PS2四个按键
		case PS2_RIGHT_PUSH_INT_GPIO_PIN:
			__HAL_TIM_DISABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
			control_state = auto_control;
			printf("右摇杆按下，切换至预设控制\r\n");
			LCD_clear(&row);
		break;
		case PS2_SWA_PUSH_INT_GPIO_PIN:
			key_state --;
			if(key_state == -1) key_state =4;
			printf("SWA按下\r\n");
		break;
		case PS2_SWB_PUSH_INT_GPIO_PIN:
			key_state ++;
			if(key_state == 5) key_state =0;
			printf("SWB按下\r\n");
		break;
		case PS2_LEFT_PUSH_INT_GPIO_PIN:
			__HAL_TIM_ENABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
			control_state = manual_control;
			printf("左摇杆按下，切换至手动控制\r\n");
			LCD_clear(&row);
		break;
		
		default:break;
	}
}
