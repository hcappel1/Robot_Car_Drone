/*
 * range_sensors.h
 *
 *  Created on: Mar 23, 2021
 *      Author: henca
 */

#ifndef INC_RANGE_SENSORS_H_
#define INC_RANGE_SENSORS_H_

#include "main.h"

void usDelay(uint32_t uSec);
void LowPassFilter(float *rangeR, float *rangeF);
//void GetRangeData(TIM_HandleTypeDef *htim, uint8_t *USNUM, float *RangeR, float *RangeF);
void GetRangeData1(TIM_HandleTypeDef *htim);
void GetRangeData2(TIM_HandleTypeDef *htim);
void GetRangeData3(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* INC_RANGE_SENSORS_H_ */
