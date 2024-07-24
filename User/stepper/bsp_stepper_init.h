#ifndef __BSP_STEPPER_INIT_H
#define	__BSP_STEPPER_INIT_H

#include "stm32f4xx_hal.h"

typedef struct{
	uint16_t total_volume;
	float length;
}Syringe_TypeDef;

/* 步进电机结构体 */
typedef struct{
  uint16_t pul_pin;                     //电机脉冲输出引脚号
  uint16_t dir_pin;                     //电机方向引脚号
  uint16_t en_pin;                      //电机使能引脚号
  uint32_t pul_channel;                 //电机脉冲输出通道
  GPIO_TypeDef *pul_port;               //电机脉冲引脚端口
  GPIO_TypeDef *dir_port;               //电机方向引脚端口
  GPIO_TypeDef *en_port;                //电机使能引脚端口

	uint16_t pulse_every_round;
	float  dao_cheng;		//丝杆导程
	
	uint32_t period;
	__IO int should_count_step_nums;
	__IO int real_count_step_nums;
	
	float set_position;
	float now_position;
}Stepper_TypeDef;

/*宏定义*/
/*******************************************************/

//设定注射泵为motor[P],XYZ三轴电机分别为motor[X],motor[Y],motor[Z]
enum
{
	pump = 0,
	X,
	Y,
	Z
};


//使用定时器8，4个通道
#define MOTOR_PUL_TIM                    TIM8
#define MOTOR_PUL_CLK_ENABLE()  		     __TIM8_CLK_ENABLE()
#define MOTOR_PUL_GPIO_AF                GPIO_AF3_TIM8

//通道1，pump电机
//方向
#define MOTOR_DIR_pump_PIN                  	GPIO_PIN_1   
#define MOTOR_DIR_pump_GPIO_PORT            	GPIOE
#define MOTOR_DIR_pump_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOE_CLK_ENABLE()

//使能
#define MOTOR_EN_pump_PIN                  	  GPIO_PIN_0
#define MOTOR_EN_pump_GPIO_PORT            	  GPIOE
#define MOTOR_EN_pump_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOE_CLK_ENABLE()

//脉冲
#define MOTOR_PUL_pump_PORT       			      GPIOI
#define MOTOR_PUL_pump_PIN             		    GPIO_PIN_5
#define MOTOR_PUL_pump_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOI_CLK_ENABLE()

//定时器通道
#define MOTOR_PUL_pump_CHANNEL                TIM_CHANNEL_1

/*******************************************************/
//通道2,X轴电机
//Motor 方向
#define MOTOR_DIR_X_PIN                  	GPIO_PIN_8
#define MOTOR_DIR_X_GPIO_PORT            	GPIOI          
#define MOTOR_DIR_X_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor 使能
#define MOTOR_EN_X_PIN                     GPIO_PIN_4
#define MOTOR_EN_X_GPIO_PORT               GPIOE                       
#define MOTOR_EN_X_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOE_CLK_ENABLE()

//Motor 脉冲
#define MOTOR_PUL_X_PORT       			      GPIOI
#define MOTOR_PUL_X_PIN             		    GPIO_PIN_6
#define MOTOR_PUL_X_GPIO_CLK_ENABLE()	  	__HAL_RCC_GPIOI_CLK_ENABLE()

//定时器通道
#define MOTOR_PUL_X_CHANNEL                TIM_CHANNEL_2
/*******************************************************/

//通道3,Y轴电机
//Motor 方向
#define MOTOR_DIR_Y_PIN                  	GPIO_PIN_11
#define MOTOR_DIR_Y_GPIO_PORT            	GPIOI          
#define MOTOR_DIR_Y_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor 使能
#define MOTOR_EN_Y_PIN                  	  GPIO_PIN_10
#define MOTOR_EN_Y_GPIO_PORT            	  GPIOI                 
#define MOTOR_EN_Y_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor 脉冲
#define MOTOR_PUL_Y_PORT       			      GPIOI
#define MOTOR_PUL_Y_PIN             		    GPIO_PIN_7
#define MOTOR_PUL_Y_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOI_CLK_ENABLE()

