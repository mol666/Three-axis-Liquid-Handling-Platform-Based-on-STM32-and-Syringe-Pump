#include "./stepper/bsp_stepper_init.h"
#include "./led/bsp_led.h"   
#include "npn.h"
#include "./usart/bsp_debug_usart.h"
#include "joystick.h"
#include "./lcd/bsp_nt35510_lcd.h"
#include "./delay/core_delay.h"
#include "platform_order.h"

/* ����������� */
Stepper_TypeDef step_motor[4] = //������������������������������ʹ��������
								//��ʱ��ͨ��
{								//���������顢���������顢ʹ��������
								//������ϸ����������
								//PWM���ڣ���λus����Ӧ��������ʵ��������
								//ָ��λ�ã�ʵ��λ��
  {MOTOR_PUL_pump_PIN, MOTOR_DIR_pump_PIN, MOTOR_EN_pump_PIN, 
  MOTOR_PUL_pump_CHANNEL, 
  MOTOR_PUL_pump_PORT, MOTOR_DIR_pump_GPIO_PORT, MOTOR_EN_pump_GPIO_PORT,
	  pulse_every_round_for_pump, 1,
  TIM_PERIOD_for_pump, 0, 0,
  0, 0},
  
  {MOTOR_PUL_X_PIN, MOTOR_DIR_X_PIN, MOTOR_EN_X_PIN,
  MOTOR_PUL_X_CHANNEL,
  MOTOR_PUL_X_PORT, MOTOR_DIR_X_GPIO_PORT, MOTOR_EN_X_GPIO_PORT,
	  pulse_every_round_for_X, 54.37f,
  TIM_PERIOD_for_X, 0, 0,
  0, 0},
  
  {MOTOR_PUL_Y_PIN, MOTOR_DIR_Y_PIN, MOTOR_EN_Y_PIN,
  MOTOR_PUL_Y_CHANNEL, 
  MOTOR_PUL_Y_PORT, MOTOR_DIR_Y_GPIO_PORT, MOTOR_EN_Y_GPIO_PORT,
	  pulse_every_round_for_Y, 54.43f,
  TIM_PERIOD_for_Y, 0, 0,
  0, 0},
  
  {MOTOR_PUL_Z_PIN, MOTOR_DIR_Z_PIN, MOTOR_EN_Z_PIN,
  MOTOR_PUL_Z_CHANNEL, 
  MOTOR_PUL_Z_PORT, MOTOR_DIR_Z_GPIO_PORT, MOTOR_EN_Z_GPIO_PORT,
	  pulse_every_round_for_Z, 54,
  TIM_PERIOD_for_Z, 0, 0,
  0 ,0},
};



Syringe_TypeDef syringe[1] = 
{
	{1000, 57},	//1mlע��������1000uL���̶��ܳ�57mm
};

  /* ��ȡ����Ԫ�ظ���(�м�������� */
uint8_t member_count_of_motor = sizeof(step_motor)/sizeof(Stepper_TypeDef);

/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */

void Step_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	MOTOR_DIR_pump_GPIO_CLK_ENABLE();
	MOTOR_EN_pump_GPIO_CLK_ENABLE();
	MOTOR_PUL_pump_GPIO_CLK_ENABLE();
	
	MOTOR_DIR_X_GPIO_CLK_ENABLE();
	MOTOR_EN_X_GPIO_CLK_ENABLE();    
	MOTOR_PUL_X_GPIO_CLK_ENABLE();

	MOTOR_DIR_Y_GPIO_CLK_ENABLE();   	  				   
	MOTOR_EN_Y_GPIO_CLK_ENABLE();    	  
	MOTOR_PUL_Y_GPIO_CLK_ENABLE();	

	MOTOR_DIR_Z_GPIO_CLK_ENABLE();
	MOTOR_EN_Z_GPIO_CLK_ENABLE();
	MOTOR_PUL_Z_GPIO_CLK_ENABLE();
	
  

	
  for(uint8_t i = 0; i < member_count_of_motor; i++)
  {  
	 /*ѡ��Ҫ���Ƶ�GPIO����*/    														   
	GPIO_InitStruct.Pin = step_motor[i].dir_pin;
	/*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Pull =GPIO_PULLUP;
	/*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	/*Motor �������� ��ʼ��*/   	
	HAL_GPIO_Init(step_motor[i].dir_port, &GPIO_InitStruct); 
	  
	if(i == pump)
		MOTOR_DIR(step_motor[i].dir_port, step_motor[i].dir_pin, PULL);//��ʼ������		  		  
	else
		MOTOR_DIR(step_motor[i].dir_port, step_motor[i].dir_pin, PULL);//��ʼ������

    
    /*Motor ʹ������ ��ʼ��*/
    GPIO_InitStruct.Pin = step_motor[i].en_pin;
    HAL_GPIO_Init(step_motor[i].en_port, &GPIO_InitStruct);
    MOTOR_EN(step_motor[i].en_port, step_motor[i].en_pin, OFF);//��ʼĬ�Ϲر�ʹ��
    
    /* ��ʱ�����ͨ����������IO��ʼ�� */
    /*�����������*/
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    /*������������ */ 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    /*���ø���*/
    GPIO_InitStruct.Alternate = MOTOR_PUL_GPIO_AF;
    /*���ø���*/
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    /*ѡ��Ҫ���Ƶ�GPIO����*/	
    GPIO_InitStruct.Pin = step_motor[i].pul_pin;
    /*Motor �������� ��ʼ��*/
    HAL_GPIO_Init(step_motor[i].pul_port, &GPIO_InitStruct);
  }
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */

	TIM_OC_InitTypeDef  TIM_OCInitStructure;  
	TIM_HandleTypeDef  TIM_TimeBaseStructure; 
