#include "mic.h"
// =====================================================================
// [6] 麦克风 ADC 采集 (适配 Camera 排针第 14 脚: PA6 -> ADC1_IN6)
// =====================================================================
void Audio_ADC_Init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_InitTypeDef   ADC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 配置 PA6 为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;      
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;               
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
}

uint16_t Get_Audio_Raw(void) {
    // 对应 ADC 通道 6
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles);
    
    ADC_SoftwareStartConv(ADC1);           
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 
    return ADC_GetConversionValue(ADC1);   
}