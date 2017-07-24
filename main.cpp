#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pin_names.h"
#include "driverlib.h"
#include "mmc.h"

extern "C" void spi_init(void);
extern "C" void spi_task(void);
extern "C" void ToGSM(unsigned char coord ,long dt,long  lat,long  lon,long  alt,long  course,long  speed);
extern unsigned int spitimer;

// #define ORIGIN_GPS
#define SEND_BY_SPI


// #define RMC_DEBUG

#define CYCLES_PER_US 1    // for 1MHz (default)
#define CYCLES_PER_MS 1000*(CYCLES_PER_US)

#define wait_us(us) __delay_cycles((CYCLES_PER_US)*(us))
#define wait_ms(ms) for (int __t=0; __t<(ms); ++__t) wait_us(1000);

#define UART_BUFFER_SIZE 150
int uart_rx_data_size = 0;
uint8_t uart_rx_data[UART_BUFFER_SIZE];
bool start_sentence = false;

const int SECTOR_SIZE = 512;
char sector_buffer[SECTOR_SIZE];
long course = 0, speed = 0;
long lat_prev = 0, lon_prev = 0, alt_prev = 0, course_prev = 0, speed_prev = 0;
long dt_prev = 0, dt = 0, lat = 0, lon = 0, alt = 0;

struct Header {
    unsigned long sector;
    unsigned int index;
    char magic[505 - 2 - 2 - 2 - 4];
    unsigned int empty_gga_in_session;
    unsigned int restarts;
    unsigned int mmc_error;
    uint32_t device_id; // 4 bytes
};

struct Header g_header;
const char MAGIC_BYTE = '$';

char mmc = MMC_SUCCESS;
unsigned long SECTORS_COUNT = 0;

bool has_date = false;
struct tm ttime;
unsigned long last_dt = 0;
int seconds = 0;
// bool flush_needed = false;
extern "C" void toggle(unsigned char a ,unsigned char b)
{
GPIO_toggleOutputOnPin(a,b);
}

#define BUTTON BIT0

void SPI_selectGSM()
{
    //Config "Chip Select" for SPI
    P2OUT |= BIT1;

    //ON test LED
    P2OUT |= BIT0;              // Toggle P1.0 using exclusive-OR
}

void SPI_DEselectGSM()
{
    //Config "Chip Select" for SPI
    P2OUT &= ~BIT1;


    //OFF test LED
    P2OUT &= ~BIT0;              // Toggle P1.0 using exclusive-OR
}

//Send one byte via SPI
void MY_spiSendByte(const unsigned char data)
{
  while (!(UCB0IFG&UCTXIFG)); // wait while USCI_A0 TX buffer not ready?
  UCB0TXBUF=data;             // Transmit first character
}

//Send a frame of bytes via SPI
unsigned char MY_spiSendFrame(unsigned char* pBuffer, unsigned int size)
{
  unsigned long i = 0;


  SPI_selectGSM();//


// SW Delay
i = 50;

    do i--;
    while(i != 0);

  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++)
  {
      MY_spiSendByte(pBuffer[i]);     // write
  }



  // SW Delay
  i = 50;

        do i--;
        while(i != 0);

  SPI_DEselectGSM();


  return(0);
}

typedef struct _a{
    char start;
    uint64_t t;
    double x;
    double y;
    char stop;

} __attribute__((packed)) type_packet;
type_packet GPS_packet;

