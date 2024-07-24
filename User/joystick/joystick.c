#include "joystick.h"
#include "./stepper/bsp_stepper_init.h"
#include "npn.h"
__IO uint16_t ADC_ConvertedValue[4]={0};//存放12位采样值
/*
	4通道静止值 2046 1972 2054 2054
	需要修正 +2 +76 -6 -6
*/
int correct_value[4] = {2, 76, -6, -6};

float ADC_ConvertedValueLocal[4]={0}; //存放电压值

DMA_HandleTypeDef DMA_Init_Handle;

ADC_HandleTypeDef ADC_Handle;

ADC_ChannelConfTypeDef ADC_Config;


void JOYSTICK_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*=====================通道1======================*/
    // 使能 GPIO 时钟
    JOYSTICK_ADC_GPIO_CLK_PUMP_ENABLE();   
    // 配置 IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_PUMP;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_PUMP, &GPIO_InitStructure);
    
    /*=====================通道2======================*/
    // 使能 GPIO 时钟
    JOYSTICK_ADC_GPIO_CLK_Z_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_Z;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_Z, &GPIO_InitStructure);
    
    /*=====================通道3======================*/
    // 使能 GPIO 时钟
    JOYSTICK_ADC_GPIO_CLK_X_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_X;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_X, &GPIO_InitStructure);

    /*=====================通道4======================*/
    // 使能 GPIO 时钟
    JOYSTICK_ADC_GPIO_CLK_Y_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = JOYSTICK_ADC_GPIO_PIN_Y;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(JOYSTICK_ADC_GPIO_PORT_Y, &GPIO_InitStructure);


}



void JOYSTICK_ADC_Mode_Config(void)
{

    // ------------------DMA Init 结构体参数 初始化--------------------------
    // ADC3使用DMA2，数据流1，通道2，这个是手册固定死的
    // 开启DMA时钟
    JOYSTICK_ADC_DMA_CLK_ENABLE();
    // 数据传输通道
    DMA_Init_Handle.Instance = JOYSTICK_ADC_DMA_STREAM;
    // 数据传输方向为外设到存储器	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // 外设寄存器只有一个，地址不用递增
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // 存储器地址固定
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // 外设数据大小为半字，即两个字节 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	存储器数据大小也为半字，跟外设数据大小相同
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
    // 循环传输模式
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // 禁止DMA FIFO	，使用直连模式
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO 大小，FIFO模式禁止时，这个不用配置	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // 选择 DMA 通道，通道存在于流中
    DMA_Init_Handle.Init.Channel = JOYSTICK_ADC_DMA_CHANNEL; 
    //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
    HAL_DMA_Init(&DMA_Init_Handle); 

    HAL_DMA_Start (&DMA_Init_Handle,JOYSTICK_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,4);

    // 开启ADC时钟
    JOYSTICK_ADC_CLK_ENABLE();
    // -------------------ADC Init 结构体 参数 初始化------------------------
    // ADC3
    ADC_Handle.Instance = JOYSTICK_ADC;
    // 时钟为fpclk 4分频	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC 分辨率
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // 扫描模式，多通道采集才需要	
    ADC_Handle.Init.ScanConvMode = ENABLE; 
    // 连续转换	
    ADC_Handle.Init.ContinuousConvMode = ENABLE;
    // 非连续转换	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // 非连续转换个数
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //禁止外部边沿触发    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //使用软件触发，外部触发不用配置，注释掉即可
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //数据右对齐	
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //转换通道个数
    ADC_Handle.Init.NbrOfConversion = 4;
    //使能连续转换请求
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;
    //转换完成标志
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // 初始化ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    // 配置 ADC 通道PUMP转换顺序为1，第一个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_PUMP;
    ADC_Config.Rank         = 1;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // 配置 ADC 通道Z转换顺序为4，第四个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_Z;
    ADC_Config.Rank         = 4;
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES; // 采样时间间隔	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // 配置 ADC 通道X转换顺序为2，第二个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_X;
    ADC_Config.Rank         = 2;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
	
	    // 配置 ADC 通道Y转换顺序为3，第三个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = JOYSTICK_ADC_CHANNEL_Y;
    ADC_Config.Rank         = 3;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
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
	int i = 0, j = 0, dead_value = 40;	//死区
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
//启用定时器2，作为摇杆数据scan
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
	// 开启定时器更新中断
	__HAL_TIM_CLEAR_FLAG(&TIM2_TimeBaseStructure_for_joystick, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM2_TimeBaseStructure_for_joystick);
	__HAL_TIM_DISABLE_IT(&TIM2_TimeBaseStructure_for_joystick,TIM_IT_UPDATE);	
		//设置抢占优先级，子优先级
	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 3);
	// 设置中断来源
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
