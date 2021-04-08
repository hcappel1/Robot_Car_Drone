/*
 * range_sensors.c
 *
 *  Created on: Mar 22, 2021
 *      Author: henca
 */

#include "range_sensors.h"

//Specific variables
uint8_t icFlag1 = 0;
uint8_t captureIdx1 = 0;
uint32_t edge1Time1=0, edge2Time1=0;
uint8_t icFlag2 = 0;
uint8_t captureIdx2 = 0;
uint32_t edge1Time2=0, edge2Time2=0;
uint8_t icFlag3 = 0;
uint8_t captureIdx3 = 0;
uint32_t edge1Time3=0, edge2Time3=0;
const float SpeedSound = 0.0343;

//Global variables
uint8_t USNUM = 1;

//Range variables
float RangeR1 = 0.0f;
float RangeF1 = 0.0f;

float RangeR2 = 0.0f;
float RangeF2 = 0.0f;

float RangeR3 = 0.0f;
float RangeF3 = 0.0f;


//Functions
void usDelay(uint32_t uSec)
{
	if(uSec < 2) uSec = 2;
	TIM4->ARR = uSec - 1;
	TIM4->EGR = 1;
	TIM4->SR &= ~1;
	TIM4->CR1 |= 1;
	while((TIM4->SR&0x0001) != 1);
	TIM4->SR &= ~(0x0001);
}

void LowPassFilter(float *rangeR, float *rangeF)
{

	if (rangeR != 0){
		*rangeF = (0.75)*(*rangeF) + (0.25)*(*rangeR);
	}

}

void GetRangeData1(TIM_HandleTypeDef *htim)
{
	//1. Set TRIG pin low for usec
	HAL_GPIO_WritePin(TRIG_1_GPIO_Port, TRIG_1_Pin, GPIO_PIN_RESET);
	usDelay(3);

	//2. Send quick pulse from trig pin
	HAL_GPIO_WritePin(TRIG_1_GPIO_Port, TRIG_1_Pin, GPIO_PIN_SET);
	usDelay(10);
	HAL_GPIO_WritePin(TRIG_1_GPIO_Port, TRIG_1_Pin, GPIO_PIN_RESET);

	//3. ECHO signal
	USNUM = 1;
	HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
	//Wait for IC flag
	uint32_t startTick = HAL_GetTick();
	do
	{
		if(icFlag1) break;
	}while((HAL_GetTick() - startTick) < 500);
	icFlag1 = 0;
	HAL_TIM_IC_Stop_IT(htim, TIM_CHANNEL_1);

	//4. calculate distance in cm
	if(edge2Time1 > edge1Time1)
	{
		RangeR1 = ((edge2Time1 - edge1Time1) + 0.0f)*(SpeedSound/2);
		RangeF1 = RangeR1;
		//LowPassFilter(&range1R, &range1F);
	}
	else
	{
		RangeR1 = 0.0f;
		RangeF1 = RangeR1;
	}
}

void GetRangeData2(TIM_HandleTypeDef *htim)
{
	//1. Set TRIG pin low for usec
	HAL_GPIO_WritePin(TRIG_2_GPIO_Port, TRIG_2_Pin, GPIO_PIN_RESET);
	usDelay(3);

	//2. Send quick pulse from trig pin
	HAL_GPIO_WritePin(TRIG_2_GPIO_Port, TRIG_2_Pin, GPIO_PIN_SET);
	usDelay(10);
	HAL_GPIO_WritePin(TRIG_2_GPIO_Port, TRIG_2_Pin, GPIO_PIN_RESET);

	//3. ECHO signal
	USNUM = 2;
	HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
	//Wait for IC flag
	uint32_t startTick = HAL_GetTick();
	do
	{
		if(icFlag2) break;
	}while((HAL_GetTick() - startTick) < 500);
	icFlag2 = 0;
	HAL_TIM_IC_Stop_IT(htim, TIM_CHANNEL_2);

	//4. calculate distance in cm
	if(edge2Time2 > edge1Time2)
	{
		RangeR2 = ((edge2Time2 - edge1Time2) + 0.0f)*(SpeedSound/2);
		RangeF2 = RangeR2;
		//LowPassFilter(&range1R, &range1F);
	}
	else
	{
		RangeR2 = 0.0f;
		RangeF2 = RangeR2;
	}
}