void send_by_spi(uint64_t dt, double lat, double lon) {
    printf("send_by_spi - begin\n");

    //config spi pin
      PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
      P1MAP3 = PM_UCB0SIMO;                     // Map UCA0SIMO output to P2.0
      P1MAP2 = PM_UCB0SOMI;                     // Map UCA0SOMI output to P2.2
      P1MAP7 = PM_UCB0CLK;                      // Map UCA0CLK output to P2.4
      PMAPPWD = 0;                              // Lock port mapping registers

      // Init Port for MMC (default high)
      P1DIR |= BIT3 + BIT2 + BIT7;              // ACLK, MCLK, SMCLK set out to pins
      P1SEL |= BIT3 + BIT2 + BIT7;              // P2.0,2,4 for debugging purposes.

                    // ACLK, MCLK, SMCLK set out to pins
      //Enable Pull-up resistor
      //1) Resistor on (write 1)
      P1REN |= BIT3 + BIT2 + BIT7;
      //2) Select pull-up (write 1)
      P1OUT |= BIT3 + BIT2 + BIT7;

        //halSPISetup
      UCB0CTL0 = UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit
      UCB0CTL0 |= UCMST;                   // SPI master
      UCB0CTL1 = UCSSEL_2+UCSWRST;              // SMCLK
      //Set bit rate
      UCB0BR0 |= 0x01;                          // UCLK/2
      UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCTXIE;                         // Enable USCI_B0 RX interrupt


    GPS_packet.start = '$';
    GPS_packet.t = dt;
    GPS_packet.x = lat;
    GPS_packet.y = lon;
    GPS_packet.stop = '*';

    MY_spiSendFrame((unsigned char*)&GPS_packet, 26);
    P2OUT ^= BIT0;

    mmc = mmcInit();

    printf("send_by_spi - end\n");
}

int atoin(const char* s, int n){
    // atoi from first n chars in string
    char buf[10];
    memcpy(buf, s, n);
    buf[n] = '\0';
    return atoi(buf);
}

char _mmcWriteSector(const unsigned long address, void *buffer) {
    unsigned char buf[512];
    memcpy(&buf[0], buffer, 512);
    return mmcWriteSector(address, &buf[0]);
}

// CONTROLLER ENCODE -----------------------------------------------------------
#pragma pack(push, 1)
struct Controller_ControlFrame {
    long dt, lat, lon, alt, course, speed; // 6*4 bytes
};
struct Controller_DiffFrame {
    char dlon, dlat, dalt, dcourse, dspeed; // 5*1 bytes
};
struct Controller_byte {
    unsigned char a : 1, b : 1, c : 1, d : 1, e : 1, f : 1, g : 1, h : 1;
};
struct Controller_Header {
    Controller_byte hdr[13]; // 13 bytes
};
union Controller_Converter {
    unsigned char val;
    Controller_byte bval;
};
#pragma pack(pop)

Controller_Header ch;
int buffer_offset = sizeof(Controller_Header);

/*void SPI_selectGSM()
{
    //Config "Chip Select" for SPI
    P2OUT |= BIT1;

    //ON test LED
    P2OUT |= BIT0;              // Toggle P1.0 using exclusive-OR
}*/

/*void SPI_DEselectGSM()
{
    //Config "Chip Select" for SPI
    P2OUT &= ~BIT1;


    //OFF test LED
    P2OUT &= ~BIT0;              // Toggle P1.0 using exclusive-OR
}*/

//Send one byte via SPI
/*void MY_spiSendByte(const unsigned char data)
{
  while (!(UCB0IFG&UCTXIFG)); // wait while USCI_A0 TX buffer not ready?
  UCB0TXBUF=data;             // Transmit first character
}*/

//Send a frame of bytes via SPI
/*unsigned char MY_spiSendFrame(unsigned char* pBuffer, unsigned int size)
{
  unsigned long i = 0;


  SPI_selectGSM();//


// SW Delay
i = 50;

    do i--;
    while(i != 0);

  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++)
  {
      MY_spiSendByte(pBuffer[i]);     // write
  }



  // SW Delay
  i = 50;

        do i--;
        while(i != 0);

  SPI_DEselectGSM();


  return(0);
}*/

/*typedef struct _a{
    char start;
    uint64_t t;
    double x;
    double y;
    char stop;

} __attribute__((packed)) type_packet;
type_packet GPS_packet;*/

