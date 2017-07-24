/*
 * deffs.h
 *
 *  Created on: 6 апр. 2017 г.
 *      Author: user
 */
#ifndef DEFFS_H_
#define DEFFS_H_

#include <string.h>
#include <stdlib.h>
//#define SEND_6


#define DPRINT

#ifdef DPRINT
#include <stdio.h>
#endif

#define USE_SERIAL_GPS

//#define SPI_POLARITY_LOW


//#define GPS_PLATA // old tracker!!!!!

//#define SPISIML //spi channel
//#define SPI_DMA //reseive side for test

#define SPI_DMA_TX
#define MASTER

#define SPI_RX_BUFF_SIZE (100)
#define SRCV_BUFF_SIZE   (100)

#ifndef USE_SERIAL_GPS
#define ONLINE_DATA_SIZE (27) //size of one message in future format with $ and *
#else
#define ONLINE_DATA_SIZE (29) //size of one message in future format with $ and *
#endif

extern void SBAdd(char);
extern void SBInit(void);
extern char SParseBuffer(char type);


#define SPI_TIME  (2)

//extern void spi_task(void);
extern void spi_receive(void);
extern void spi_transmit(void);
extern void spi_send(char* ,int);
extern void spi_dma_rcv(void);
extern char SParseBuffer(char);
extern char CheckCrc(char *, int);
extern unsigned int spitimer;
extern unsigned short serial;
extern void toggle(unsigned char, unsigned char);

#endif /* DEFFS_H_ */
