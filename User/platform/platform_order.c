#include "platform_order.h"
#include "joystick.h"
#include "./usart/bsp_debug_usart.h"	
#include "./lcd/bsp_nt35510_lcd.h"
#include "main.h"
#include "./delay/core_delay.h"
#include "npn.h"
int abs_int(int num)
{
	return (num < 0)? -num : num;
}

float calc_now_position(Stepper_TypeDef motor_control)
{
	return (float)motor_control.real_count_step_nums * motor_control.dao_cheng / motor_control.pulse_every_round; 
}


void Reset_position(uint16_t * LCD_row)
{
	__HAL_TIM_DISABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);
	control_state = auto_control;
	Four_Step_motor_position_set(step_motor, calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]),-400);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	Four_Step_motor_position_set(step_motor, -400,-400,calc_now_position(step_motor[Z]));
	printf("平台复位\r\n");
	LCD_clear(LCD_row);
}

void Shake_Z(void)
{
	uint8_t i =0;
	for(i = 0;i < 10;i++)
	{
	Four_Step_motor_position_set(step_motor, calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]), calc_now_position(step_motor[Z])+1.0f);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	Four_Step_motor_position_set(step_motor, calc_now_position(step_motor[X]), calc_now_position(step_motor[Y]), calc_now_position(step_motor[Z])-1.0f);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	}
}



void Volume_distribute(int v1_n_v2[], int vol)
{
	int min = 9, max = 17;
	v1_n_v2[0] = min;
	while(1)
	{
		if(vol%v1_n_v2[0] == 0) break;
		else if(vol%v1_n_v2[0]>=min&&vol%v1_n_v2[0]<=max) break;
		else v1_n_v2[0] ++;
	}
	v1_n_v2[1] = vol/v1_n_v2[0];
	v1_n_v2[2] = vol%v1_n_v2[0];
}
float Z_give = 85.0f, Z_reset = -100.0f, Z_get = 100.0f;

void Inject_drug(float posX, float posY, Syringe_TypeDef syringe, int vol)
{
	int v1_n_v2[3]={0,0,0};
	
	Four_Step_motor_position_set(step_motor, posX, posY, calc_now_position(step_motor[Z]));
	while(step_motor[X].real_count_step_nums != step_motor[X].should_count_step_nums || step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums)
		Motion_scan(step_motor);
	//XY移动完成
	Four_Step_motor_position_set(step_motor, posX, posY, Z_give);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	//Z移动完成
	Volume_distribute(v1_n_v2, vol);
	//printf("%d = %d X %d + %d\r\n",vol,v1_n_v2[0],v1_n_v2[1],v1_n_v2[2]);
	for(uint8_t i = 0;i<v1_n_v2[1];i++)
	{
		Step_of_pump_run_set_uL(syringe, -v1_n_v2[0]);
		while(step_motor[pump].real_count_step_nums != step_motor[pump].should_count_step_nums)
			Motion_scan(step_motor);
		//pump移动完成
		delay_ms(300);
		Shake_Z();
	}
	if(v1_n_v2[2]!=0)
	{
		Step_of_pump_run_set_uL(syringe, -v1_n_v2[2]);
		while(step_motor[pump].real_count_step_nums != step_motor[pump].should_count_step_nums)
			Motion_scan(step_motor);
		//pump移动完成
		delay_ms(200);
		Shake_Z();
	}
	
	Four_Step_motor_position_set(step_motor,calc_now_position(step_motor[X]),calc_now_position(step_motor[Y]),Z_reset);
	
}


void Inject_drug_into_iXj_well(int row, int column, float now_X_pos, float now_Y_pos, Syringe_TypeDef syringe, int vol)
{
	int i =0 ,j = 0, well_distance = 0;	
	/*
		6well培养皿，直径36mm，圆心距39mm
		12well培养皿，直径24mm,圆心距26mm
	*/
	if(row * column == 6) well_distance = 39;
	else if(row * column == 12) well_distance = 26;
	
	Four_Step_motor_position_set(step_motor, now_X_pos, now_Y_pos, Z_reset);

	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums||step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums||step_motor[X].real_count_step_nums != step_motor[X].should_count_step_nums)
			Motion_scan(step_motor);
/********************** 谜之BUG **********************/
//	for(i = 0; i<row; i++)
//	{
//		if(i%2 == 0)
//			for(j = 0;j<column;j++)
//				Inject_drug(now_X_pos + well_distance * j, now_Y_pos + well_distance * i, syringe , vol);
//		else 
//			for(j = column-1;j>=0;j--)
//				Inject_drug(now_X_pos + well_distance * j, now_Y_pos + well_distance * i, syringe , vol);
//	}
	
	for(i = 0; i<row; i++)
	{
			if(i == 0)
			{
			for(j = 0;j<column;j++)
				if(j!=0)
				Inject_drug(calc_now_position(step_motor[X]) +  well_distance, calc_now_position(step_motor[Y]), syringe , vol);
				else
				Inject_drug(calc_now_position(step_motor[X])  , calc_now_position(step_motor[Y]), syringe , vol);
			}
			else if(i == 1)
			{
			Four_Step_motor_position_set(step_motor,calc_now_position(step_motor[X]),calc_now_position(step_motor[Y])+well_distance, calc_now_position(step_motor[Z]));
				while(step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums)
					Motion_scan(step_motor);
				
			for(j = 0;j<column;j++)
				if(j!=0)
				Inject_drug(calc_now_position(step_motor[X]) - well_distance , calc_now_position(step_motor[Y]), syringe , vol);
				else
				Inject_drug(calc_now_position(step_motor[X])  , calc_now_position(step_motor[Y]), syringe , vol);
			}
			else if(i == 2)
			{
				Four_Step_motor_position_set(step_motor,calc_now_position(step_motor[X]),calc_now_position(step_motor[Y])+well_distance, calc_now_position(step_motor[Z]));
				while(step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums)
					Motion_scan(step_motor);
			for(j = 0;j<column;j++)
				if(j!=0)
				Inject_drug(calc_now_position(step_motor[X]) + well_distance , calc_now_position(step_motor[Y]), syringe , vol);
				else
				Inject_drug(calc_now_position(step_motor[X])  , calc_now_position(step_motor[Y]), syringe , vol);
			}
	}
	
	Four_Step_motor_position_set(step_motor, calc_now_position(step_motor[X]),calc_now_position(step_motor[Y]), -400);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	Four_Step_motor_position_set(step_motor, -400,-400, calc_now_position(step_motor[Z]));
}

void Sample_drug(float posX, float posY, Syringe_TypeDef syringe, int vol)
{
	Four_Step_motor_position_set(step_motor, posX, posY, calc_now_position(step_motor[Z]));
	while(step_motor[X].real_count_step_nums != step_motor[X].should_count_step_nums || step_motor[Y].real_count_step_nums != step_motor[Y].should_count_step_nums)
		Motion_scan(step_motor);
	//XY移动完成
	Four_Step_motor_position_set(step_motor, posX, posY, Z_get);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	//Z移动完成
	Step_of_pump_run_set_uL(syringe, vol);
	while(step_motor[pump].real_count_step_nums != step_motor[pump].should_count_step_nums)
		Motion_scan(step_motor);
	//pump移动完成
	
	Four_Step_motor_position_set(step_motor, posX, posY, Z_reset);
	while(step_motor[Z].real_count_step_nums != step_motor[Z].should_count_step_nums)
		Motion_scan(step_motor);
	//Z拉上去
	
	Inject_drug(posX,posY+85.0f,syringe,vol);

}

void Supply_drug(Syringe_TypeDef syringe, int vol)
{
	
}