void TIM8_PWMOUTPUT_Config(void)
{

  /*ʹ�ܶ�ʱ��*/
  MOTOR_PUL_CLK_ENABLE();

  TIM_TimeBaseStructure.Instance = MOTOR_PUL_TIM;
  TIM_TimeBaseStructure.Init.Period = TIM_PERIOD_for_INIT-1;
  TIM_TimeBaseStructure.Init.Prescaler = TIM_PRESCALER-1; 
  TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM_TimeBaseStructure.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
	
//	__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_UPDATE);
//	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);
//	__HAL_TIM_DISABLE_IT(&TIM_TimeBaseStructure,TIM_IT_UPDATE);
//	
//		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 2, 2);
//		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
	
	__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC1);
	__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC2);
	__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC3);
	__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC4);
	for(uint8_t i = 0; i < member_count_of_motor; i++)
	{	
		
		HAL_TIM_PWM_Start_IT(&TIM_TimeBaseStructure, step_motor[i].pul_channel);
		switch(i)
		{
			case 0:
				__HAL_TIM_DISABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC1);
			break;
			case 1:
				__HAL_TIM_DISABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC2);
			break;
			case 2:
				__HAL_TIM_DISABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC3);
			break;
			case 3:
				__HAL_TIM_DISABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC4);
			break;
			default:break;	
		}
	}
		
	HAL_NVIC_SetPriority(TIM8_CC_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
	
  /*PWMģʽ����*/
  TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;//����ΪPWMģʽ1 
  TIM_OCInitStructure.Pulse = TIM_PERIOD_for_pump/2;//Ĭ��ռ�ձ�Ϊ50%
  TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
  /*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH; 

  /* ��ȡ����Ԫ�ظ���(�м�������� */

  for(uint8_t i = 0; i < member_count_of_motor; i++)
  {
	/*����PWMͨ��*/
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure, &TIM_OCInitStructure, step_motor[i].pul_channel);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure, step_motor[i].pul_channel);
  }

}

void Step_motor_Init(void)
{
	Step_GPIO_Config();//����ֱ�����г�ʼ��ʱ�ӡ����š��жϵȣ�TIM8û���õ�Msp_Init����
	
	TIM8_PWMOUTPUT_Config();	
}


void Four_Step_motor_position_set(Stepper_TypeDef motor_control[], float posX, float posY, float posZ)
{
	
	motor_control[X].should_count_step_nums = posX /motor_control[X].dao_cheng * motor_control[X].pulse_every_round;
	motor_control[Y].should_count_step_nums = posY /motor_control[Y].dao_cheng * motor_control[Y].pulse_every_round;
	motor_control[Z].should_count_step_nums = posZ /motor_control[Z].dao_cheng * motor_control[Z].pulse_every_round;
	
	if(motor_control[X].real_count_step_nums  < motor_control[X].should_count_step_nums) motor_control[X].real_count_step_nums -= 81;
	else if(motor_control[X].real_count_step_nums  > motor_control[X].should_count_step_nums) motor_control[X].real_count_step_nums += 81;
	
	if(motor_control[Y].real_count_step_nums +200 < motor_control[Y].should_count_step_nums) motor_control[Y].real_count_step_nums -= 84;
	else if(motor_control[Y].real_count_step_nums -200 > motor_control[Y].should_count_step_nums) motor_control[Y].real_count_step_nums += 84;
	
	
}



