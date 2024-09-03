/*
 * BMP180.c
 *
 *  Created on: Sep 3, 2024
 *      Author: AYKUT
 */

#include "BMP180.h"
/*EEPROM CALIBRATION*/
int16_t AC1;
int16_t AC2;
int16_t AC3;
int16_t AC4;
int16_t AC5;
int16_t AC6;
int16_t B1;
int16_t B2;
int16_t MB;
int16_t MC;
int16_t MD;
/*TEMPERATURE*/
int16_t unCompTemp;
float temp;
int32_t tX1, tX2, tB5;
/*PRESURE*/
uint32_t unCompPresure;
float presure, presureATM;
int32_t pB6,pX1,pX2,pB3,pB4,pB7,pX3;
/*---------------------------------------------------------------------------------------------------------------*/
void BMP180_Init()
{
	// IF CONNECTION IS OK, GREEN LED TURN ON. THIS MEANS NO PROBLEM, EVERYTHINK IS OK.
	if(HAL_I2C_IsDeviceReady(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 100000)==HAL_OK)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

	BMP180_Get_Calibration_Values();
}
void BMP180_Get_Calibration_Values()
{
	uint8_t calibrationBuff[BMP180_CALIBRATION_VALUE_LENGTH] = {0};

	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_CALIBRATION_START_ADDRESS, 1, calibrationBuff, BMP180_CALIBRATION_VALUE_LENGTH,10000 );

	/*shifting operations*/
	AC1 = calibrationBuff[0]<<8 | calibrationBuff[1]; //8 bit MSB shifting left(15,14,13..), 8 bit LSB stay(7,6,5,..0)
	AC2 = calibrationBuff[2]<<8 | calibrationBuff[3];
	AC3 = calibrationBuff[4]<<8 | calibrationBuff[5];
	AC4 = calibrationBuff[6]<<8 | calibrationBuff[7];
	AC5 = calibrationBuff[8]<<8 | calibrationBuff[9];
	AC6 =calibrationBuff[10]<<8 | calibrationBuff[11];
	B1 = calibrationBuff[12]<<8 | calibrationBuff[13];
	B2 = calibrationBuff[14]<<8 | calibrationBuff[15];
	MB = calibrationBuff[16]<<8 | calibrationBuff[17];
	MC = calibrationBuff[18]<<8 | calibrationBuff[19];
	MD = calibrationBuff[20]<<8 | calibrationBuff[21];

	/*CONTROL CALIBRATION VALUEs COMING FROM to EEPROM, IF THERE IS A PROBLEM, ORANGE LED WILL TURN ON*/
	/*IF ORANGE LED DOESN'T WORK, EVERYTHINK IS OK*/

	if(AC1 == 0x0000 || AC1 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}

	if(AC2 == 0x0000 || AC2 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(AC3 == 0x0000 || AC3 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(AC4 == 0x0000 || AC4 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(AC5 == 0x0000 || AC5 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(AC6 == 0x0000 || AC6 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(B1 == 0x0000 || B1 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(B2 == 0x0000 || B2 == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(MB == 0x0000 || MB == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(MC == 0x0000 || MC == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	if(MD == 0x0000 || MD == 0xFFFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
}
void BMP180_Get_Uncompansated_Temperature(void)
{
	uint8_t wData[1];
	uint8_t rData[2]={0};
	wData[0]=0x2E;
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 2, 100000);
	unCompTemp = (int16_t)((rData[0]<<8) | rData[1]); //8 bit MSB shifting left(15,14,13..), 8 bit LSB stay(7,6,5,..0)
}
void BMP180_Get_Temperature()
{
	BMP180_Get_Uncompansated_Temperature();

	//DATASHEET CALCULATING

	tX1 = ((int32_t)unCompTemp - (int32_t)AC6) * (int32_t)AC5 / 32768;
	tX2 = ((int32_t)MC * 2048) / (tX1 + (int32_t)MD);

	if(tX1==0 && MD==0) // CONTROL tx1 and md FOR 0
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1); //ERROR
		while(1);
	}


	tB5 = tX1 + tX2;
	temp = (tB5 + 8) / 16 * 0.1;

}
void BMP180_Get_Uncompansated_Presure(void)
{
	uint8_t rData[3]={0};

	uint8_t wData[1];
	wData[0]=0x34 | (0x03<<6); //oversampling_setting is ultra high resolution, it's mean 0x03 and shifting 6 bit to left
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(26); // 26 ms delay because we selected ultra high resolution(oversampling_settings)

	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 3, 100000);
	unCompPresure = (rData[0]<<16 | rData [1]<<8 | rData[2]) >> (8 - (uint8_t)(0x03));
	// shifting operation -> unCompPresure = (MSB<<16 + LSB<<8 + XLSB) >> (8-oversample_settings)
}
void BMP180_Get_Presure()
{
	BMP180_Get_Uncompansated_Presure();

	// DATASHEET CALCULATING

	pB6 = tB5 - 4000;
	pX1 = (B2 * (pB6 * pB6 / 4096)) / 2048;
	pX2 = AC2 * pB6 / 2048;
	pX3 = pX1 + pX2;
	pB3 = (((AC1 * 4 + pX3) << (uint8_t)0x03) + 2) / 4;
	pX1 = AC3 * pB6 / 8192;
	pX2 = (B1 * (pB6 * pB6 / 4096)) / 65536;
	pX3 = ((pX1 + pX2) + 2) /4;
	pB4 = AC4 * (unsigned long)(pX3 + 32768) / 32768;
	pB7 = ((unsigned long)unCompPresure - pB3) * (50000 >> (uint8_t)0x03);

	if(pB7 < 0x8000000)
		presure = (pB7 * 2) / pB4;
	else
		presure = (pB7 / pB4) * 2;

	pX1 = (presure / 256) * (presure / 256);
	pX1 = (pX1 * 3038) / 65536;
	pX2 = (-7357 * presure) / 65537;
	presure = presure + (pX1 + pX2 + 3791) / 16; //HECTOPASCAL
	presureATM = presure * 0.00000986923267; //HECTOPASCAL TO ATM
}

























