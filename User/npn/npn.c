#include "npn.h"
#include "stm32f4xx_it.h"
#include "./stepper/bsp_stepper_init.h"
#include "./led/bsp_led.h"   
#include "./usart/bsp_debug_usart.h"
#include "joystick.h"
#include "./lcd/bsp_nt35510_lcd.h"
#include "./delay/core_delay.h"
#include "platform_order.h"
/* �ӽ��������飬�����������ӽ����ص����� */
NPN_TypeDef npn_switch[12] = 
{
	//X����λ����
	{SWITCH_X_MIN_INT_GPIO_PORT, SWITCH_X_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_X_MIN_INT_EXTI_IRQ},
  {SWITCH_X_MAX_INT_GPIO_PORT, SWITCH_X_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_X_MAX_INT_EXTI_IRQ},
  {SWITCH_X_ZERO_INT_GPIO_PORT, SWITCH_X_ZERO_INT_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, SWITCH_X_ZERO_INT_EXTI_IRQ},
	//Y����λ����
    {SWITCH_Y_MIN_INT_GPIO_PORT, SWITCH_Y_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Y_MIN_INT_EXTI_IRQ},
  {SWITCH_Y_MAX_INT_GPIO_PORT, SWITCH_Y_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Y_MAX_INT_EXTI_IRQ},
  {SWITCH_Y_ZERO_INT_GPIO_PORT, SWITCH_Y_ZERO_INT_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, SWITCH_Y_ZERO_INT_EXTI_IRQ},
	//Z����λ����
    {SWITCH_Z_MIN_INT_GPIO_PORT, SWITCH_Z_MIN_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Z_MIN_INT_EXTI_IRQ},
	{SWITCH_Z_MAX_INT_GPIO_PORT, SWITCH_Z_MAX_INT_GPIO_PIN, GPIO_MODE_IT_FALLING, SWITCH_Z_MAX_INT_EXTI_IRQ},
	//PS2�ĸ�����
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
    /* ѡ��ӽ����ص����� */ 
    GPIO_InitStructure.Pin = npn_switch[i].pin;
    /* ��������Ϊ�½��ش���ģʽ */ 
    GPIO_InitStructure.Mode = npn_switch[i].gpio_mode;
    /* �������Ų�����Ҳ������ */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* ʹ������Ľṹ���ʼ������ */
    HAL_GPIO_Init(npn_switch[i].port, &GPIO_InitStructure);
    
    /* ���� EXTI �ж�Դ �����š������ж����ȼ�*/
    HAL_NVIC_SetPriority(npn_switch[i].IRQn, 0, 0);
    /* ʹ���ж� */
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
  * @brief  �ⲿ�ж�15_10������
	*	@note   �ж���15����10��9����5�ֱ���һ���жϷ�����
  *         ������ⲿ�ж�4����0����ÿ���ж��߶�Ӧ�������жϷ�����
  * @retval ��
  */
void EXTI0_IRQHandler(void)//�ж���0��PG0
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Z_MAX_INT_GPIO_PIN);
}

void EXTI1_IRQHandler(void)	//�ж���1,PG1
{
	HAL_GPIO_EXTI_IRQHandler(PS2_LEFT_PUSH_INT_GPIO_PIN);
}

void EXTI2_IRQHandler(void)	//�ж���2��PH2
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Y_MIN_INT_GPIO_PIN);
}

void EXTI3_IRQHandler(void)//�ж���3��PH3
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Y_MAX_INT_GPIO_PIN);
}

