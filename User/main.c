#include "main.h"
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./delay/core_delay.h"
#include "./stepper/bsp_stepper_init.h"
#include "./key/bsp_key.h" 
#include "./led/bsp_led.h"
#include "npn.h"
#include "joystick.h"
#include "platform_order.h"
#include "./lcd/bsp_nt35510_lcd.h"
/*	ע��þ��ȣ�
	1.��֪1mLע������1mL->57mm����С�̶���20uL->1.14mm��˿�˵���Ϊ1mm�����������ת360�㣬��/��1mm
	2.������10000ϸ��+200us����PWM����ô1����/��0.0001mm���ٶ�0.5mm/s
	3.�����㣬������������1uLҪ0.0057mm��Ҫ��57��
*/
 

uint16_t row = 0;
float X_23 = 52,Y_23 = 40,X_34 = 180,Y_34 = 34;

int main(void) 
{
	uint16_t  i = 0 ;
	
	/* ��ʼ��ϵͳʱ��Ϊ168MHz */
	SystemClock_Config();
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	DEBUG_USART_Config();
	/*������ʼ��*/
	Key_GPIO_Config();	
	/*led��ʼ��*/
	LED_GPIO_Config();
	//LCD ��ʼ��
	NT35510_Init (); 
	NT35510_GramScan ( 6 );
	LCD_SetFont(&Font16x32);
	LCD_SetColors(BLACK,WHITE);
	NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
	
	
	/*ҡ�˳�ʼ��*/
	JOYSTICK_Init();
	/*NPN��λ���ء�ҡ��4��������ʼ�����ⲿ�ж�*/
	NPN_Init();
	
	/*���������ʼ��*/
	Step_motor_Init();
	
	/*TIM2��ʼ��������ҡ������*/
	TIM2_Init();

	printf("Hello world!\r\n");
	NT35510_DispStringLine_EN_CH(LINE(row++),"Hello world!");

	/*ƽ̨λ�ó�ʼ��*/
	printf("���� KEY1 ����ƽ̨\r\n���� KEY2 ��������\r\n");
	NT35510_DispStringLine_EN_CH(LINE(row++),"���� KEY1 ����ƽ̨");
	NT35510_DispStringLine_EN_CH(LINE(row++),"���� KEY2 ��������");
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON || ch == '1')
		{
			ch = '0';
			printf("���ڹ�����......\r\n");
			NT35510_DispStringLine_EN_CH(LINE(row++),"���ڹ�����......");
			Platform_Init();
			while(platform_init_success < 3);
			
			while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums||step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums||step_motor[X].real_count_step_nums != step_motor[X].should_count_step_nums)
				Motion_scan(step_motor);
			
			
			printf("�������\r\n");
			NT35510_DispStringLine_EN_CH(LINE(row++),"������ɣ�");
			break;
		}else if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON || ch == '2')
		{
			ch = '0';
			printf("����������\r\n");
			NT35510_DispStringLine_EN_CH(LINE(row++),"���������㣡");
			platform_init_success = 3;
			break;
		}
	}
	
	
	printf("���� KEY1 ѡ���ֶ�����\r\n");
	printf("���� KEY2 ѡ��Ԥ�����\r\n");
	NT35510_DispStringLine_EN_CH(LINE(row++),"���� KEY1 ѡ���ֶ�����");
	NT35510_DispStringLine_EN_CH(LINE(row++),"���� KEY2 ѡ��Ԥ�����");
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON || ch == '1')
		{
			ch = '0';
			control_state = manual_control;
			__HAL_TIM_ENABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
			printf("�ѿ���ҡ���ֶ����ƣ��л�Ԥ����ư���ҡ��\r\n");
			LCD_clear(&row);
			break;
		}else if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON || ch == '2')
		{
			ch = '0';
			control_state = auto_control;
			LCD_clear(&row);
			break;
		}
	}

	while(1)
	{
		i++;
		if(i==5000)
		{
			//printf("now_posX : %f\r\nnow_posY : %f\r\nnow_posZ : %f\r\n",calc_now_position(step_motor[X]),calc_now_position(step_motor[Y]),calc_now_position(step_motor[Z]));
			//printf("mark = %u should = %d real = %d\r\n",mark, step_motor[X].should_count_step_nums,step_motor[X].real_count_step_nums);
		}
		Motion_scan(step_motor);
		/* ��λ������ҡ�˰��� */		
		if(ch == '6')
		{
			ch = '0';
			__HAL_GPIO_EXTI_GENERATE_SWIT(PS2_LEFT_PUSH_INT_GPIO_PIN);
			
		}else if(ch == '7')
		{
			ch = '0';
			__HAL_GPIO_EXTI_GENERATE_SWIT(PS2_RIGHT_PUSH_INT_GPIO_PIN);
		}
		


		
			/*��������λ��Ԥ��ָ��*/
			if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON || ch == '1')//��λ����
			{
				ch = '0';
				Reset_position(&row);
				
			}
			if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON || ch == '2')
			{
				ch = '0';
				if(control_state == auto_control)
				{
					Inject_drug(calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]),syringe[0],10);
					//Four_Step_motor_position_set(step_motor, calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]), 150);
					
				}else
				{
					printf("KEY2ָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
					NT35510_DispStringLine_EN_CH(LINE(row++),"KEY2ָ����Ч");
					NT35510_DispStringLine_EN_CH(LINE(row++),"���Ȱ���ҡ���л���Ԥ��ģʽ");
				}
			}
			
			if(Key_Scan(KEY3_GPIO_PORT,KEY3_PIN) == KEY_ON || ch == '3')
			{
				ch = '0';
				if(control_state == auto_control)
				{
					//Four_Step_motor_position_set(step_motor, 50, 50, calc_now_position(step_motor[Z]));
					Inject_drug_into_iXj_well(2, 3, X_23, Y_23, syringe[0], 10);
				}else
				{
					printf("KEY3ָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
					NT35510_DispStringLine_EN_CH(LINE(row++),"KEY3ָ����Ч");
					NT35510_DispStringLine_EN_CH(LINE(row++),"���Ȱ���ҡ���л���Ԥ��ģʽ");
				}
			}
			
			if(Key_Scan(KEY4_GPIO_PORT,KEY4_PIN) == KEY_ON || ch == '4')
			{
				ch = '0';
				if(control_state == auto_control)
				{ 
					
					//Four_Step_motor_position_set(step_motor, 70, 50, calc_now_position(step_motor[Z]));
					Inject_drug_into_iXj_well(3, 4, X_34, Y_34, syringe[0], 10);
				}else
				{
					printf("KEY4ָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
					NT35510_DispStringLine_EN_CH(LINE(row++),"KEY4ָ����Ч");
					NT35510_DispStringLine_EN_CH(LINE(row++),"���Ȱ���ҡ���л���Ԥ��ģʽ");
				}
			}
			if(Key_Scan(KEY5_GPIO_PORT,KEY5_PIN) == KEY_ON || ch == '5')/* ��������ʾȫ�� */
			{
				ch = '0';
				if(control_state == auto_control)
				{

					//Step_of_pump_run_set_uL(syringe[0], 1000);
					Sample_drug(calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]),syringe[0],1000);
					//Four_Step_motor_position_set(step_motor, 90, 50, calc_now_position(step_motor[Z]));
				}else
				{
					printf("KEY5ָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
					NT35510_DispStringLine_EN_CH(LINE(row++),"KEY5ָ����Ч");
					NT35510_DispStringLine_EN_CH(LINE(row++),"���Ȱ���ҡ���л���Ԥ��ģʽ");
				}
			}
			
			Single_hole_operation_detect();
		

	}

} 	