//定时器通道
#define MOTOR_PUL_Y_CHANNEL                TIM_CHANNEL_3
/*******************************************************/

//通道4,Z轴电机
//Motor 方向
#define MOTOR_DIR_Z_PIN                  	GPIO_PIN_2
#define MOTOR_DIR_Z_GPIO_PORT            	GPIOF
#define MOTOR_DIR_Z_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOF_CLK_ENABLE()

//Motor 使能
#define MOTOR_EN_Z_PIN                  	  GPIO_PIN_1
#define MOTOR_EN_Z_GPIO_PORT            	  GPIOF       
#define MOTOR_EN_Z_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOF_CLK_ENABLE()

//Motor 脉冲
#define MOTOR_PUL_Z_PORT       			      GPIOC
#define MOTOR_PUL_Z_PIN             		    GPIO_PIN_9
#define MOTOR_PUL_Z_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

//定时器通道
#define MOTOR_PUL_Z_CHANNEL                TIM_CHANNEL_4


//控制使能引脚
/* 带参宏，可以像内联函数一样使用 */
#define MOTOR_EN(port, pin, x)       		HAL_GPIO_WritePin(port, pin, x)
#define MOTOR_PUL(port, pin, x)           HAL_GPIO_WritePin(port, pin, x)
#define MOTOR_DIR(port, pin, x)           HAL_GPIO_WritePin(port, pin, x)

#define MOTOR_EN_Toggle(port, pin)  		HAL_GPIO_TogglePin(port, pin)
#define MOTOR_PUL_Toggle(port, pin)			HAL_GPIO_TogglePin(port, pin)
#define MOTOR_DIR_Toggle(port, pin)  		HAL_GPIO_TogglePin(port, pin)


/*频率相关参数*/
//定时器实际时钟频率为：168MHz/TIM_PRESCALER
//其中 高级定时器的 频率为168MHz,其他定时器为84MHz

//具体需要的频率可以自己计算
#define TIM_PRESCALER                168
#define TIM_PERIOD_for_pump          200	//慢推情况下，快推可以用500甚至100
#define TIM_PERIOD_for_X          	500
#define TIM_PERIOD_for_Y			500
#define TIM_PERIOD_for_Z			500
#define TIM_PERIOD_for_INIT				500



#define pulse_every_round_for_pump 10000	//10000步每转
#define pulse_every_round_for_X 3200	//3200步每转
#define pulse_every_round_for_Y 3200
#define pulse_every_round_for_Z 1600

#define min_accurate_volume 10	//最小精度值，需要更大体积就 X 这个值，用于Step_of_pump_run_set_uL函数

/************************************************************/
#define HIGH GPIO_PIN_SET	  //高电平
#define LOW  GPIO_PIN_RESET	//低电平

#define ON   LOW	          //开
#define OFF  HIGH	          //关

#define PULL  HIGH				//拉注射器
#define PUSH  LOW				//推注射器

/* 步进电机数组 */
extern Stepper_TypeDef step_motor[4];
/* 注射器数组 */
extern Syringe_TypeDef syringe[1];
/* 总中断次数 */
extern int mark;
/* 步进电机数量 */
extern uint8_t member_count_of_motor;
/* TIM8句柄 */
extern TIM_HandleTypeDef  TIM_TimeBaseStructure; 

/*初始化函数*/
void Step_GPIO_Config(void);
void TIM8_PWMOUTPUT_Config(void);
void Step_motor_Init(void);

/*用户接口函数*/

void Four_Step_motor_position_set(Stepper_TypeDef motor_control[], float posX, float posY, float posZ);

void Motion_scan(Stepper_TypeDef motor_control[]);

void Step_of_pump_run_set_uL(Syringe_TypeDef syringe, int volume);

void Step_run_set_distance(Stepper_TypeDef* motor_control, float distance);

void Step_run_set_angle(Stepper_TypeDef* motor_control, float angle);

#endif /* __BSP_STEPPER_INIT_H */
