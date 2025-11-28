#include "main.h"
/****FUNCTION PROTOTYPE***********/
static void delay_in_us(TIM_TypeDef*,uint16_t us);
/*********************************/

void dht11_init(void)
{
	//INITIALISATION OF GPIOA
	if(!(RCC->AHB1ENR & 0x1U))
	{
		RCC->AHB1ENR|=0x01U;
	}
	
	//CONFIGURATION 
	SET_PIN_MODE_OUTPUT(GPIOA,8);
	GPIOA->ODR|=BIT(8);
	HAL_Delay(2000); //wait 1 sec in order to pass the unstable status
}

uint8_t check_DHT_response(TIM_TypeDef *TIMx)
{
	//timeout value for 2sec
	volatile uint32_t timeout=333400U;
	
	/**** STARTING COMMUNICATION WITH DHT ****/
	
	// high to low to initilise communication
	GPIOA->ODR&=~BIT(8); 
	
	//wait atleast 18ms to ensure DHT�s detection of MCU's signal
	HAL_Delay(20);
	//pull-up voltage for DHT response
	GPIOA->ODR|=BIT(8); 
	
	//releasing pin for DHT response
	SET_PIN_MODE_INPUT(GPIOA,8); 
	
	//wait atleast 40us for DHT�s response
	delay_in_us(TIMx,40);
//	while(GPIOA->IDR & BIT(8));
	
	//DHT send response signal and pull-down line for 80us
	delay_in_us(TIMx,80);
	
	//DHT pulls up voltage and keeps it for 80us and prepares for data transmission
	while(GPIOA->IDR & BIT(8))
	{
		timeout--;
		if(timeout<=0)
		{
			//again MCU takes the control
			SET_PIN_MODE_OUTPUT(GPIOA,8);
			GPIOA->ODR|=BIT(8);
			return FAIL;
		}
	}
	return SUCCESS;
}
	/*******************************************************/
int8_t getHT(TIM_TypeDef *TIMx,uint8_t* data_buf,uint8_t size)
{
	uint8_t indx=0,bit=0,_8bit_value=0;
	/**** COLLECTING DATA FROM DHT *******/
	for(;indx<size;indx++)
	{
		while(bit < 8)
		{
			//every bit of data begins with the 50us low-voltage-level 
			//delay_in_us(50);
			while(!(GPIOA->IDR & BIT(8)));
		
			//the length of following high-voltage-level signal determines whether data bit is "0" or "1"
			delay_in_us(TIMx,45);
			
			// if voltage is high more than 30us then data bit is "1"
			if(GPIOA->IDR & BIT(8))
			{
				_8bit_value |= 0x1U;
				
				//wait until line is pull-down
				while(GPIOA->IDR & BIT(8));
			}
			if(bit!=7)
				_8bit_value <<=1U;
			bit++;
		}
		//resetting bit to 0
		bit=0; 
		
		//transfering the 8-bit value to data buffer
		data_buf[indx]=_8bit_value;
		
		//resetting the value
		_8bit_value=0;
	}
	//wait atleast 50us for DHT to release the line
	while(!(GPIOA->IDR & BIT(8)));
	
	//again MCU takes the control
	SET_PIN_MODE_OUTPUT(GPIOA,8);
	GPIOA->ODR|=BIT(8);
	//CHECKSUM
	if((data_buf[0]+data_buf[1]+data_buf[2]+data_buf[3])== data_buf[4])
		return SUCCESS;
	
	return FAIL;
}

static void delay_in_us(TIM_TypeDef *TIMx,uint16_t delay)
{
	TIMx->CNT=0; //resetting counter value to 0
	while(TIMx->CNT < delay);//wait until overflow event is occurs
}
