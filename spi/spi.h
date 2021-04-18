/*********************************************************************************
 *      Copyright:  (C) 2021 9305
 *                  All rights reserved.
 *
 *       Filename:  spi.c
 *    Description:  This file generate a interface of spi
 *
 *        Version:  1.0.0(04/16/2021)
 *         Author:  yjc <6762641@qq.com>
 *      ChangeLog:  1, Release initial version on "04/16/2021 11:40:15 PM"
 *
 ********************************************************************************/


#ifndef __SPI_H__
#define __SPI_h__

#define    device_spi   "/dev/spidev0.0"
#define    mode_spi     0
#define    bits_spi     8
#define    speed_spi    500000
#define    delay_spi    0
extern void  spi_write(int fd,unsigned char * opt);
extern unsigned char  spi_read(int fd, unsigned char *opt);
extern void  spi_close(int fd);
extern void spi_init (int fd, unsigned char mode1, unsigned char bits1, unsigned int speed1);


#endif
