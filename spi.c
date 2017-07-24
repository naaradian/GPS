//   initialization waits for DCO to stabilize against ACLK.
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2

#include <msp430.h>
#include "deffs.h"



#ifdef SPISIML
unsigned int spitimer;
#ifdef SPI_DMA
unsigned char MST_Data,SLV_Data;

char SPI_RX_Buff0[SPI_RX_BUFF_SIZE];
char SPI_RX_Buff1[SPI_RX_BUFF_SIZE];
unsigned short spi_rcv_cnt;
char spi_dma_rxbuf_flag;

void spi_clear_buff(char * Buff, unsigned short len)
{
    while(len--)
  {
       SBAdd(*Buff);
        Buff++;
  }
}

void spi_dma_rcv(void)
{
    unsigned short len;
    unsigned short val;
   DMA2CTL &= ~(DMAEN); //170412
    val = DMA2SZ;
    DMA2CTL |= DMAEN;
    if(val < SPI_RX_BUFF_SIZE)
    {
        DMA2CTL &= ~(DMAEN);
        if(!spi_dma_rxbuf_flag)
        {
            __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) &SPI_RX_Buff1[0]);
            spi_dma_rxbuf_flag = 1;
        }
        else
        {
            __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) &SPI_RX_Buff0[0]);
             spi_dma_rxbuf_flag = 0;
        }
        DMA2SZ = (unsigned short)SPI_RX_BUFF_SIZE;
        DMA2CTL |= DMAEN;
        len = (unsigned short)SPI_RX_BUFF_SIZE - val; //4 -for test
        spi_dma_rxbuf_flag ? spi_clear_buff(&SPI_RX_Buff0[0], len) : spi_clear_buff(&SPI_RX_Buff1[0], len);
     }
}
#endif


void spi_task(void)
{
    if(spitimer < SPI_TIME) return;
    spitimer = 0;

#ifdef SPI_DMA
   spi_dma_rcv();
#endif
 //  spi_transmit(); //to simulate gps data!!!
   SParseBuffer(0); //test receive
}
#endif

void spi_init(void)
{
printf("\n\r spi init\n\r");

#ifdef SPISIML
  SBInit();
#endif

#ifndef GPS_PLATA

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs  
  P1MAP3 = PM_UCB0SIMO;                     // Map UCB0SIMO  to P1.3
  P1MAP2 = PM_UCB0SOMI;                     // Map UCB0SOMI to P1.2
  P1MAP4 = PM_UCB0CLK;                      // Map UCB0CLK output to P1.4
  PMAPPWD = 0;                              // Lock port mapping registers  
   
  P1DIR |= BIT2; //BIT2;            // ACLK, MCLK, SMCLK set out to pins
  P1SEL |= BIT3 + BIT2 + BIT4;   // debugging purposes.

  #else
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
    P1MAP3 = PM_UCB0SIMO;                     // Map UCB0SIMO  to P1.3
    P1MAP2 = PM_UCB0SOMI;                     // Map UCB0SOMI to P1.2
    P1MAP4 = PM_UCB0CLK;                      // Map UCB0CLK output to P1.4
    PMAPPWD = 0;                              // Lock port mapping registers

    P1DIR |= BIT2; //BIT2;            // ACLK, MCLK, SMCLK set out to pins
    P1SEL |= BIT3 + BIT2 + BIT4;   // debugging purposes.

  /*


  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
    P2MAP1 = PM_UCA0SIMO;                     // Map UCA0SIMO output to P2.0
    P2MAP3 = PM_UCA0SOMI;                     // Map UCA0SOMI output to P2.2
    P2MAP5 = PM_UCA0CLK;                      // Map UCA0CLK output to P2.4
    PMAPPWD = 0;                              // Lock port mapping registers

   P2DIR |= BIT1 + BIT3 + BIT5;              // ACLK, MCLK, SMCLK set out to pins
   P2SEL |= BIT1 + BIT3 + BIT5;              // P2.0,2,4 for debugging purposes.
   */
#endif
  UCB0CTL1 |= UCSWRST;  // **Put state machine in reset**

#ifdef MASTER
#ifndef  SPI_POLARITY_LOW
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
#else
  UCB0CTL0 |= UCMST+UCSYNC+UCMSB;    // 3-pin, 8-bit SPI master
#endif

#else
#ifndef  SPI_POLARITY_LOW
  UCB0CTL0 = UCSYNC+UCCKPL+UCMSB;               //slave
#else
  UCB0CTL0 = UCSYNC + UCMSB;               //slave
#endif
#endif
                                           // Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 2;//500 kHz
//  UCB0BR0 = 4; //100 kHz
  UCB0BR1 = 0;
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


#ifdef SPISIML
   UCB0STAT |= UCLISTEN;  //temporary loop
#endif


#ifdef SPISIML

#ifdef SPI_DMA
   DMACTL1 |= DMA2TSEL_18;                       //UCB0 receive ?
   DMACTL4 |= ROUNDROBIN;
  __data16_write_addr((unsigned short) &DMA2SA,(unsigned long) &UCB0RXBUF);
  __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) &SPI_RX_Buff0[0]);
   DMA2SZ = (unsigned short)SPI_RX_BUFF_SIZE;                               // Block size
   spi_dma_rxbuf_flag = 0;
   spi_rcv_cnt = 0;
   DMA2CTL = DMADSTINCR_3+DMASBDB+DMALEVEL+DMADT_0+DMAEN;  // dst increment  ???
  MST_Data = 0x00;                          // Initialize data values
  SLV_Data = 0x00;                          //
