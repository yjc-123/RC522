/*********************************************************************************
 *      Copyright:  (C) 2021 9305
 *                  All rights reserved.
 *
 *       Filename:  rc522.h
 *    Description:  This file The basic logic of rc522 is realized
 *
 *        Version:  1.0.0(04/16/2021)
 *         Author:  yjc <6762641@qq.com>
 *      ChangeLog:  1, Release initial version on "04/16/2021 11:40:15 PM"
 *
 ********************************************************************************/


#ifndef __RC522_H__
#define __RC522_H__



/**************************************************************************************
 *  Description:
 *************************************************************************************/
#define PCD_IDLE            0x00    /* Cancle the current command */
#define PCD_AUTHENT         0x0E    /* Authentication key */
#define PCD_RECEIVE         0x08    /* Receive data */
#define PCD_TRANSMIT        0x04    /* To send data */
#define PCD_TRANSCEIVE      0x0C    /* To send and receive data */
#define PCD_RESETPHASE      0x0F    /* Reset */ 
#define PCD_CALCCRC         0x03    /* CRC calculation */



/**************************************************************************************
 *  Description:
 *************************************************************************************/
#define PICC_REQIDL         0x26    /* Looking for a IC card in the area of antenna, that 
                                       didn't enter a dormant state the area of IC card */
#define PICC_REQALL         0x52    /* Looking for all IC card in the area of antena */
#define PICC_ANTICOLL1      0x93    /* Anticollision */
#define PICC_ANTICOLL2      0x95    /* Anticollision */
#define PICC_AUTHENT1A      0x60    /* Authentication A key */
#define PICC_AUTHENT1B      0x61    /* Authentication B key */
#define PICC_READ           0x30    /* Read block */
#define PICC_WRITE          0xA0    /* Write block */
#define PICC_DECREMENT      0xC0    /* Deductions */
#define PICC_INCREMENT      0xC1    /* Rechange */
#define PICC_RESTORE        0xC2    /* The block of data transferrend to the buffer */
#define PICC_TRANSFER       0xB0    /* Save a data in a buffer */
#define PICC_HALT           0x50    /* dormancy */



/**************************************************************************************
 *  Description:
 *************************************************************************************/
#define DEF_FIFO_LENGTH     64      /* FIFO size = 64byte */
#define MAXRLEN             18      /* The maximum length of data received */



/**************************************************************************************
 *  Description:
 *************************************************************************************/

/* PAGE 0 */
#define RFU00               0x00
#define COMMAND_REG         0x01
#define COM_IEN_REG         0x02
#define DIVL_EN_REG         0x03
#define COM_IRQ_REG         0x04
#define DIV_IRQ_REG         0x05
#define ERROR_REG           0x06
#define STATUS1_REG         0x07
#define STATUS2_REG         0x08
#define FIFO_DATA_REG       0x09
#define FIFO_LEVEL_REG      0x0A
#define WATER_LEVEL_REG     0x0B
#define CONTROL_REG         0x0C
#define BIT_FRAMING_REG     0x0D
#define COLL_REG            0x0E
#define RFU0F               0x0F

/* PAGE 1 */
#define RFU10               0x10
#define MODE_REG            0x11
#define TX_MODE_REG         0x12
#define RX_MODE_REG         0x13
#define TX_CONTROL_REG      0x14
#define TX_AUTO_REG         0x15
#define TX_SEL_REG          0x16
#define RX_SEL_REG          0x17
#define RX_THRESHOLD_REG    0x18
#define DEMOD_REG           0x19
#define RFU1A               0x1A
#define RFU1B               0x1B
#define MIFARE_REG          0x1C
#define RFU1D               0x1D
#define RFU1E               0x1E
#define SERIAL_SPEED_REG    0x1F

/* PAGE 2 */
#define RFU20               0x20  
#define CRC_RESULT_REG_M    0x21
#define CRC_RESULT_REG_L    0x22
#define RFU23               0x23
#define MOD_WIDTH_REG       0x24
#define RFU25               0x25
#define RF_CFG_REG          0x26
#define GS_NREG             0x27
#define CWGS_CFG_REG        0x28
#define MOD_GS_CFG_REG      0x29
#define T_MODE_REG          0x2A
#define T_PRESCALER_REG     0x2B
#define T_RELOAD_REG_H      0x2C
#define T_RELOAD_REG_L      0x2D
#define T_COUN_VALUE_REGH   0x2E
#define T_COUN_VALUE_REGL   0x2F

/* PAGE 3 */
#define RFU30               0x30
#define TEST_SEL1_REG       0x31
#define TEST_SEL2_REG       0x32
#define TEST_PIN_EN_REG     0x33
#define TEST_PIN_VALUE_REG  0x34
#define TEST_BUS_REG        0x35
#define AUTO_TEST_REG       0x36
#define VERSION_REG         0x37
#define ANALOG_TEST_REG     0x38
#define TEST_DAC1_REG       0x39  
#define TEST_DAC2_REG       0x3A   
#define TEST_ADC_REG        0x3B   
#define RFU3C               0x3C   
#define RFU3D               0x3D   
#define RFU3E               0x3E   
#define RFU3F               0x3F

/**************************************************************************************
 *  Description:
 *************************************************************************************/
#define MI_OK               0    
#define MI_NOTAGERR         (-1)
#define MI_ERR              (-2)



/**************************************************************************************
 *  Description:
 *************************************************************************************/
extern void spi_read_byte (unsigned char *byte);
extern void spi_write_byte (unsigned char byte);
extern void write_reg (unsigned char addr, unsigned char data);
extern void write_string (unsigned char addr, unsigned char *data, unsigned char len);
extern unsigned char  read_reg (unsigned char addr);
extern void read_string (unsigned char addr, unsigned char *data, unsigned char len);
extern void clear_bit_mask (unsigned char reg, unsigned char mask);
extern void set_bit_mask (unsigned char reg, unsigned char mask);
extern void calulatate_crc (unsigned char *indata, unsigned char len, unsigned char *outdata);
extern int rfid_com (unsigned char command, unsigned char *indata,
                     unsigned char inlenbyte, unsigned char *outdata,
                     unsigned int  *outlenbit);
extern int rfid_request (unsigned char reg_code, unsigned char *card_type);
extern int rfid_anticoll (unsigned char *sernum);
extern int rfid_select (unsigned char *sernum);
extern int rfid_auth_state (unsigned char auth_mode, unsigned char addr,
                            unsigned char *key, unsigned char *sernum);
extern int rfid_read (unsigned char, unsigned char *data);
extern int rfid_write (unsigned char addr, unsigned char *data);
extern int rfid_halt (void);
extern void rfid_reset (void);
extern void rfid_antenna_on (void);
extern void rfid_antenna_off (void);
extern int rfid_conf_iso_type (unsigned char type);
extern void rfid_init (int rfid_fd);



#endif