void EXTI9_5_IRQHandler(void) //�ж���5-9��PG5,PD7
{

	HAL_GPIO_EXTI_IRQHandler(PS2_SWB_PUSH_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(PS2_RIGHT_PUSH_INT_GPIO_PIN);
}

void EXTI15_10_IRQHandler(void)	//�ж���10-15��PF12,PF13,PF15,PD11
{
	HAL_GPIO_EXTI_IRQHandler(SWITCH_X_MIN_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(SWITCH_X_MAX_INT_GPIO_PIN);
	HAL_GPIO_EXTI_IRQHandler(SWITCH_Z_MIN_INT_GPIO_PIN);
	
	HAL_GPIO_EXTI_IRQHandler(PS2_SWA_PUSH_INT_GPIO_PIN);
}


/**
  * @brief  �ⲿ�жϻص�����
  * @param  GPIO_Pin�������ⲿ�жϵ�IO����
  * @retval ��
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
		  //printf("��ʼ��X��λ\r\n");	
			  break;
		  case SWITCH_X_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[X].dir_port, step_motor[X].dir_pin) == PULL)
			  {
			  MOTOR_EN(step_motor[X].en_port, step_motor[X].en_pin, OFF);
				platform_init_success++;
				step_motor[X].should_count_step_nums = 1000;
				  //printf("��ʼ��X���\r\n");	
			  }
				break;
			  
		  case SWITCH_Y_MAX_INT_GPIO_PIN:
			  MOTOR_DIR(step_motor[Y].dir_port, step_motor[Y].dir_pin, PULL);
		  //printf("��ʼ��Y��λ\r\n");	
			  break;
		  case SWITCH_Y_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[Y].dir_port, step_motor[Y].dir_pin) == PULL)
			  {
			  MOTOR_EN(step_motor[Y].en_port, step_motor[Y].en_pin, OFF);
				platform_init_success++;
				  //printf("��ʼ��Y���\r\n");	
				  step_motor[Y].should_count_step_nums = 1000;
			  }
				break;
			  
		  case SWITCH_Z_MAX_INT_GPIO_PIN:
			  MOTOR_DIR(step_motor[Z].dir_port, step_motor[Z].dir_pin, PULL);
				//printf("��ʼ��Z��λ\r\n");	
			  break;
		  case SWITCH_Z_MIN_INT_GPIO_PIN:
			  if(HAL_GPIO_ReadPin(step_motor[Z].dir_port, step_motor[Z].dir_pin) == PULL)
			  {
				MOTOR_EN(step_motor[Z].en_port, step_motor[Z].en_pin, OFF);
				platform_init_success++;
				  //printf("��ʼ��Z���\r\n");	
				  step_motor[Z].should_count_step_nums = 1000;
			  }
				break;
		  default:break;
		}
	  
	}else 
	switch(GPIO_Pin)
	{
	  
	  //X������λ
	  case SWITCH_X_MIN_INT_GPIO_PIN:
		step_motor[X].real_count_step_nums = 0;
	  	step_motor[X].should_count_step_nums = safe_distance;
		//printf("X���\r\n");

		break;
	  case SWITCH_X_MAX_INT_GPIO_PIN:
			step_motor[X].should_count_step_nums = step_motor[X].real_count_step_nums - safe_distance;
			//printf("X��λ\r\n");

		  break;
	  
	  //Y������λ
	  case SWITCH_Y_MAX_INT_GPIO_PIN:
		step_motor[Y].should_count_step_nums = step_motor[Y].real_count_step_nums - safe_distance;
		//printf("Y��λ\r\n");

	  break;
	  case SWITCH_Y_MIN_INT_GPIO_PIN:
		step_motor[Y].real_count_step_nums = 0;
		step_motor[Y].should_count_step_nums = safe_distance;
		//printf("Y���\r\n");

			break;
	  
		//Z������λ
		case SWITCH_Z_MIN_INT_GPIO_PIN:
			step_motor[Z].real_count_step_nums = 0;
			step_motor[Z].should_count_step_nums = safe_distance;
			//printf("Z���\r\n");	

			break;
		case SWITCH_Z_MAX_INT_GPIO_PIN:
			step_motor[Z].should_count_step_nums = step_motor[Z].real_count_step_nums - safe_distance;
			//printf("Z��λ\r\n");

		break;
		
		//PS2�ĸ�����
		case PS2_RIGHT_PUSH_INT_GPIO_PIN:
			__HAL_TIM_DISABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
			control_state = auto_control;
			printf("��ҡ�˰��£��л���Ԥ�����\r\n");
			LCD_clear(&row);
		break;
		case PS2_SWA_PUSH_INT_GPIO_PIN:
			key_state --;
			if(key_state == -1) key_state =4;
			printf("SWA����\r\n");
		break;
		case PS2_SWB_PUSH_INT_GPIO_PIN:
			key_state ++;
			if(key_state == 5) key_state =0;
			printf("SWB����\r\n");
		break;
		case PS2_LEFT_PUSH_INT_GPIO_PIN:
			__HAL_TIM_ENABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
			control_state = manual_control;
			printf("��ҡ�˰��£��л����ֶ�����\r\n");
			LCD_clear(&row);
		break;
		
		default:break;
	}
}