#endif

#endif

#ifdef SPI_DMA_TX
  DMACTL0 = DMA0TSEL_19;        //  UCB0TXIFG
  DMACTL4 |= ROUNDROBIN;
  __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &UCB0TXBUF);
  DMA0CTL = DMASRCINCR_3+DMASBDB+DMALEVEL;  // src increment
//  UCB0IE |= UCTXIE;
#endif

#ifdef SPISIML

#ifndef SPI_DMA
  UCB0IE |= UCRXIE;   //without this do not receive?
#endif
#endif

}

#ifdef SPI_DMA_TX

void dma_spi_send(char * Buff, unsigned short len)
{
    static char SpiTxBuff[ONLINE_DATA_SIZE << 1];  //can be maked modify
    memcpy(SpiTxBuff, Buff, len);
//    printf("\n\r%d>", DMA0SZ);
    DMA0CTL &=~(DMAEN);
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &SpiTxBuff);
//t    DMA0SZ = len;
    DMA0SZ = 40;
    DMA0CTL |= DMAEN;
}
#endif

#ifdef SPISIML

char CheckCrc(char * Buff, int len)
{
 char ret = 0;
 int i;
 unsigned char crc= 0;
//printf("\n\r");
 for(i = 1; i < len - 2; i++)
 {
     crc+=Buff[i];
 //    printf(" %d", Buff[i]) ;
 }
 //printf(" %d", Buff[i]) ;
 crc -=1;
 if(Buff[len-2] == crc) {ret = 1;}
// else {
  //   printf( "\n\r Buff = %d  crc = %d \n\r" , Buff[len-2] , crc);
  // for(i = 1; i < len - 2; i++)
   //  {
    //      printf(" %d", Buff[i]);
    // }
    // printf(" %d", Buff[i]) ;
 // }

 return ret;
}

#endif

void AddCrc(char * Buff, int len)
{
 int i;
 unsigned char crc= 0;
 for(i = 1; i < len - 2; i++) //Buff[0] do not used (= $)
 {
     crc+=Buff[i];
 }
 crc -=1;
 Buff[len-2] = crc;
}

#ifdef SPISIML

void spi_transmit(void)
{

 // char TestData1[] ={'$','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','l','m','n','o','p','q','r','*'};
#ifndef USE_SERIAL_GPS
char TestData1[] ={'$','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','l','m','n','o','p','q','r','*'};
#else
char TestData1[] ={'$','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','l','m','n','o','p','q','r','s','t','*'};
#endif



#ifndef SEND_6
  long dt = 1492420083;
  double lat = 59.934280;
  double lon = 30.335099;
//  long course = 12345678;
//  long speed = 87654321;
   memcpy(&TestData1[1], &dt, sizeof(long));
   memcpy(&TestData1[1 + sizeof(long)], &lat, sizeof(double));
   memcpy(&TestData1[1 + sizeof(long) + sizeof(double)], &lon, sizeof(double));
#ifdef USE_SERIAL_GPS
 //    printf("\n\rs> %x  %x \n\r", serial >> 8, serial & 0xff);
     TestData1[ONLINE_DATA_SIZE - 4] = serial >> 8;
     TestData1[ONLINE_DATA_SIZE - 3] = serial;
#endif
#else
    long dt = 1492420083;
    long lat = 59934280;
    long lon = 30335099;
    long alt = 10;
    long course = 12345678;
    long speed = 87654321;

     memcpy(&TestData1[1], &dt, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)]  , &lat, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*2], &lon, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*3], &alt, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*4], &course, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*5], &speed, sizeof(long));
