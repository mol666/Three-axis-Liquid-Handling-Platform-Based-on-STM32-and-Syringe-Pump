#ifndef __PLATFORM_ORDER_H
#define __PLATFORM_ORDER_H

#include "stm32f4xx_hal.h"
#include "./stepper/bsp_stepper_init.h"


float calc_now_position(Stepper_TypeDef motor_control);
void Shake_Z(void);
void Reset_position(uint16_t * LCD_row);
void Inject_drug(float posX, float posY, Syringe_TypeDef syringe, int vol);
void Inject_drug_into_iXj_well(int row, int column, float now_X_pos, float now_Y_pos, Syringe_TypeDef syringe, int vol);
void Sample_drug(float posX, float posY, Syringe_TypeDef syringe, int vol);
void Supply_drug(Syringe_TypeDef syringe, int vol);
#endif