void Single_hole_operation_detect(void)
{
	int well_2X3 = 39, well_3X4 = 26;
	
	if(ch>='A'&&ch<='R')
	{
		//printf("%c\r\n%d\r\n",ch,volume_from_PC);
		if(control_state == auto_control)
		{
			if(ch>='A'&&ch<='F')
			{
				//Inject_drug(calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]),syringe[0],volume_from_PC);
				Inject_drug(X_23+(int)((ch-'A')%3) * well_2X3,Y_23+(int)((ch-'A')/3) * well_2X3,syringe[0], volume_from_PC);
				ch = '0';
				volume_from_PC = 0;
			}
			else
			{
				Inject_drug(X_34+(int)((ch-'G')%4) * well_3X4,Y_34+(int)((ch-'G')/4) * well_3X4,syringe[0], volume_from_PC);
				ch = '0';
				volume_from_PC = 0;
			}
		}
		else
		{
			printf("��ҩָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
		}
	}
	else if(ch>='a'&&ch<='r')
	{
		if(control_state == auto_control)
		{
			if(ch>='a'&&ch<='f')
			{
				Sample_drug(X_23+(int)((ch-'a')%3) * well_2X3,Y_23+(int)((ch-'f')/3) * well_2X3,syringe[0], volume_from_PC);
				ch = '0';
				volume_from_PC = 0;
			}
			else
			{
				Sample_drug(X_34+(int)((ch-'g')%4) * well_3X4,Y_34+(int)((ch-'g')/4) * well_3X4,syringe[0], volume_from_PC);
				ch = '0';
				volume_from_PC = 0;
			}
		}
		else
		{
			printf("����ָ����Ч�����Ȱ���ҡ���л���Ԥ��ģʽ\r\n");
		}
	}
			
}


void LCD_clear(uint16_t * LCD_row)
{
	NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
	*LCD_row = 0;
	if(control_state == manual_control)//�˿����ֶ�ģʽ
	{
			NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"�ѿ���ҡ���ֶ�����");
			NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"�л�Ԥ����ư���ҡ��");
			NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY1 ------ ��λ");
			printf("��ѡ���ֶ����ƣ��л�Ԥ������밴��ҡ��\r\n");
			printf("KEY1 ------ ��λ\r\n");
	}
	else if(control_state == auto_control)
		LCD_USART_show_command(LCD_row);
}

void LCD_USART_show_command(uint16_t * LCD_row)
{
	printf("��ѡ��Ԥ����ƣ��л��ֶ������밴��ҡ��\r\n\
KEY1 ------ ��λ\r\n\
KEY2 ------ ���׸�ҩ\r\n\
KEY3 ------ 2X3 �װ��ҩ\r\n\
KEY4 ------ 3X4 �װ��ҩ\r\n\
KEY5 ------ ���ײ���\r\n");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"��ѡ��Ԥ��������");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"�����ֶ������밴��ҡ��");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY1 ------ ��λ");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY2 ------ ���׸�ҩ");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY3 ------ 2X3�װ��ҩ");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY4 ------ 3X4�װ��ҩ");
	NT35510_DispStringLine_EN_CH(LINE((*LCD_row)++),"KEY5 ------ ���ײ���");
}






/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
 void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1) {};
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