/*void send_by_spi(uint64_t dt, double lat, double lon) {
    printf("send_by_spi - begin\n");

    //config spi pin
      PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
      P1MAP3 = PM_UCB0SIMO;                     // Map UCA0SIMO output to P2.0
      P1MAP2 = PM_UCB0SOMI;                     // Map UCA0SOMI output to P2.2
      P1MAP7 = PM_UCB0CLK;                      // Map UCA0CLK output to P2.4
      PMAPPWD = 0;                              // Lock port mapping registers

      // Init Port for MMC (default high)
      P1DIR |= BIT3 + BIT2 + BIT7;              // ACLK, MCLK, SMCLK set out to pins
      P1SEL |= BIT3 + BIT2 + BIT7;              // P2.0,2,4 for debugging purposes.

                    // ACLK, MCLK, SMCLK set out to pins
      //Enable Pull-up resistor
      //1) Resistor on (write 1)
      P1REN |= BIT3 + BIT2 + BIT7;
      //2) Select pull-up (write 1)
      P1OUT |= BIT3 + BIT2 + BIT7;

        //halSPISetup
      UCB0CTL0 = UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit
      UCB0CTL0 |= UCMST;                   // SPI master
      UCB0CTL1 = UCSSEL_2+UCSWRST;              // SMCLK
      //Set bit rate
      UCB0BR0 |= 0x01;                          // UCLK/2
      UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCTXIE;                         // Enable USCI_B0 RX interrupt


    GPS_packet.start = '$';
    GPS_packet.t = dt;
    GPS_packet.x = lat;
    GPS_packet.y = lon;
    GPS_packet.stop = '*';

    MY_spiSendFrame((unsigned char*)&GPS_packet, 26);
    P2OUT ^= BIT0;

    mmc = mmcInit();

    printf("send_by_spi - end\n");
}*/

int controller_control_frame(char *sector_buffer, long dt, long lat, long lon, long alt, long course, long speed) {
    Controller_ControlFrame cf;
    cf.dt = dt;
    cf.lat = lat;
    cf.lon = lon;
    cf.alt = alt;
    cf.course = course;
    cf.speed = speed;
    memcpy(sector_buffer, &cf, sizeof(cf));
    return sizeof(Controller_ControlFrame);
}

int controller_diff_frame(char *sector_buffer, int dlat, int dlon, int dalt, int dcourse, int dspeed) {
    Controller_DiffFrame df;
    df.dlat = dlat;
    df.dlon = dlon;
    df.dalt = dalt;
    df.dcourse = dcourse;
    df.dspeed = dspeed;
    memcpy(sector_buffer, &df, sizeof(df));
    return sizeof(Controller_DiffFrame);
}

int recs = 0;

void set_bit(Controller_byte *bitmask, int value) {
    int bindex = recs / 8;
    int byte = recs % 8;

    switch (byte) {
    case 0: bitmask[bindex].a = value; break;
    case 1: bitmask[bindex].b = value; break;
    case 2: bitmask[bindex].c = value; break;
    case 3: bitmask[bindex].d = value; break;
    case 4: bitmask[bindex].e = value; break;
    case 5: bitmask[bindex].f = value; break;
    case 6: bitmask[bindex].g = value; break;
    case 7: bitmask[bindex].h = value; break;
    }

    recs ++;
}

void write_magic_byte(Controller_byte *bitmask, int records_in_sector) {
    Controller_Converter cc;
    cc.val = (char)records_in_sector; // 1..96
    bitmask[12] = cc.bval;
}

