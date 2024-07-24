#ifndef __BSP_STEPPER_INIT_H
#define	__BSP_STEPPER_INIT_H

#include "stm32f4xx_hal.h"

typedef struct{
	uint16_t total_volume;
	float length;
}Syringe_TypeDef;

/* ��������ṹ�� */
typedef struct{
  uint16_t pul_pin;                     //�������������ź�
  uint16_t dir_pin;                     //����������ź�
  uint16_t en_pin;                      //���ʹ�����ź�
  uint32_t pul_channel;                 //����������ͨ��
  GPIO_TypeDef *pul_port;               //����������Ŷ˿�
  GPIO_TypeDef *dir_port;               //����������Ŷ˿�
  GPIO_TypeDef *en_port;                //���ʹ�����Ŷ˿�

	uint16_t pulse_every_round;
	float  dao_cheng;		//˿�˵���
	
	uint32_t period;
	__IO int should_count_step_nums;
	__IO int real_count_step_nums;
	
	float set_position;
	float now_position;
}Stepper_TypeDef;

/*�궨��*/
/*******************************************************/

//�趨ע���Ϊmotor[P],XYZ�������ֱ�Ϊmotor[X],motor[Y],motor[Z]
enum
{
	pump = 0,
	X,
	Y,
	Z
};


//ʹ�ö�ʱ��8��4��ͨ��
#define MOTOR_PUL_TIM                    TIM8
#define MOTOR_PUL_CLK_ENABLE()  		     __TIM8_CLK_ENABLE()
#define MOTOR_PUL_GPIO_AF                GPIO_AF3_TIM8

//ͨ��1��pump���
//����
#define MOTOR_DIR_pump_PIN                  	GPIO_PIN_1   
#define MOTOR_DIR_pump_GPIO_PORT            	GPIOE
#define MOTOR_DIR_pump_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOE_CLK_ENABLE()

//ʹ��
#define MOTOR_EN_pump_PIN                  	  GPIO_PIN_0
#define MOTOR_EN_pump_GPIO_PORT            	  GPIOE
#define MOTOR_EN_pump_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOE_CLK_ENABLE()

//����
#define MOTOR_PUL_pump_PORT       			      GPIOI
#define MOTOR_PUL_pump_PIN             		    GPIO_PIN_5
#define MOTOR_PUL_pump_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOI_CLK_ENABLE()

//��ʱ��ͨ��
#define MOTOR_PUL_pump_CHANNEL                TIM_CHANNEL_1

/*******************************************************/
//ͨ��2,X����
//Motor ����
#define MOTOR_DIR_X_PIN                  	GPIO_PIN_8
#define MOTOR_DIR_X_GPIO_PORT            	GPIOI          
#define MOTOR_DIR_X_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor ʹ��
#define MOTOR_EN_X_PIN                     GPIO_PIN_4
#define MOTOR_EN_X_GPIO_PORT               GPIOE                       
#define MOTOR_EN_X_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOE_CLK_ENABLE()

//Motor ����
#define MOTOR_PUL_X_PORT       			      GPIOI
#define MOTOR_PUL_X_PIN             		    GPIO_PIN_6
#define MOTOR_PUL_X_GPIO_CLK_ENABLE()	  	__HAL_RCC_GPIOI_CLK_ENABLE()

//��ʱ��ͨ��
#define MOTOR_PUL_X_CHANNEL                TIM_CHANNEL_2
/*******************************************************/

//ͨ��3,Y����
//Motor ����
#define MOTOR_DIR_Y_PIN                  	GPIO_PIN_11
#define MOTOR_DIR_Y_GPIO_PORT            	GPIOI          
#define MOTOR_DIR_Y_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor ʹ��
#define MOTOR_EN_Y_PIN                  	  GPIO_PIN_10
#define MOTOR_EN_Y_GPIO_PORT            	  GPIOI                 
#define MOTOR_EN_Y_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOI_CLK_ENABLE()

//Motor ����
#define MOTOR_PUL_Y_PORT       			      GPIOI
#define MOTOR_PUL_Y_PIN             		    GPIO_PIN_7
#define MOTOR_PUL_Y_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOI_CLK_ENABLE()

