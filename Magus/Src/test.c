#include <stdint.h>
#include "device.h"
#include "HAL_MAX11300.h"
#include "HAL_TLC5946.h"
#include "HAL_OLED.h"
#include "HAL_Encoders.h"

uint16_t usiResult;
extern uint16_t rgENC_Values[7];

void setup(void);
void loop(void);

void setup(void){
  extern TIM_HandleTypeDef htim3;
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

  extern SPI_HandleTypeDef hspi5;
  TLC5946_init(&hspi5);
  MAX11300_init(&hspi5);
  OLED_init(&hspi5);
	Encoders_init(&hspi5);

  // Pixi
  MAX11300_setDeviceControl(DCR_DACCTL_ImmUpdate|DCR_DACREF_Int|DCR_ADCCTL_ContSweep/*|DCR_BRST_Contextual*/);

  MAX11300_setPortMode(1,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(2,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(3,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(4,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(5,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(6, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(7, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(8, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
	MAX11300_setPortMode(9,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(10, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(1,  	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(12, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(13, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(14, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
  MAX11300_setPortMode(15, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);
	MAX11300_setPortMode(16, 	PCR_Range_ADC_0_P10|PCR_Mode_ADC_SgEn_PosIn|PCR_ADCSamples_16|PCR_ADCref_INT);

//  MAX11300_setPortMode(0,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_1,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
//  MAX11300_setPortMode(2,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_3,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
//  MAX11300_setPortMode(4,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_5,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
//  MAX11300_setPortMode(6,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_7,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
//  MAX11300_setPortMode(8,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_9,  PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
//  MAX11300_setPortMode(10, PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_11, PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
 // MAX11300_setPortMode(12, PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_13, PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
 // MAX11300_setPortMode(14, PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
  /* MAX11300_setPortMode(PORT_15, PCR_Range_DAC_M5_P5|PCR_Mode_DAC); */
 // MAX11300_setPortMode(16, PCR_Range_DAC_M5_P5|PCR_Mode_DAC);
	
  // LEDs
  Magus_setRGB_DC(63,63,63);
  
	
	OLED_ClearScreen();
}

void loop(void)
{
	for(int x=1; x<17; x++)
	{
		usiResult = MAX11300_readADC(x);
		Magus_setRGB(x, usiResult, usiResult, usiResult);
	}
	
	OLED_Refresh();
	TLC5946_Refresh_GS();
	Encoders_readAll();
	HAL_Delay(10);
}