void flush_sector() {
    printf("**** SAVING TO %d sector %d records\n", (int)g_header.sector, recs);
    write_magic_byte(&ch.hdr[0], recs);
    memcpy(sector_buffer, &ch, sizeof(Controller_Header));

    mmc = mmcInit();
    mmc = _mmcWriteSector(g_header.sector, sector_buffer);
    if (mmc != MMC_SUCCESS) {
        printf("***** MEGA ERROR WRITING SECTOR !!! *****\n");
        g_header.mmc_error ++;
    }
    mmc = _mmcWriteSector(0, &g_header);
    mmcGoIdle();

    g_header.sector ++;

    memset(&ch, 0, sizeof(ch));
    memset(sector_buffer, 0xFF, SECTOR_SIZE);

    buffer_offset = sizeof(Controller_Header);
    recs = 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void port2_isr(void) {
    P2IFG = 0;

    seconds = 5;
    GPIO_setOutputHighOnPin(LED_2); // power led

    __bic_SR_register_on_exit(LPM3_bits | GIE); // to return to main()
}

void init_button() {
    // Button setup
    P2DIR &= ~BUTTON;                     // button is an input
    P2OUT |= BUTTON;                      // pull-up resistor
    P2REN |= BUTTON;                      // resistor enabled
    P2IES |= BUTTON;                      // interrupt on low-to-high transition
    P2IE |= BUTTON;                       // interrupt enable
}

#ifdef ORIGIN_GPS
bool any_gps = false;
uint8_t rx_char;
void init_gps() {
    GPIO_setAsOutputPin(PIN_GPS_ON_OFF);
    GPIO_setOutputLowOnPin(PIN_GPS_ON_OFF);

    // wait module to startup-ready state
    // wait_ms(500);
    wait_ms(500);

    // раз что-то получили по i2c - значит GPS - модуль уже разбужен (проц перезагружался).
    // по хорошему надо бы еще проверить наличие $ в данных

    if (any_gps) {
        return;
    }

    while (1) {
        GPIO_toggleOutputOnPin(LED_2);

        // low-high-low pulse
        GPIO_setOutputHighOnPin(PIN_GPS_ON_OFF);
        wait_ms(100); // 100
        GPIO_setOutputLowOnPin(PIN_GPS_ON_OFF);

        wait_ms(300); // 300

        // uint8_t rx_char = USCI_B_I2C_slaveGetData(USCI_B0_BASE);
        // rx_char = USCI_A_UART_receiveData(USCI_A0_BASE);
        // printf("char = [%d]\n", rx_char);

        // must wait for "$PSRF150,"
        if (any_gps) {
            break;
        }
    }
}
#endif

void uart_configure() {
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN5 + GPIO_PIN6);

    USCI_A_UART_initParam param = {0};

    param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    //  9600
#ifdef ORIGIN_GPS
    param.clockPrescalar    = 210;//209;
#else
    param.clockPrescalar    = 109;
#endif

    param.firstModReg       = 0;
    param.secondModReg      = 0;

    param.parity            = USCI_A_UART_NO_PARITY;
    param.msborLsbFirst     = USCI_A_UART_LSB_FIRST;
    param.numberofStopBits  = USCI_A_UART_ONE_STOP_BIT;
    param.uartMode          = USCI_A_UART_MODE;
    param.overSampling      = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    USCI_A_UART_init(USCI_A0_BASE, &param);

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);

    //Enable Receive Interrupt
    USCI_A_UART_clearInterrupt(USCI_A0_BASE,  USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);

#ifdef ORIGIN_GPS
    __enable_interrupt();
    init_gps();
    __disable_interrupt();
#endif
}

void init_gpio() {
    GPIO_setAsInputPinWithPullUpResistor(PIN_JP4);

    GPIO_setAsOutputPin(LED_1);
    GPIO_setOutputLowOnPin(LED_1);

    GPIO_setAsOutputPin(LED_2);
    GPIO_setOutputLowOnPin(LED_2);

    GPIO_setAsOutputPin(LED_3);
    GPIO_setOutputLowOnPin(LED_3);

    GPIO_setOutputHighOnPin(PIN_PA_EN);
    GPIO_setOutputLowOnPin(PIN_LNA_EN);
    GPIO_setOutputLowOnPin(PIN_HGM);

    for (int i = 0; i < 6; i ++) {
        GPIO_toggleOutputOnPin(LED_1);
        GPIO_toggleOutputOnPin(LED_2);
        GPIO_toggleOutputOnPin(LED_3);
        wait_ms(80);
    }
    GPIO_setOutputHighOnPin(LED_2);
}

bool uart_event = false;
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    // printf("interrupt!\n");

    switch(__even_in_range(UCA0IV, 4)) {
    case 2: { // Vector 2 - RXIFG
        uint8_t rx_char = USCI_A_UART_receiveData(USCI_A0_BASE);
#ifdef ORIGIN_GPS
        any_gps = true;
#endif
        if (rx_char == '$') {
            start_sentence = true;
        }
        if (start_sentence) {
            if (uart_rx_data_size < UART_BUFFER_SIZE - 1) {
                uart_rx_data[uart_rx_data_size++] = rx_char;
                uart_rx_data[uart_rx_data_size] = 0;
                if (rx_char == '\r' || rx_char == '\n') {
                    WDT_A_resetTimer(WDT_A_BASE);

                    // printf("%s\n", uart_rx_data);

                    // printf(">> %s\n", uart_rx_data);
                    if ((strncmp((const char *)uart_rx_data, "$GPRMC", 5) == 0 || strncmp((const char *)uart_rx_data, "$GNRMC", 5) == 0) ||
                        (strncmp((const char *)uart_rx_data, "$GPGGA", 5) == 0 || strncmp((const char *)uart_rx_data, "$GNGGA", 5) == 0)) {
                        // printf(">> %s\n", uart_rx_data);
                        uart_event = true;
                        __bic_SR_register_on_exit(LPM3_bits | GIE);
                    }
                    uart_rx_data_size = 0;
                    start_sentence = false;
                }
            } else {
                start_sentence = false;
                uart_rx_data_size = 0;
            }
        }
        break;
    }
    }
}

