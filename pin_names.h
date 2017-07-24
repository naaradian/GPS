#ifndef PIN_NAMES_H_
#define PIN_NAMES_H_

	#include "driverlib.h"

	#define PINS_I2C  		GPIO_PORT_P1, GPIO_PIN2 + GPIO_PIN3

	#define PIN_GPS_ON_OFF 	GPIO_PORT_P5, GPIO_PIN6		// надо замкнуть JP11
	// #define PIN_GPS_Reset 	GPIO_PORT_P5, GPIO_PIN6  	// надо замкнуть JP1

	// управление усилителем
	#define PIN_PA_EN 	GPIO_PORT_P2,  GPIO_PIN6
	#define PIN_LNA_EN 	GPIO_PORT_P2,  GPIO_PIN5
	#define PIN_HGM 	GPIO_PORT_P2,  GPIO_PIN4


	//#define LED_1 	GPIO_PORT_P2,  GPIO_PIN0
	//#define LED_2 	GPIO_PORT_P2,  GPIO_PIN1

	#define LED_1 	GPIO_PORT_P2,  GPIO_PIN1
	#define LED_2 	GPIO_PORT_P2,  GPIO_PIN2
    #define LED_3   GPIO_PORT_P2,  GPIO_PIN3

	#define BTN_K1  GPIO_PORT_P2,  GPIO_PIN5
	#define BTN_K2  GPIO_PORT_P3,  GPIO_PIN0


	#define PIN_1PPS 	GPIO_PORT_P2,  GPIO_PIN0

	#define PIN_JP4 	GPIO_PORT_P4,  GPIO_PIN0 // need pull-up


	#define EVM_LED_G 	GPIO_PORT_P1,  GPIO_PIN0  // (при замкнутом JP5 )
	#define EVM_LED_R 	GPIO_PORT_P3,  GPIO_PIN6  // (при замкнутом JP10)


#endif /* PIN_NAMES_H_ */
