/*
 * config.cpp
 *
 *  Created on: 11 авг. 2016 г.
 *      Author: kaiser
 */


#include "driverlib.h"
#include "config.h"



const int INFOA_START=0x1980;
uint8_t read_U8(uint8_t offset){
	return *(uint8_t*)(INFOA_START + offset);
}
void write_U8(uint8_t offset, uint8_t value){
	FlashCtl_write8(&value, (uint8_t*)(INFOA_START + offset),  1);
}

int get_device_id(){
	uint8_t dev_id = read_U8(0);
	uint8_t check = read_U8(1);
	if ((dev_id + check) & 0xff) {
		// memory was empty or corrupted
		return -1;
	}
	return dev_id;
}

void set_device_id(uint8_t dev_id){
	FlashCtl_unlockInfoA();	//Unlock Info Segment A
	bool status;
	do {  //Erase INFOA
		FlashCtl_eraseSegment((uint8_t*)INFOA_START);
		status = FlashCtl_performEraseCheck((uint8_t*)INFOA_START, 128);
	} while (status == STATUS_FAIL);
	write_U8(0, dev_id);
	write_U8(1, 0x100-dev_id); // control byte
	FlashCtl_lockInfoA();	//Lock Info Segment A
}