void GetRangeData3(TIM_HandleTypeDef *htim)
{
	//1. Set TRIG pin low for usec
	HAL_GPIO_WritePin(TRIG_3_GPIO_Port, TRIG_3_Pin, GPIO_PIN_RESET);
	usDelay(3);

	//2. Send quick pulse from trig pin
	HAL_GPIO_WritePin(TRIG_3_GPIO_Port, TRIG_3_Pin, GPIO_PIN_SET);
	usDelay(10);
	HAL_GPIO_WritePin(TRIG_3_GPIO_Port, TRIG_3_Pin, GPIO_PIN_RESET);

	//3. ECHO signal
	USNUM = 3;
	HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_4);
	//Wait for IC flag
	uint32_t startTick = HAL_GetTick();
	do
	{
		if(icFlag3) break;
	}while((HAL_GetTick() - startTick) < 500);
	icFlag3 = 0;
	HAL_TIM_IC_Stop_IT(htim, TIM_CHANNEL_4);

	//4. calculate distance in cm
	if(edge2Time3 > edge1Time3)
	{
		RangeR3 = ((edge2Time3 - edge1Time3) + 0.0f)*(SpeedSound/2);
		RangeF3 = RangeR3;
		//LowPassFilter(&range1R, &range1F);
	}
	else
	{
		RangeR3 = 0.0f;
		RangeF3 = RangeR3;
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (USNUM == 1){
		if(captureIdx1 == 0)
		{
			edge1Time1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

			captureIdx1 = 1;
		}
		else if(captureIdx1 == 1)
		{
			edge2Time1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			captureIdx1 = 0;
			icFlag1 = 1;
		}
	}
	else if(USNUM == 2){
		if(captureIdx2 == 0)
		{
			edge1Time2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

			captureIdx2 = 1;
		}
		else if(captureIdx2 == 1)
		{
			edge2Time2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			captureIdx2 = 0;
			icFlag2 = 1;
		}
	}
	else if(USNUM == 3){
		if(captureIdx3 == 0)
		{
			edge1Time3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

			captureIdx3 = 1;
		}
		else if(captureIdx3 == 1)
		{
			edge2Time3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
			captureIdx3 = 0;
			icFlag3 = 1;
		}
	}
}

//void GetRangeData(TIM_HandleTypeDef *htim, uint8_t *USNUM, GPIO_TypeDef *TRIG_Port, float *RangeR, float *RangeF)
//{
//	//Configure for specific sensor
//	if (*USNUM == 1){
//		//TRIG_Port = *TRIG_1_GPIO_Port;
//		TRIG_Pin = TRIG_1_Pin;
//		channel = TIM_CHANNEL_1;
//		captureIdx = &captureIdx1;
//		edge1Time = &edge1Time1;
//		edge2Time = &edge2Time1;
//		icFlag = &icFlag1;
//	}
//	else if (*USNUM == 2){
//		//TRIG_Port = *TRIG_2_GPIO_Port;
//		TRIG_Pin = TRIG_2_Pin;
//		channel = TIM_CHANNEL_2;
//		captureIdx = &captureIdx2;
//		edge1Time = &edge1Time2;
//		edge2Time = &edge2Time2;
//		icFlag = &icFlag2;
//	}
//	else if (*USNUM == 3){
//		//TRIG_Port = *TRIG_3_GPIO_Port;
//		TRIG_Pin = TRIG_3_Pin;
//		channel = TIM_CHANNEL_4;
//		captureIdx = &captureIdx3;
//		edge1Time = &edge1Time3;
//		edge2Time = &edge2Time3;
//		icFlag = &icFlag3;
//	}
//
//	//1. Set TRIG pin low for usec
//	HAL_GPIO_WritePin(TRIG_Port, TRIG_Pin, GPIO_PIN_RESET);
//	usDelay(3);
//
//	//2. Send quick pulse from trig pin
//	HAL_GPIO_WritePin(TRIG_Port, TRIG_Pin, GPIO_PIN_SET);
//	usDelay(10);
//	HAL_GPIO_WritePin(TRIG_Port, TRIG_Pin, GPIO_PIN_RESET);
//
//	//3. ECHO signal
//	HAL_TIM_IC_Start_IT(htim, channel);
//	//Wait for IC flag
//	uint32_t startTick = HAL_GetTick();
//	do
//	{
//		if(*icFlag) break;
//	}while((HAL_GetTick() - startTick) < 500);
//	*icFlag = 0;
//	HAL_TIM_IC_Stop_IT(htim, channel);
//
//	//4. calculate distance in cm
//	if(*edge2Time > *edge1Time)
//	{
//		*RangeR = ((*edge2Time - *edge1Time) + 0.0f)*(SpeedSound/2);
//		*RangeF = *RangeR;
//		//LowPassFilter(&range1R, &range1F);
//	}
//	else
//	{
//		*RangeR = 0.0f;
//		*RangeF = *RangeR;
//	}
//
//}
