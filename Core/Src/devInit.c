#include "main.h"
extern I2C_HandleTypeDef hi2c1;

void devInit(void)
{
	char I2c_Number_Buf[10];
	int8_t wifi_status=0;
	KM_LCD_Init();
	HAL_Delay(1000);
	KM_LCD_Write_Cmd(0x81);
	KM_LCD_Write_Str("KERNEL MASTERS");
	HAL_Delay(3000);
	KM_LCD_Write_Cmd(0x01); //CLEAR SCREEN CMD
	KM_LCD_Write_Cmd(0x81);
	KM_LCD_Write_Str("SMART WEATHER");
	KM_LCD_Write_Cmd(0xC0);
	KM_LCD_Write_Str("MONITERING V3.0");
	HAL_Delay(3000);
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Cmd(0x80);

	KM_LCD_Write_Str("RTC TESTCASE");
	HAL_Delay(1000);
	while(1)
	{
		if((HAL_I2C_IsDeviceReady(&hi2c1,(0x68<<1),1,10))==HAL_OK)
		 {
				KM_LCD_Write_Cmd(0xC0);
				//KM_LCD_Write_Str("                ");
				sprintf(I2c_Number_Buf,"%d",0x68);
				KM_LCD_Write_Str("DS1307 Found:");
				KM_LCD_Write_Str(I2c_Number_Buf);
				HAL_Delay(2000);
				break;
		 }
		 else
		 {
			 KM_LCD_Write_Cmd(0xC1);
			 KM_LCD_Write_Str("                ");
			 KM_LCD_Write_Str("RTC Not Found:");
			 HAL_Delay(2000);
			 continue;
		 }
	}
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str("WIFI INIT...");
	HAL_Delay(500);

	wifi_status=wifi_init();
	if(wifi_status==WIFI_NOT_FOUND)
	{
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Cmd(0x80);
		KM_LCD_Write_Str("WIFI NOT FOUND");
		HAL_Delay(500);
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Cmd(0x80);
		KM_LCD_Write_Str("CHECK WIFI");
		KM_LCD_Write_Cmd(0xC9);
		KM_LCD_Write_Str("MODULE");
		HAL_Delay(500);
		while(1);
	}
	else if(wifi_status==PASSWORD_ERROR)
	{
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Cmd(0x80);
		KM_LCD_Write_Str("Wrong Name/Pass");
		KM_LCD_Write_Cmd(0xc0);
		KM_LCD_Write_Str("Check & Reset");
		HAL_Delay(500);
		while(1);
	}
	else if(wifi_status==SUCCESS)
	{
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Cmd(0x80);
		KM_LCD_Write_Str("WIFI Init Done!!");
		HAL_Delay(2000);
		KM_LCD_Write_Cmd(0x01);
	}

		/*display format date & time*/
		KM_LCD_Write_Cmd(0x83);
		KM_LCD_Write_Str(":");
		KM_LCD_Write_Cmd(0x86);
		KM_LCD_Write_Str(":");
		KM_LCD_Write_Cmd(0xC3);
		KM_LCD_Write_Str("/");
		KM_LCD_Write_Cmd(0xC6);
		KM_LCD_Write_Str("/");

		/*display format temperature*/
		KM_LCD_Write_Cmd(0x8A);
		KM_LCD_Write_Str("T:");
		KM_LCD_Write_Cmd(0x8E);
		KM_LCD_Write_Data(223);
		KM_LCD_Write_Str("C");
}
