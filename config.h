/*
 * config.h
 *
 *  Created on: 11 авг. 2016 г.
 *      Author: kaiser
 */

#ifndef CONFIG_H_
#define CONFIG_H_



#define MAX_TRACKERS 50

// get/set device id from flash
int get_device_id();
void set_device_id(uint8_t dev_id);



#endif /* CONFIG_H_ */
