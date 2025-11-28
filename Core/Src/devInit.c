#include <extra_tasks.h>
#include "main.h"
#include "image.h"
#include "dht11.h"
extern I2C_HandleTypeDef hi2c1;
extern int8_t config_flag;
#define TRIALS	3

void devInit(void)
{
	char I2C_dev_buf[20];
	int8_t wifi_status=0;

	/************************ TFT-LCD INITIALISATION **************************/
	ST7735_Init();

	/*************************** PROJECT NAME DISPLAY*******************************/
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_WriteString(30,30,"KERNEL",Font_16x26,ST7735_RED,DISABLE);
	ST7735_WriteString(21,57,"MASTERS",Font_16x26,ST7735_RED,DISABLE);
	ST7735_WriteString(50,87,"PRESENTS",Font_7x10,ST7735_TURQUOISE,DISABLE);
	HAL_Delay(3000);

	// TO ENTER TASK 5
	if(config_flag==SET)
	{
		config_flag=0;
		Task5_SWMS_Config();
	}
	ST7735_DrawImage(ST7735_XSTART, ST7735_YSTART, WIDTH,HEIGHT, (uint16_t *)image);
	HAL_Delay(20);
	ST7735_WriteString(10,30,"SMART WEATHER",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(25,50,"MONITERING",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(45,70,"SYSTEM",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(57,90,"V3.1",Font_11x18,ST7735_BLACK,DISABLE);
	HAL_Delay(3000);
	/**********************************************************************************/

	/********************************* I2C TESTCASES***********************************/
	ST7735_DrawImage(ST7735_XSTART, ST7735_YSTART, WIDTH,HEIGHT, (uint16_t *)image);
	ST7735_WriteString(46,5,"DEVICE",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(3,27,"INITIALIZATION",Font_11x18,ST7735_BLACK,DISABLE);
	drawRect(6, 50, 150, 60, ST7735_ONYX);
	ST7735_FillRectangle(7, 51, 148, 58, ST7735_WHITE);
	ST7735_WriteString(36,53,"I2C TESTCASE",Font_7x10,ST7735_ONYX,DISABLE);
	ST7735_WriteString(10,66,"SearchinG for Devices",Font_6x8,ST7735_PURPLE,DISABLE);
	HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
	HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
	HAL_Delay(2000);
	while(1)
	{
		if((HAL_I2C_IsDeviceReady(&hi2c1,RTC_ADDR,TRIALS,10))==HAL_OK)
		 {
				snprintf(I2C_dev_buf,16,"RTC Found:0x%x",0x68);
				ST7735_WriteString(10,76,I2C_dev_buf,Font_6x8,ST7735_PURPLE,DISABLE);
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				HAL_Delay(2000);
		 }
		 else
		 {

			 ST7735_WriteString(10,76,"RTC Not Found !!!",Font_6x8,ST7735_RED,DISABLE);
			 HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
			 HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
			 HAL_Delay(2000);
			 continue;
		 }
		if((HAL_I2C_IsDeviceReady(&hi2c1,EEPROM_ADDR,TRIALS,10))==HAL_OK)
		{
				snprintf(I2C_dev_buf,20,"EEPROM Found:0x%x",0x50);
				ST7735_WriteString(10,86,I2C_dev_buf,Font_6x8,ST7735_PURPLE,DISABLE);
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				HAL_Delay(2000);
				break;
		 }
		 else
		 {
			 ST7735_WriteString(10,86,"EEPROM Not Found !!!",Font_6x8,ST7735_RED,DISABLE);
			 HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
			 HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
			 HAL_Delay(2000);
			 continue;
		 }
	}
	/*******************************************************************************************/

	/****************************** DHT INITIALISATION ****************************************/
	ST7735_FillRectangle(7, 51, 148, 58, ST7735_WHITE);
	ST7735_WriteString(39,53,"DHT TESTCASE",Font_7x10,ST7735_ONYX,DISABLE);
	ST7735_WriteString(10,66,"SearchinG For DHT Sensor",Font_6x8,ST7735_PURPLE,DISABLE);
	HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
	HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
	HAL_Delay(2000);

	// DHT INTIALISATION
	dht11_init();

	if(check_DHT_response(TIM9)==FAIL)
	{
		ST7735_WriteString(10,76,"DHT NOT FOUND !!!",Font_6x8,ST7735_RED,DISABLE);
		HAL_Delay(500);
		ST7735_WriteString(10,86,"Check Device ...",Font_6x8,ST7735_RED,DISABLE);
		HAL_Delay(500);
		ST7735_WriteString(10,96,"Press Reset ...",Font_6x8,ST7735_RED,DISABLE);
		//HAL_Delay(1750);
		while(1);
	}
	ST7735_WriteString(10,76,"DHT Found....",Font_6x8,ST7735_PURPLE,DISABLE);
	HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
	HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
	HAL_Delay(2000);
	ST7735_WriteString(10,86,"Initialization Done....",Font_6x8,ST7735_DARK_GREEN,DISABLE);
	HAL_Delay(2000);
	/*******************************************************************************************/

	/****************************** WIFI INITIALISATION****************************************/
	ST7735_FillRectangle(7, 51, 148, 58, ST7735_WHITE);
	ST7735_WriteString(36,53,"WIFI TESTCASE",Font_7x10,ST7735_ONYX,DISABLE);
	ST7735_WriteString(10,66,"SearchinG for WIFI",Font_6x8,ST7735_PURPLE,DISABLE);
	ST7735_WriteString(10,76,"Module...",Font_6x8,ST7735_PURPLE,DISABLE);
	HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //set 13th bit postion to turn ON red LED
	HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //clear 13th bit postion to turn OFF green LED
	HAL_Delay(2000);
	wifi_status=wifi_init();
	if(wifi_status==WIFI_NOT_FOUND)
	{
		ST7735_WriteString(10,86,"WIFI NOT FOUND !!!",Font_6x8,ST7735_RED,DISABLE);
		HAL_Delay(500);
		ST7735_WriteString(10,96,"CHECK WIFI MODULE...",Font_6x8,ST7735_RED,DISABLE);
		HAL_Delay(500);
		while(1);
	}
	else if(wifi_status==PASSWORD_ERROR)
	{
		ST7735_FillRectangle(7, 66, 148, 43, ST7735_WHITE); // this fill doesn't clear heading
		ST7735_WriteString(10,66,"Wrong SSID/Password",Font_6x8,ST7735_RED,DISABLE);
		HAL_Delay(500);
		ST7735_WriteString(10,76,"Check & Press Reset",Font_6x8,ST7735_RED,DISABLE);
		while(1);
	}
	else if(wifi_status==SUCCESS)
	{
		ST7735_FillRectangle(7, 66, 148, 43, ST7735_WHITE); // this fill doesn't clear heading
		ST7735_WriteString(10,66,"Wifi Initialization ",Font_6x8,ST7735_DARK_GREEN,DISABLE);
		ST7735_WriteString(10,76,"Done... ",Font_6x8,ST7735_DARK_GREEN,DISABLE);
		HAL_Delay(500);
	}
	ST7735_WriteString(10,86,"All TestCases Passed..",Font_6x8,ST7735_DARK_GREEN,DISABLE);
	HAL_Delay(2000);
	ST7735_DrawImage(ST7735_XSTART, ST7735_YSTART, WIDTH,HEIGHT, (uint16_t *)image);
	ST7735_WriteString(46,33,"DEVICE",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(3,55,"INITIALIZATION",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(52,78,"DONE...",Font_11x18,ST7735_BLACK,DISABLE);
	HAL_Delay(2000);
	/*******************************************************************************/

	/******************* FOR OPERATION PURPOSE***************************/
	ST7735_DrawImage(ST7735_XSTART, ST7735_YSTART, WIDTH,HEIGHT, (uint16_t *)image);

	/************************* DRAWING TOP NOTCH***********************/
	ST7735_FillRectangle(0, 0, 160, 15, ST7735_BLACK);
	drawRect(2, 42, 155, 82, ST7735_ONYX);
	ST7735_FillRectangle(3, 43, 153, 80, ST7735_WHITE);

	/************************ OPERATION BODY ***************************/
	ST7735_WriteString(16,17,"READINGS",Font_16x26,ST7735_DARK_GREEN,DISABLE);
	ST7735_WriteString(4,45,"Temperature:",Font_11x18,ST7735_BLACK,DISABLE);
	ST7735_WriteString(4,83,"Humidity:",Font_11x18,ST7735_BLACK,DISABLE);

}