//��ʱ��ͨ��
#define MOTOR_PUL_Y_CHANNEL                TIM_CHANNEL_3
/*******************************************************/

//ͨ��4,Z����
//Motor ����
#define MOTOR_DIR_Z_PIN                  	GPIO_PIN_2
#define MOTOR_DIR_Z_GPIO_PORT            	GPIOF
#define MOTOR_DIR_Z_GPIO_CLK_ENABLE()   	  __HAL_RCC_GPIOF_CLK_ENABLE()

//Motor ʹ��
#define MOTOR_EN_Z_PIN                  	  GPIO_PIN_1
#define MOTOR_EN_Z_GPIO_PORT            	  GPIOF       
#define MOTOR_EN_Z_GPIO_CLK_ENABLE()    	  __HAL_RCC_GPIOF_CLK_ENABLE()

//Motor ����
#define MOTOR_PUL_Z_PORT       			      GPIOC
#define MOTOR_PUL_Z_PIN             		    GPIO_PIN_9
#define MOTOR_PUL_Z_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

//��ʱ��ͨ��
#define MOTOR_PUL_Z_CHANNEL                TIM_CHANNEL_4


//����ʹ������
/* ���κ꣬��������������һ��ʹ�� */
#define MOTOR_EN(port, pin, x)       		HAL_GPIO_WritePin(port, pin, x)
#define MOTOR_PUL(port, pin, x)           HAL_GPIO_WritePin(port, pin, x)
#define MOTOR_DIR(port, pin, x)           HAL_GPIO_WritePin(port, pin, x)

#define MOTOR_EN_Toggle(port, pin)  		HAL_GPIO_TogglePin(port, pin)
#define MOTOR_PUL_Toggle(port, pin)			HAL_GPIO_TogglePin(port, pin)
#define MOTOR_DIR_Toggle(port, pin)  		HAL_GPIO_TogglePin(port, pin)


/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/TIM_PRESCALER
//���� �߼���ʱ���� Ƶ��Ϊ168MHz,������ʱ��Ϊ84MHz

//������Ҫ��Ƶ�ʿ����Լ�����
#define TIM_PRESCALER                168
#define TIM_PERIOD_for_pump          200	//��������£����ƿ�����500����100
#define TIM_PERIOD_for_X          	500
#define TIM_PERIOD_for_Y			500
#define TIM_PERIOD_for_Z			500
#define TIM_PERIOD_for_INIT				500



#define pulse_every_round_for_pump 10000	//10000��ÿת
#define pulse_every_round_for_X 3200	//3200��ÿת
#define pulse_every_round_for_Y 3200
#define pulse_every_round_for_Z 1600

#define min_accurate_volume 10	//��С����ֵ����Ҫ��������� X ���ֵ������Step_of_pump_run_set_uL����

/************************************************************/
#define HIGH GPIO_PIN_SET	  //�ߵ�ƽ
#define LOW  GPIO_PIN_RESET	//�͵�ƽ

#define ON   LOW	          //��
#define OFF  HIGH	          //��

#define PULL  HIGH				//��ע����
#define PUSH  LOW				//��ע����

/* ����������� */
extern Stepper_TypeDef step_motor[4];
/* ע�������� */
extern Syringe_TypeDef syringe[1];
/* ���жϴ��� */
extern int mark;
/* ����������� */
extern uint8_t member_count_of_motor;
/* TIM8��� */
extern TIM_HandleTypeDef  TIM_TimeBaseStructure; 

/*��ʼ������*/
void Step_GPIO_Config(void);
void TIM8_PWMOUTPUT_Config(void);
void Step_motor_Init(void);

/*�û��ӿں���*/

void Four_Step_motor_position_set(Stepper_TypeDef motor_control[], float posX, float posY, float posZ);

void Motion_scan(Stepper_TypeDef motor_control[]);

void Step_of_pump_run_set_uL(Syringe_TypeDef syringe, int volume);

void Step_run_set_distance(Stepper_TypeDef* motor_control, float distance);

void Step_run_set_angle(Stepper_TypeDef* motor_control, float angle);

#endif /* __BSP_STEPPER_INIT_H */