//�˺��������ע���,��������룬������Ƴ�,��λuL
void Step_of_pump_run_set_uL(Syringe_TypeDef syringe, int volume)
{
	float distance = 0;
	distance = volume * syringe.length / syringe.total_volume;
	Step_run_set_distance(&step_motor[pump], distance);
}

//�˶�������ƽӿڣ����������������������������
void Step_run_set_distance(Stepper_TypeDef* motor_control, float distance)
{
	float angle = 0;
	angle = distance / motor_control->dao_cheng * 360.0f ;
	Step_run_set_angle(motor_control, angle);
}


//ת���Ƕȿ��ƽӿ�

void Step_run_set_angle(Stepper_TypeDef* motor_control, float angle)
{
	motor_control->should_count_step_nums += (int)(angle/360.0f * motor_control->pulse_every_round);//�ѽǶȻ���ɲ���
}

//int period_num=0;
//��ʱ�Ե���˶�������м�⣬����Ҫ�˶���������Ӧ���
//���Զ�ģʽ�£�4���������ͬʱ�˶���XY�������ȣ�Z����Σ�ע������
void Motion_scan(Stepper_TypeDef motor_control[])
{
	uint8_t i = 0;
	for(i = 0;i < member_count_of_motor;i++)
	{
		if(motor_control[i].real_count_step_nums != motor_control[i].should_count_step_nums)//���˶�������
		{
			//������
			if(motor_control[i].real_count_step_nums > motor_control[i].should_count_step_nums)
				MOTOR_DIR(motor_control[i].dir_port, motor_control[i].dir_pin, PULL);
			else if(motor_control[i].real_count_step_nums < motor_control[i].should_count_step_nums)
				MOTOR_DIR(motor_control[i].dir_port, motor_control[i].dir_pin, PUSH);
			
			
			if(platform_init_success >= 3)
				__HAL_TIM_SET_AUTORELOAD(&TIM_TimeBaseStructure, step_motor[i].period - 1);
			//���û����������
			if(HAL_GPIO_ReadPin(motor_control[i].en_port, motor_control[i].en_pin) != ON)
			{
				MOTOR_EN(motor_control[i].en_port, motor_control[i].en_pin, ON);	//����
				switch(i)
				{
					case pump:
						/*��������*/
						__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC1);

						__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC1);
						LED1_ON;		//��ָʾ��
						break;
					case X:
						__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC2);

						__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC2);
						LED2_ON;
						break;
					case Y:
						__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC3);
						__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC3);
						LED3_ON;
						break;
					case Z:
						__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_CC4);
						__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure,TIM_IT_CC4);
						LED4_ON;
						break;
					default:break;
				}
				
//				__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure,TIM_IT_UPDATE);
//				switch(i)
//				{
//					case pump:
//						LED1_ON;		//��ָʾ��
//						break;
//					case X:
//						LED2_ON;
//						break;
//					case Y:
//						LED3_ON;
//						break;
//					case Z:
//						LED4_ON;
//						break;
//					default:break;
//				}

			}
		}
	}	
	
}




void TIM8_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);
}

