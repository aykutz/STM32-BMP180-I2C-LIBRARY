/*
 * BMP180.h
 *
 *  Created on: Sep 3, 2024
 *      Author: AYKUT
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_
#include "stm32f4xx_hal.h"
extern I2C_HandleTypeDef hi2c1;
//FUNCTIONs
void BMP180_Init(void);
void BMP180_GetCalibration(void);
void BMP180_Get_Calibration_Values(void);
void BMP180_Get_Uncompansated_Temperature(void);
void BMP180_Get_Temperature();
void BMP180_Get_Uncompansated_Presure(void);
void BMP180_Get_Presure();
//------------------------------------------------------
//BMP180 DEVICE ADDRESS
#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF
//-----------------------------------------------------------
//BMP180 CALIBRATION VALUE LENGTH
#define BMP180_CALIBRATION_VALUE_LENGTH 22 /*11x2, 8 bit per word, but we have 16 bits*/
//-------------------------------------------------------------
//BMP180 CALIBRATION START ADDRESS
#define BMP180_CALIBRATION_START_ADDRESS 0xAA
//-----------------------------------------------------------
//TEMPERATURE VALUEs----------------------------------------
extern int16_t unCompTemp;
extern float temp;
extern int32_t tX1, tX2, tB5;
//-------------------------------------------------------
//PRESURE VALUEs----------------------------------------------
extern uint32_t unCompPresure;
extern float presure, presureATM;
extern int32_t pB6,pX1,pX2,pB3,pB4,pB7,pX3;
//-------------------------------------------------------------
// EEPROM VALUEs-----------------------------------------------
extern int16_t AC1;
extern int16_t AC2;
extern int16_t AC3;
extern int16_t AC4;
extern int16_t AC5;
extern int16_t AC6;
extern int16_t B1;
extern int16_t B2;
extern int16_t MB;
extern int16_t MC;
extern int16_t MD;
//------------------------------------------------------------



#endif /* INC_BMP180_H_ */
