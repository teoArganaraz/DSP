/*
 * Autores: ALIAGA, Julio.
 * 			ARGAÑARAZ, Teo.
 * 			VELEZ, Nicolas.
 *
 * Objetivo:
 * Manejar el modulo del conversor A/D integrado en el MCU MK64FN1M0VLL12 de la familia Kinetis K64.
 *
 * Descripción:
 * Realizar un programa aplicativo que sea capaz de digitalizar una señal a través del modulo del ADC disponible en la board
 * FRDM-K64F a distintas velocidades de muestreo, las velocidades requeridas son 8K/S, 16K/S 22K/S, 44K/S y 48K/S. Los cambios
 * de las velocidades de muestreo serán realizados con una de las teclas de la placa de evaluación, en forma de un buffer circular.
 * Cada velocidad de muestreo se indicará a través de un color RGB del LED. Con otra tecla de la placa se habilitara la adquisición
 * o se parara la misma (Run/Stop). Los valores adquiridos serán almacenados en memoria en un buffer circular de 512 muestras del
 * tipo q15 (fraccional 15bits) y a su vez serán enviados a través del DAC (de 12bits).
 */

/* NOTAS
 * Falta implementar bien el buffer circular
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "arm_math.h"
#include "stdbool.h"


/* DEFINITIONS */
#define _8KHZ_SR 	7500   	// Ticks del PIT para un Sample Rate de 8kHz para un bus clock de 60 MHz
#define _16KHZ_SR 	3750	// Ticks del PIT para un Sample Rate de 16kHz para un bus clock de 60 MHz
#define _22KHZ_SR 	2724	// Ticks del PIT para un Sample Rate de 22kHz para un bus clock de 60 MHz
#define _44KHZ_SR 	1364	// Ticks del PIT para un Sample Rate de 44kHz para un bus clock de 60 MHz
#define _48KHZ_SR 	1250	// Ticks del PIT para un Sample Rate de 48kHz para un bus clock de 60 MHz

#define _BUFFER_SIZE			512  	// Tamaño del Buffer
#define _DAC_BASEADDR 	 		DAC0 	// Definicion del DAC
#define _DAC_BUFFER_INDEX 		0U   	// El indice del buffer es siempre cero debido a que no se usa el buffer del DAC
#define _DC_OFFSET (uint16_t)	32768	// Offset en 1.65 V


/*VARIABLES*/

/*Flags de control*/
bool RUN_FLAG = false;			// Flag de RUN de la funcionalidad RUN/STOP del SW2/GPIOC
bool BUFFER_LOAD_FLAG = false;	// Flag que indica que el buffer esta cargado por al menos una muestra del ADC

/*Variables de Flags y LEDs*/
uint8_t counter = 0;	// Variable que se modifica con el SW3/GPIOA, modifica el color del LED RGB y el Sample Rate del PIT
uint8_t toggle = 0;		// Variable que togglea entre RUN y STOP cuando se presiona el SW2/GPIOC

/*Buffer*/
q15_t buffer[_BUFFER_SIZE] = {0}; //Inicializacion del Buffer en cero
q15_t *ptrADC, *ptrDAC;  		  //Punteros del buffer



/*CODE*/

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
	uint32_t intStatus;
	/* Reading all interrupt flags of status register */
	intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
	PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

	/* Start of conversion */
	ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, ADC0_channelsConfig);
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}
/* ADC0_IRQn interrupt handler */
void ADC0_IRQHANDLER(void) {

	uint16_t result_value = 0;

	uint32_t status = ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
	if ( status == kADC16_ChannelConversionDoneFlag){
		result_value = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
	}

    if((ptrADC != &buffer[_BUFFER_SIZE]) && (RUN_FLAG == true)){
	   *ptrADC = (q15_t)(result_value - _DC_OFFSET );
	   ptrADC ++;
	   ptrDAC = ptrADC -1;
	   BUFFER_LOAD_FLAG = true;
    }

    else{
	   ptrADC = &buffer[0];
	   ptrDAC = &buffer[_BUFFER_SIZE];
    }

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/* PORTC_IRQn interrupt handler */
void GPIOC_IRQHANDLER(void) {
	/* Get pin flags */
	uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOC);

	toggle ++;
	if(toggle % 2 == 1){
			RUN_FLAG = true;
	}
	else{
			RUN_FLAG = false;
	}

	/* Clear pin flags */
	GPIO_PortClearInterruptFlags(GPIOC, pin_flags);

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}

/* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
	/* Get pin flags */
	uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);
	/* Place your interrupt code here */
	counter++;
	PRINTF("Cambio sample rate %d\r\n", counter);
	switch(counter){
	case 1:
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, _8KHZ_SR);
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		GPIO_PortClear(GPIOB, 1U << BOARD_LED_RED_GPIO_PIN);
		GPIO_PortClear(GPIOE, 1U << BOARD_LED_GREEN_GPIO_PIN);
		GPIO_PortClear(GPIOB, 1U << BOARD_LED_BLUE_GPIO_PIN);
		break;
	case 2:
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, _16KHZ_SR);
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		GPIO_PortSet(GPIOB, 1U << BOARD_LED_RED_GPIO_PIN);
		GPIO_PortSet(GPIOE, 1U << BOARD_LED_GREEN_GPIO_PIN);
		GPIO_PortClear(GPIOB, 1U << BOARD_LED_BLUE_GPIO_PIN);
		break;
	case 3:
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, _22KHZ_SR);
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		GPIO_PortSet(GPIOB, 1U << BOARD_LED_RED_GPIO_PIN);
		GPIO_PortClear(GPIOE, 1U << BOARD_LED_GREEN_GPIO_PIN);
		GPIO_PortSet(GPIOB, 1U << BOARD_LED_BLUE_GPIO_PIN);
		break;
	case 4:
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, _44KHZ_SR);
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		GPIO_PortClear(GPIOB, 1U << BOARD_LED_RED_GPIO_PIN);
		GPIO_PortClear(GPIOE, 1U << BOARD_LED_GREEN_GPIO_PIN);
		GPIO_PortSet(GPIOB, 1U << BOARD_LED_BLUE_GPIO_PIN);
		break;
	case 5:
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, _48KHZ_SR);
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		GPIO_PortClear(GPIOB, 1U << BOARD_LED_RED_GPIO_PIN);
		GPIO_PortSet(GPIOE, 1U << BOARD_LED_GREEN_GPIO_PIN);
		GPIO_PortSet(GPIOB, 1U << BOARD_LED_BLUE_GPIO_PIN);
		counter=0;
		break;
	default:
		break;
	}

	/* Clear pin flags */
	GPIO_PortClearInterruptFlags(GPIOA, pin_flags);

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}


/*
 * @brief   Application entry point.
 */
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	ptrADC = &buffer[0];
	ptrDAC = &buffer[0];

	PRINTF("Hello World\r\n");

	/* Force the counter to be placed into memory. */
	//volatile static int i = 0 ;
	/* Enter an infinite loop, just incrementing a counter. */
	while(1) {
		/*Buffer circular.*/
		while(BUFFER_LOAD_FLAG){
			DAC_SetBufferValue(_DAC_BASEADDR, 0U, (*ptrDAC+ _DC_OFFSET)>>4);
			break;
		}
	}
	return 0;
}
