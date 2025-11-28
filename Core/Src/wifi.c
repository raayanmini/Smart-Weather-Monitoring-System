
#include "main.h"
#include <stdio.h>
#include <string.h>
#define RX_BUF_SIZE 630

uint8_t rx_buffer[RX_BUF_SIZE];
short timeout_flag=0;
extern UART_HandleTypeDef huart6;
int wifi_flag=0;

#define WIFI_SSID	"No Devices Found..."
#define WIFI_PASSWORD	"24681012"

short int wifi_feedback(char sub[])
{
	HAL_UART_Receive(&huart6,rx_buffer,1000,1000);
	register int i;
    for (i=0; i<RX_BUF_SIZE; i++)
    {
        //logic to find substring in string
        if (sub[0]==rx_buffer[i]) 
        {
            register int s1Ind=1,strInd=i+1;
            while (sub[s1Ind]) 
            {
                if (sub[s1Ind]!=rx_buffer[strInd]) 
                {
                    break;
                }
                else 
                {
                    s1Ind++;
                    strInd++;
                }
            }
            if(!sub[s1Ind])
            {   
                return SUCCESS;
            }
        }
    }
		return FAIL;
}




int wifi_init(void)
{	
	int test=1;
	int count=1;
	while(1)
	{
		if(test==1)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT\r\n",sizeof("AT\r\n"),100); //basic communication
			wifi_flag=wifi_feedback("OK");
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				KM_LCD_Write_Cmd(0xCA);
				KM_LCD_Write_Str("      ");
				KM_LCD_Write_Cmd(0xc0);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
				test++;
				count=1;
			}
			else if(wifi_flag==FAIL)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
				KM_LCD_Write_Cmd(0xCA);
				KM_LCD_Write_Str("T.O: ");
				KM_LCD_Write_Cmd(0xcF);
				KM_LCD_Write_Data(count + '0');
				HAL_Delay(1000);
				count++;
				
			}
			if(count==5)
			{
				count=0;
				return WIFI_NOT_FOUND;
			}
			HAL_Delay(500);			
		}
		if(test==2)
		{			
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+RST\r\n",sizeof("AT+RST\r\n"),100);
			wifi_flag=wifi_feedback("WIFI CONNECTED");
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				KM_LCD_Write_Cmd(0xc1);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
				test=5;
				if(test==5)
					KM_LCD_Write_Str("..");
			}
			else if(wifi_flag==FAIL)
			{
				KM_LCD_Write_Data('.');
				test++;
			}
			
		}	
		if(test==3)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CWMODE=3\r\n",sizeof("AT+CWMODE=3c\r\n"),100);
			HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
			KM_LCD_Write_Cmd(0xc2);
			KM_LCD_Write_Str(".");
			HAL_Delay(500);
			test++;
		}	
		if(test==4)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWORD"\"\r\n",sizeof("AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWORD"\"\r\n"),100);
			wifi_flag=wifi_feedback("OK");		
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				KM_LCD_Write_Cmd(0xCA);
				KM_LCD_Write_Str("      ");
				KM_LCD_Write_Cmd(0xc3);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
				test++;
			}
			else if(wifi_flag==FAIL)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
				KM_LCD_Write_Cmd(0xCA);
				KM_LCD_Write_Str("T.O: ");
				KM_LCD_Write_Cmd(0xCF);
				KM_LCD_Write_Data(count + '0');
				HAL_Delay(1000);
				count++;
			}
		  if(count==5)
			{
				count=0;
				return PASSWORD_ERROR;
			}	
		}
		if(test==5)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIFSR\r\n",sizeof("AT+CIFSR\r\n"),100);
			wifi_flag=wifi_feedback("OK");		
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				KM_LCD_Write_Cmd(0xc4);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
				test++;
			}
		}
		if(test==6)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIPMUX=0\r\n",sizeof("AT+CIPMUX=0\r\n"),100);
			wifi_flag=wifi_feedback("OK");		
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
				KM_LCD_Write_Cmd(0xc5);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
			  test++;
			}
		}
		
		if(test==7)
		{
			HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIPMODE=0\r\n",sizeof("AT+CIPMODE=0\r\n"),100);
			wifi_flag=wifi_feedback("OK");		
			if(wifi_flag==SUCCESS)
			{
				HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
				HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED

				KM_LCD_Write_Cmd(0xc6);
				KM_LCD_Write_Str(".");
				HAL_Delay(500);
				return SUCCESS;
			}
		}
	}
}

int8_t checkInt(void)
{
	HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIPSTATUS\r\n",sizeof("AT+CIPSTATUS\r\n"),100);
	wifi_flag=wifi_feedback("STATUS:5"); //STATUS:5 -> DISCONNECTION
	if(wifi_flag==SUCCESS)
	{
		HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
		HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
		return STATUS_ERROR;
	}
	else if(wifi_flag==FAIL)
	{
		HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
		HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
	}
	return SUCCESS;
}

int wifi_transmit(int temp_value)
{
	char Fetch[50];
	HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n",sizeof("AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n"),100);
	osDelay(100);

	HAL_UART_Transmit(&huart6,(uint8_t *)"AT+CIPSEND=42\r\n",sizeof("AT+CIPSEND=42\r\n"),100);

	sprintf(Fetch,"GET /page.php?temp=%d&hum=25&dev=2\r\n\r\n", temp_value); //ESP8266_SendTcp
	osDelay(100);

	HAL_UART_Transmit(&huart6,(uint8_t *)Fetch,100,100);
	return SUCCESS;
}

