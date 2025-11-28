#include <stdint.h>

#define BIT(x)	(1U<<x)

/*********FUNCTION PROTOTYPES********/

/*@brief: initialise DHT11 sensor (basically initialising and configuring data pin for DHT11)
 * Function: dht11_init
 * Arguments:void
 * Return: void
 */
void dht11_init(void);

/* @brief: get temperature and humidity value from DHT11 sensor
 * Function: receives data from DHT Sensor
 * Arguments:TIMx: pointer to a TIM9_Typedef structure that contains the configuration information for the specified Timer.
			 data_buffer: pointer points to buffer where DHT11 data will be stored
			 Size: holds size of the buffer
 * Return: STATUS: SUCCESS OR FAIL
 */
int8_t getHT(TIM_TypeDef *TIMx,uint8_t* data_buffer, uint8_t Size);

/* @brief: get temperature and humidity value from DHT11 sensor
 * Function: Establish connection with DHT11 sensor
 * Arguments:TIMx pointer to a TIM9_Typedef structure that contains the configuration information for the specified Timer.
 * Return: STATUS: SUCCESS OR FAIL
 */
uint8_t check_DHT_response(TIM_TypeDef *TIMx);



/************ PRIVATE MACROS*********/

#define SET_PIN_MODE_OUTPUT(GPIOx,PIN)  GPIOx->OTYPER &=~BIT(8);\
										GPIOx->MODER |= (0x01U << (2*PIN))


#define SET_PIN_MODE_INPUT(GPIOx,PIN)	GPIOx->MODER &= ~(0x01U << (2*PIN));\
										GPIOx->OTYPER |=BIT(8);\
										GPIOx->PUPDR |= (0x1U << (2*PIN))

/************************************/