int	mark=0;	//��¼�жϴ���
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	mark++;
	if(htim->Instance == TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if(step_motor[pump].real_count_step_nums == step_motor[pump].should_count_step_nums)
			{
				MOTOR_EN(step_motor[pump].en_port, step_motor[pump].en_pin, OFF);
				__HAL_TIM_DISABLE_IT(htim,TIM_IT_CC1);
				LED1_OFF;
			}else if(step_motor[pump].real_count_step_nums < step_motor[pump].should_count_step_nums)
				step_motor[pump].real_count_step_nums++;
			else step_motor[pump].real_count_step_nums--;
			
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(step_motor[X].real_count_step_nums == step_motor[X].should_count_step_nums)
			{
				MOTOR_EN(step_motor[X].en_port, step_motor[X].en_pin, OFF);
				__HAL_TIM_DISABLE_IT(htim,TIM_IT_CC2);
				LED2_OFF;
			}else if(step_motor[X].real_count_step_nums < step_motor[X].should_count_step_nums)
				step_motor[X].real_count_step_nums++;
			else step_motor[X].real_count_step_nums--;
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if(step_motor[Y].real_count_step_nums == step_motor[Y].should_count_step_nums)
			{
				MOTOR_EN(step_motor[Y].en_port, step_motor[Y].en_pin, OFF);
				__HAL_TIM_DISABLE_IT(htim,TIM_IT_CC3);
				LED3_OFF;
			}else if(step_motor[Y].real_count_step_nums < step_motor[Y].should_count_step_nums)
				step_motor[Y].real_count_step_nums++;
			else step_motor[Y].real_count_step_nums--;
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if(step_motor[Z].real_count_step_nums == step_motor[Z].should_count_step_nums)
			{
				MOTOR_EN(step_motor[Z].en_port, step_motor[Z].en_pin, OFF);
				__HAL_TIM_DISABLE_IT(htim,TIM_IT_CC4);
				LED4_OFF;
			}else if(step_motor[Z].real_count_step_nums < step_motor[Z].should_count_step_nums)
				step_motor[Z].real_count_step_nums++;
			else step_motor[Z].real_count_step_nums--;
		}
		
	}
}


/* 
	����ע����һ��Σ�����ʱ���ж��������������û�в������ַ��������õ�������Ƚ��жϡ�
	ʱ���жϼ���������Ƚ��жϼ������жϵĴ����ٺܶࡣ
	ǰ�߻��жϡ�4���������֮�͡��Σ����߻��жϡ����Զ���ı������Ρ�
	��ʵ�������á�
*/
//void TIM8_UP_TIM13_IRQHandler(void)
//{
//	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM2)
//	{
//		Go_with_joystick();
//	}
//	
//	if(htim->Instance == TIM8)
//	{
//		mark++;
//		if(HAL_GPIO_ReadPin(step_motor[pump].en_port, step_motor[pump].en_pin) == ON)
//		{
//			if(step_motor[pump].real_count_step_nums == step_motor[pump].should_count_step_nums)
//			{
//				MOTOR_EN(step_motor[pump].en_port, step_motor[pump].en_pin, OFF);
//				__HAL_TIM_DISABLE_IT(htim,TIM_IT_UPDATE);
//				LED1_OFF;
//			}else if(step_motor[pump].real_count_step_nums < step_motor[pump].should_count_step_nums)
//				step_motor[pump].real_count_step_nums++;
//			else step_motor[pump].real_count_step_nums--;
//		}
//		if(HAL_GPIO_ReadPin(step_motor[X].en_port, step_motor[X].en_pin) == ON)
//		{
//			if(step_motor[X].real_count_step_nums == step_motor[X].should_count_step_nums)
//			{
//				MOTOR_EN(step_motor[X].en_port, step_motor[X].en_pin, OFF);
//				__HAL_TIM_DISABLE_IT(htim,TIM_IT_UPDATE);
//				LED2_OFF;
//			}else if(step_motor[X].real_count_step_nums < step_motor[X].should_count_step_nums)
//				step_motor[X].real_count_step_nums++;
//			else step_motor[X].real_count_step_nums--;
//		}
//		if(HAL_GPIO_ReadPin(step_motor[Y].en_port, step_motor[Y].en_pin) == ON)
//		{
//			if(step_motor[Y].real_count_step_nums == step_motor[Y].should_count_step_nums)
//			{
//				MOTOR_EN(step_motor[Y].en_port, step_motor[Y].en_pin, OFF);
//				__HAL_TIM_DISABLE_IT(htim,TIM_IT_UPDATE);
//				LED3_OFF;
//			}else if(step_motor[Y].real_count_step_nums < step_motor[Y].should_count_step_nums)
//				step_motor[Y].real_count_step_nums++;
//			else step_motor[Y].real_count_step_nums--;
//		}
//		if(HAL_GPIO_ReadPin(step_motor[Z].en_port, step_motor[Z].en_pin) == ON)
//		{
//			if(step_motor[Z].real_count_step_nums == step_motor[Z].should_count_step_nums)
//			{
//				MOTOR_EN(step_motor[Z].en_port, step_motor[Z].en_pin, OFF);
//				__HAL_TIM_DISABLE_IT(htim,TIM_IT_UPDATE);
//				LED4_OFF;
//			}else if(step_motor[Z].real_count_step_nums < step_motor[Z].should_count_step_nums)
//				step_motor[Z].real_count_step_nums++;
//			else step_motor[Z].real_count_step_nums--;
//		}
//		
//	}
//}