long my_abs(long a) {
    return (a > 0 ? a : -a);
}


int rmc_sector = 1000;

int checksum() {
    int cs = 0;
    for (int i = 1; uart_rx_data[i] && uart_rx_data[i] != '*'; i ++) {
        cs = cs ^ (signed char)uart_rx_data[i];
    }
    return cs;
}

#include <stdlib.h>

char hexString[20];
int x;

void parse_nmea() {
    // printf("%s\n", uart_rx_data);
    if (g_header.sector < SECTORS_COUNT) {
        int cs = checksum();

        char *nmea[30];
        char delims[] = ",";

        printf("%s\n", uart_rx_data);

        int i = 0;
        nmea[i] = strtok((char *)uart_rx_data, delims);
        while (nmea[i] != NULL && i < sizeof(nmea) / sizeof(nmea[0])) {
            nmea[++ i] = strtok(NULL, delims);
        }

        if (i > 1) {
            char *ptr = strchr(nmea[i - 1], '*');
            if (ptr != NULL) {
                int ccs = strtol(ptr + 1, NULL, 16);
                if (ccs != cs) {
                    printf("* CHECKSUM ERROR (%d != %d)\n", ccs, cs);
                    return;
                }
            }
        }

        char packet_ok = false;

        if (strstr(nmea[0], "GGA") != NULL) {
            // GPIO_toggleOutputOnPin(LED_1);
// #ifdef ORIGIN_GPS
            if (i >= 6 && (has_date)) {
                if ((*nmea[3] == 'N' || *nmea[3] == 'S') && (*nmea[5] == 'E' || *nmea[5] == 'W')) {
                    if (i >= 9 && *(nmea[9]) != 0) {
                        alt = atof(nmea[9]) * 10;
                    }
                }
            }
//#else
            if (seconds == 0) {
                // GPIO_setOutputLowOnPin(LED_1);
                GPIO_setOutputLowOnPin(LED_2);
            }

            if (seconds > 0) {
                seconds --;
            }

            if (i >= 6 && (has_date)) {
                if ((*nmea[3] == 'N' || *nmea[3] == 'S') && (*nmea[5] == 'E' || *nmea[5] == 'W')) {
                    packet_ok = 1;

                    if (seconds > 0) {
                        if (seconds % 2 == 0) {
                            GPIO_setOutputHighOnPin(LED_2);
                        } else {
                            GPIO_setOutputLowOnPin(LED_2);
                        }
                    }

                    char *time0 = nmea[1];
                    char *slat = nmea[2];
                    bool isN = ((*nmea[3]) == 'N');
                    char *slon = nmea[4];
                    bool isE = ((*nmea[5]) == 'E');

                    double lat_start = (double)atoin(slat, 2);
                    double lat_end = (double)atof(slat + 2) / 60;
                    double _lat = lat_start + lat_end;

                    double lon_start = (double)atoin(slon, 3);
                    double lon_end = (double)atof(slon + 3) / 60;
                    double _lon = lon_start + lon_end;

                    ttime.tm_hour = atoin(time0, 2);
                    ttime.tm_min = atoin(time0 + 2, 2);
                    ttime.tm_sec = atoin(time0 + 4, 2);

                    unsigned long dt = mktime(&ttime);
                    double xlat = _lat * (isN ? 1 : -1);
                    double xlon = _lon * (isE ? 1 : -1);

                    long lat = xlat * 1000000;
                    long lon = xlon * 1000000;

                    if (dt < last_dt) {
                        dt += 86400;
                    } else {
                        last_dt = dt;
                    }

                    int delta = (int)(dt - dt_prev);
                    // printf("interval %d seconds\n", delta);
                    if (delta > 1) {
                        //printf("GGA %s ERROR %d seconds\n", nmea[1], delta);
                        // printf("*** ERROR ***\n");
                    } else {
                        //printf("GGA %s\n", nmea[1]);
                    }

                    if (delta == 0) {
                        //printf("GGA %s SKIPPING\n", nmea[1]);
                        return;
                    }

                    printf("%s\n", time0);

                    if (lat_prev == 0 || (dt_prev != 0 && (delta > 1))) { // first coord or delta(dt) > 1
                        if (buffer_offset > SECTOR_SIZE - sizeof(Controller_ControlFrame)) { // sector overflow
                            flush_sector();
                        }

                        buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                        set_bit(&ch.hdr[0], 1);
                    } else {
                        long lat_delta    = (lat    - lat_prev),
                             lon_delta    = (lon    - lon_prev),
                             alt_delta    = (alt    - alt_prev),
                             course_delta = (course - course_prev),
                             speed_delta  = (speed  - speed_prev);

                        if (my_abs(lat_delta) > 127 || my_abs(lon_delta) > 127 || my_abs(alt_delta) > 127 || my_abs(course_delta) > 127 || my_abs(speed_delta) > 127) { // deltas overflow
                            if (buffer_offset > SECTOR_SIZE - sizeof(Controller_ControlFrame)) { // sector overflow
                                flush_sector();
                            }

                            buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                            set_bit(&ch.hdr[0], 1);
                        } else {
                            if (buffer_offset > SECTOR_SIZE - sizeof(Controller_DiffFrame)) { // sector overflow
                                flush_sector();

                                buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                                set_bit(&ch.hdr[0], 1);
                            } else {
                                buffer_offset += controller_diff_frame(&sector_buffer[buffer_offset], (int)lat_delta, (int)lon_delta, (int)alt_delta, (int)course_delta, (int)speed_delta);
                                set_bit(&ch.hdr[0], 0);
                            }
                        }
                    }

                    //send_by_spi(dt, xlat, xlon);
#ifdef SEND_BY_SPI
                    GPIO_toggleOutputOnPin(LED_3);
                    ToGSM(1, dt, lat, lon, alt, course, speed);
#endif

                    lat_prev    = lat;
                    lon_prev    = lon;
                    alt_prev    = alt;
                    course_prev = course;
                    speed_prev  = speed;
                    dt_prev     = dt;
                } else {
                    printf("*** NO FIX 1 ***\n");
                }
            } else {
                printf("*** NO FIX 2 ***\n");
            }
//#endif
        } else { // RMC packet

            /*for (int j = 0; j < i; j ++) {
                printf("%d -> %s\n", j, nmea[j] ? nmea[j] : "(null)");
            }*/
            // GPIO_toggleOutputOnPin(LED_3);

            /*if (i >= 5 && (*nmea[2]) == 'V') { // GPS NO 3D fix but has date
                char *date = nmea[3];

                memset(&ttime, 0, sizeof(struct tm));

                ttime.tm_mday = atoin(date, 2) + 1;
                ttime.tm_mon = atoin(date + 2, 2) - 1;
                ttime.tm_year = atoin(date + 4, 2) + 30;

                printf("RMC NO 3D fix DATE=%s\n", date);

                has_date = true;
            }*/

            if (i >= 11 && (*nmea[2]) == 'A') { // GPS 3D fix
                char *date = nmea[9];

                memset(&ttime, 0, sizeof(struct tm));

                ttime.tm_mday = atoin(date, 2) + 1;
                ttime.tm_mon = atoin(date + 2, 2) - 1;
                ttime.tm_year = atoin(date + 4, 2) + 30;

                if (*(nmea[7]) != 0) {
                    speed = my_abs(atof(nmea[7]) * 0.5144 * 10);
                }
                if (*(nmea[8]) != 0) {
                    course = atof(nmea[8]) * 10;
                }

                /*
                    0 $GNRMC
                    1 150758.000
                    2 A
                    3 5955.9934
                    4 N
                    5 03021.5867
                    6 E
                    7 0.00
                    8 293.01
                    9 230517
                    10 A*72
                 */

                has_date = true;

//#ifdef ORIGIN_GPS
                /*if (seconds == 0) {
                    // GPIO_setOutputLowOnPin(LED_1);
                    GPIO_setOutputLowOnPin(LED_2);
                }

                if (seconds > 0) {
                    seconds --;
                }*/

                if (i >= 6 && (has_date)) {
                    if ((*nmea[4] == 'N' || *nmea[4] == 'S') && (*nmea[6] == 'E' || *nmea[6] == 'W')) {
                        packet_ok = 1;

                        /*if (seconds > 0) {
                            if (seconds % 2 == 0) {
                                GPIO_setOutputHighOnPin(LED_2);
                            } else {
                                GPIO_setOutputLowOnPin(LED_2);
                            }
                        }*/

                        char *time0 = nmea[1];
                        char *slat = nmea[3];
                        bool isN = ((*nmea[4]) == 'N');
                        char *slon = nmea[5];
                        bool isE = ((*nmea[6]) == 'E');

                        double lat_start = (double)atoin(slat, 2);
                        double lat_end = (double)atof(slat + 2) / 60;
                        double _lat = lat_start + lat_end;

                        double lon_start = (double)atoin(slon, 3);
                        double lon_end = (double)atof(slon + 3) / 60;
                        double _lon = lon_start + lon_end;

                        ttime.tm_hour = atoin(time0, 2);
                        ttime.tm_min = atoin(time0 + 2, 2);
                        ttime.tm_sec = atoin(time0 + 4, 2);

                        unsigned long dt = mktime(&ttime);
                        double xlat = _lat * (isN ? 1 : -1);
                        double xlon = _lon * (isE ? 1 : -1);

                        long lat = xlat * 1000000;
                        long lon = xlon * 1000000;

                        if (dt < last_dt) {
                            dt += 86400;
                        } else {
                            last_dt = dt;
                        }

                        int delta = (int)(dt - dt_prev);
                        // printf("interval %d seconds\n", delta);
                        if (delta > 1) {
                            // printf("RMC %s ERROR %d seconds\n", nmea[1], delta);
                            // printf("*** ERROR ***\n");
                        } else {
                            // printf("RMC %s\n", nmea[1]);
                        }
                        if (delta == 0) {
                            // printf("RMC %s SKIPPING\n", nmea[1]);
                            return;
                        }

                        printf("%s\n", time0);

                        if (lat_prev == 0 || (dt_prev != 0 && (delta > 1))) { // first coord or delta(dt) > 1
                            if (buffer_offset > SECTOR_SIZE - sizeof(Controller_ControlFrame)) { // sector overflow
                                flush_sector();
                            }

                            buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                            set_bit(&ch.hdr[0], 1);
                        } else {
                            long lat_delta    = (lat    - lat_prev),
                                 lon_delta    = (lon    - lon_prev),
                                 alt_delta    = (alt    - alt_prev),
                                 course_delta = (course - course_prev),
                                 speed_delta  = (speed  - speed_prev);

                            if (my_abs(lat_delta) > 127 || my_abs(lon_delta) > 127 || my_abs(alt_delta) > 127 || my_abs(course_delta) > 127 || my_abs(speed_delta) > 127) { // deltas overflow
                                if (buffer_offset > SECTOR_SIZE - sizeof(Controller_ControlFrame)) { // sector overflow
                                    flush_sector();
                                }

                                buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                                set_bit(&ch.hdr[0], 1);
                            } else {
                                if (buffer_offset > SECTOR_SIZE - sizeof(Controller_DiffFrame)) { // sector overflow
                                    flush_sector();

                                    buffer_offset += controller_control_frame(&sector_buffer[buffer_offset], dt, lat, lon, alt, course, speed);
                                    set_bit(&ch.hdr[0], 1);
                                } else {
                                    buffer_offset += controller_diff_frame(&sector_buffer[buffer_offset], (int)lat_delta, (int)lon_delta, (int)alt_delta, (int)course_delta, (int)speed_delta);
                                    set_bit(&ch.hdr[0], 0);
                                }
                            }
                        }

                        //send_by_spi(dt, xlat, xlon);
#ifdef SEND_BY_SPI
                        GPIO_toggleOutputOnPin(LED_3);

                        ToGSM(1, dt, lat, lon, alt, course, speed);
#endif

                        lat_prev    = lat;
                        lon_prev    = lon;
                        alt_prev    = alt;
                        course_prev = course;
                        speed_prev  = speed;
                        dt_prev     = dt;
                    } else {
                        printf("*** NO FIX 1 ***\n");
                    }
                } else {
                    printf("*** NO FIX 2 ***\n");
                }
//#else
                // printf("RMC %s\n", nmea[1]);
//#endif
            }

        }

#ifdef SEND_BY_SPI
        if (packet_ok == 0) {
            GPIO_toggleOutputOnPin(LED_2);
            ToGSM(0, dt, lat, lon, alt, course, speed);
        }
#endif
    }
}

