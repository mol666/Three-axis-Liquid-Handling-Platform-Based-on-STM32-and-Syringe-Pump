#include "joystick.h"
#include "./stepper/bsp_stepper_init.h"
#include "npn.h"
__IO uint16_t ADC_ConvertedValue[4]={0};//���12λ����ֵ
/*
	4ͨ����ֵֹ 2046 1972 2054 2054
	��Ҫ���� +2 +76 -6 -6
*/
int correct_value[4] = {2, 76, -6, -6};

float ADC_ConvertedValueLocal[4]={0}; //��ŵ�ѹֵ

DMA_HandleTypeDef DMA_Init_Handle;

ADC_HandleTypeDef ADC_Handle;

ADC_ChannelConfTypeDef ADC_Config;


void JOYSTICK_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*=====================ͨ��1======================*/
    // ʹ�� GPIO ʱ��
    JOYSTICK_ADC_GPIO_CLK_PUMP_ENABLE();   
    // ���� IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_PUMP;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_PUMP, &GPIO_InitStructure);
    
    /*=====================ͨ��2======================*/
    // ʹ�� GPIO ʱ��
    JOYSTICK_ADC_GPIO_CLK_Z_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_Z;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_Z, &GPIO_InitStructure);
    
    /*=====================ͨ��3======================*/
    // ʹ�� GPIO ʱ��
    JOYSTICK_ADC_GPIO_CLK_X_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_X;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_X, &GPIO_InitStructure);

    /*=====================ͨ��4======================*/
    // ʹ�� GPIO ʱ��
    JOYSTICK_ADC_GPIO_CLK_Y_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_Y;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_Y, &GPIO_InitStructure);


}



void JOYSTICK_ADC_Mode_Config(void)
{

    // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
    // ADC3ʹ��DMA2��������1��ͨ��2��������ֲ�̶�����
    // ����DMAʱ��
    JOYSTICK_ADC_DMA_CLK_ENABLE();
    // ���ݴ���ͨ��
    DMA_Init_Handle.Instance = JOYSTICK_ADC_DMA_STREAM;
    // ���ݴ��䷽��Ϊ���赽�洢��	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // �洢����ַ�̶�
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // �������ݴ�СΪ���֣��������ֽ� 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
    // ѭ������ģʽ
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO ��С��FIFOģʽ��ֹʱ�������������	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // ѡ�� DMA ͨ����ͨ������������
    DMA_Init_Handle.Init.Channel = JOYSTICK_ADC_DMA_CHANNEL; 
    //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
    HAL_DMA_Init(&DMA_Init_Handle); 

    HAL_DMA_Start (&DMA_Init_Handle,JOYSTICK_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,4);

    // ����ADCʱ��
    JOYSTICK_ADC_CLK_ENABLE();
    // -------------------ADC Init �ṹ�� ���� ��ʼ��------------------------
    // ADC3
    ADC_Handle.Instance = JOYSTICK_ADC;
    // ʱ��Ϊfpclk 4��Ƶ	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC �ֱ���
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // ɨ��ģʽ����ͨ���ɼ�����Ҫ	
    ADC_Handle.Init.ScanConvMode = ENABLE; 
    // ����ת��	
    ADC_Handle.Init.ContinuousConvMode = ENABLE;
    // ������ת��	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // ������ת������
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //��ֹ�ⲿ���ش���    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //ʹ������������ⲿ�����������ã�ע�͵�����
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //�����Ҷ���	
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //ת��ͨ������
    ADC_Handle.Init.NbrOfConversion = 4;
    //ʹ������ת������
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;
    //ת����ɱ�־
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // ��ʼ��ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    // ���� ADC ͨ��PUMPת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_PUMP;
    ADC_Config.Rank         = 1;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // ���� ADC ͨ��Zת��˳��Ϊ4�����ĸ�ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_Z;
    ADC_Config.Rank         = 4;
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES; // ����ʱ����	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // ���� ADC ͨ��Xת��˳��Ϊ2���ڶ���ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_X;
    ADC_Config.Rank         = 2;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
	
	    // ���� ADC ͨ��Yת��˳��Ϊ3��������ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_Y;
    ADC_Config.Rank         = 3;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);

    HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1);
}

void JOYSTICK_Init(void)
{
	JOYSTICK_ADC_GPIO_Config();
	JOYSTICK_ADC_Mode_Config();
}

void ADC_get_value(void)
{
	ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*(float)3.3;
	ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*(float)3.3;
	ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*(float)3.3;
	ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[2]/4096*(float)3.3;

}

void Go_with_joystick(void)
{
	int i = 0, j = 0, dead_value = 40;	//����
	switch(key_state)
	{
		case only_X:
			i = X;
			j = i+1;
			break;
		case only_Y:
			i = Y;
			j = i+1;
			break;
		case only_Z:
			i = Z;
			j = i+1;
			break;
		case only_pump:
			i = pump;
			j = i+1;
			break;
		
		case all_ok:
			i = 1;
			j = 4;
			break;
		default:
			break;
	}
	for(;i<j;i++)
	{
		if(ADC_ConvertedValue[i] +correct_value[i]  > dead_value + 2048||ADC_ConvertedValue[i] + correct_value[i] < 2048 - dead_value)
			step_motor[i].should_count_step_nums = step_motor[i].real_count_step_nums - (ADC_ConvertedValue[i]+correct_value[i] - 2048)*2;
		else step_motor[i].should_count_step_nums = step_motor[i].real_count_step_nums;
	}
}
//���ö�ʱ��2����Ϊҡ������scan
TIM_HandleTypeDef TIM2_TimeBaseStructure_for_joystick;
void TIM2_Init(void)
{
	
	__TIM2_CLK_ENABLE();
	
	TIM2_TimeBaseStructure_for_joystick.Instance = TIM2;
	TIM2_TimeBaseStructure_for_joystick.Init.Period = 500 - 1;	//500ms
	TIM2_TimeBaseStructure_for_joystick.Init.Prescaler = 168000 - 1;
	TIM2_TimeBaseStructure_for_joystick.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2_TimeBaseStructure_for_joystick.Init.ClockDivision =  TIM_CLOCKDIVISION_DIV1;
	
	HAL_TIM_Base_Init(&TIM2_TimeBaseStructure_for_joystick);
	// ������ʱ�������ж�
	__HAL_TIM_CLEAR_FLAG(&TIM2_TimeBaseStructure_for_joystick, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM2_TimeBaseStructure_for_joystick);
	__HAL_TIM_DISABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);	
		//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 3);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2_TimeBaseStructure_for_joystick);
	
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		Go_with_joystick();
	}
}