#ifdef USE_SERIAL_GPS
   //  printf("\n\rs> %x  %x \n\r", serial >> 8, serial & 0xff);
     TestData1[ONLINE_DATA_SIZE - 4] = serial >> 8;
     TestData1[ONLINE_DATA_SIZE - 3] = serial;
#endif
#endif


   spi_send(TestData1, ONLINE_DATA_SIZE);
}

#endif

void spi_send(char* TestData1, int size)
{
   int i;
#ifdef SPI_DMA_TX
   char TestData2[ONLINE_DATA_SIZE << 1];
   int j = 1;
#endif
   AddCrc( TestData1, size);
#ifdef SPI_POLARITY_LOW
                    toggle(2, 2);
#endif
#ifndef MASTER
                    toggle(2, 3);
                    toggle(2, 4);
#endif

#ifdef SPI_DMA_TX
   TestData2[0] = TestData1[0];
   for(i = 1; i < size; i++) //t
    {
       switch(TestData1[i])
      {
      case '\\':  TestData2[j++] = TestData1[i];
                  TestData2[j++] = '0';  break;
      case '$' :  TestData2[j++] = '\\';
                  TestData2[j++] = '1';   ; break;
      default :
                  TestData2[j++] = TestData1[i];                 // Send next value
      }
   }
   dma_spi_send(TestData2, j);
#else

   UCB0TXBUF = TestData1[0];
   while (!(UCB0IFG&UCTXIFG));
   for(i = 1; i < size; i++) //t
   {
     while (!(UCB0IFG&UCTXIFG));
     switch(TestData1[i])
     {
     case '\\':  UCB0TXBUF = TestData1[i];
                  while (!(UCB0IFG&UCTXIFG));
                  __delay_cycles(40);
                  UCB0TXBUF = '0';  break;
     case '$' :   UCB0TXBUF = '\\';
                     while (!(UCB0IFG&UCTXIFG));
                   __delay_cycles(40);
                  UCB0TXBUF = '1';  break;
     default :
                 UCB0TXBUF = TestData1[i];                 // Send next value
     }
      __delay_cycles(40);
    }

#endif
}

void ToGSM(unsigned char coord, long dt, long lat, long lon, long alt, long course, long speed)
{
 //  static int t = 0;
//   t += 1;

#ifndef USE_SERIAL_GPS
char TestData1[] ={'$','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','l','m','n','o','p','q','r','*'};
if(!coord) return;
#else
char TestData1[] ={'$','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','l','m','n','o','p','q','r','s','t','*'};
if(!coord) TestData1[ONLINE_DATA_SIZE - 1] = '+';
#endif

// t+= 1;

#ifndef SEND_6
    //  long dt = 1492420083;
    double dlat = (double)lat/(double)1000000l;
    double dlon = (double)lon/(double)1000000l;
  //  long course = 12345678;
  //  long speed = 87654321;
 //   dt++;
   memcpy(&TestData1[1], &dt, sizeof(long));
   memcpy(&TestData1[1 + sizeof(long)], &dlat, sizeof(double));
   memcpy(&TestData1[1 + sizeof(long) + sizeof(double)], &dlon, sizeof(double));
//        memcpy(&TestData1[1], &t, sizeof(long)); //t170630
#ifdef USE_SERIAL_GPS
  //   printf("\n\rs> %x  %x \n\r", serial >> 8, serial & 0xff);
     TestData1[ONLINE_DATA_SIZE - 4] = serial >> 8;
     TestData1[ONLINE_DATA_SIZE - 3] = serial;
#endif
//     memcpy(&TestData1[1 + sizeof(long) + (sizeof(double) << 1) ], &serial, sizeof(unsigned short));

#else
     memcpy(&TestData1[1], &dt, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)]  , &lat, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*2], &lon, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*3], &alt, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*4], &course, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*5], &speed, sizeof(long));
     memcpy(&TestData1[1 + sizeof(long)*6], &serial, sizeof(unsigned short));
#ifdef USE_SERIAL_GPS
     TestData1[ONLINE_DATA_SIZE - 4] = serial >> 8;
     TestData1[ONLINE_DATA_SIZE - 3] = serial;
#endif //use_serial_gps

#endif //send_6

     spi_send(TestData1, ONLINE_DATA_SIZE);
}

#ifdef SPISIML
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
   SBAdd(UCB0RXBUF);
}
#endif