//#include "version.h"
#include "config.h"
unsigned short serial;


int main(void) {
    WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_SMCLK, WDT_A_CLOCKDIVIDER_8192K);
    WDT_A_start(WDT_A_BASE);

    //unsigned char bDieRecord_bytes;
    //struct s_TLV_Die_Record *pDIEREC;
    //TLV_getInfo(TLV_DIERECORD, 0, &bDieRecord_bytes, (unsigned int **)&pDIEREC);

    init_gpio();
    uart_configure();

    memset(&ch, 0, sizeof(ch));
    memset(sector_buffer, 0xFF, SECTOR_SIZE);
    memset(&g_header, 0, sizeof(g_header));

    mmc = mmcInit();
    printf("restart mmc=%d\n", mmc);
    if (mmc == MMC_SUCCESS) {
        SECTORS_COUNT = mmcReadCardSize() / SECTOR_SIZE;
        mmc = mmcReadSector(0, (unsigned char *)&g_header);
        if (mmc == MMC_SUCCESS) {
            if (g_header.magic[0] == MAGIC_BYTE && g_header.sector > 0) {
                g_header.restarts ++;
                mmc = _mmcWriteSector(0, &g_header);

                g_header.sector ++;
            } else {
                mmc = MMC_CRC_ERROR;
            }
        }
    } else {
        // no sd-card
        GPIO_setOutputHighOnPin(LED_1);
        GPIO_setOutputHighOnPin(LED_2);
        GPIO_setOutputHighOnPin(LED_3);

        while(1);
    }

    if (mmc != MMC_SUCCESS) {
        memset(&g_header, 0, sizeof(g_header));
        g_header.magic[0] = MAGIC_BYTE;
        g_header.sector = 1;
        g_header.restarts = 1;
        g_header.mmc_error = 0;
    } else {
        mmcGoIdle();
    }

    // g_header.device_id = pDIEREC->wafer_id; // set device id
    g_header.device_id = get_device_id();
    if (g_header.device_id == -1) {
        /*
         * Tracker  ELA-1        1
         * Tracker  ORG-2 (chef)    1
         * Tracker  ORG-3           2     (OK)
         * Tracker  ELA-4        2
         * SP       ELA-5        3
         * SP       ORG-6           3     (OK)
         * SP (old) ELA-7              1
         * SP (old) ELA-8              2
         * SP (old) ELA-9              3
         * SP       ORG-10          4     (OK)
         * SP       ELA-11       4
         * SP       ORG-12          5     (OK)
         * Tracker  ORG-13          6     (OK) with OrangePi Zero 2G IoT
         * SP       ELA-14       5
         * Tracker  ELA-15       6
         * */
        g_header.device_id = 14;
        set_device_id(g_header.device_id);
    }

    serial = (unsigned short)g_header.device_id;

    init_button();

#ifdef SEND_BY_SPI
    spi_init();
#endif

    __enable_interrupt();

    P2IFG = 0;

    while(1) {

       //      spitimer++; //for siml only
       //      spi_task(); //for siml only


        if (uart_event) {
            uart_event = false;
            __disable_interrupt();

            parse_nmea();

            __enable_interrupt();
        }

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();
    }

	// return 0;
}
