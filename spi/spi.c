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


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <errno.h>
#include "spi.h"



/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
void spi_init (int fd, unsigned char mode1, unsigned char bits1, unsigned int speed1)
{
    int ret;

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode1);
    if (ret == -1)
    {
       printf("can't set spi mode\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode1);
    if (ret == -1)
    {
        printf("can't get spi mode\n");
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits1);
    if (ret == -1)
    {
        printf("can't set bits per word\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits1);
    if (ret == -1)
    {
        printf("can't get bits per word\n");
    }

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed1);
    if (ret == -1)
    {
        printf("can't set max speed hz\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed1);
    if (ret == -1)
    {
        printf("can't get max speed hz\n");
    }


} /* ----- End of spi_init()  ----- */

void spi_close(int fd){
    close(fd);
}

void  spi_write(int fd , unsigned char  *opt)
{
    int ret;
    unsigned char  tx[2];
    unsigned char  rx[2];
    tx[0] = opt[0];
    tx[1] = opt[1];
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = 2;
    tr.delay_usecs = delay_spi;
    tr.speed_hz = speed_spi;
    tr.bits_per_word = bits_spi;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        printf("错误是%s\n",strerror(errno));
        printf("can't write spi message\n");
    }
}

unsigned char  spi_read(int fd, unsigned char  *opt)
{
    int ret;
    unsigned char  tx[2];
    unsigned char  rx[2];
    tx[0] = opt[0];
    tx[1] = opt[1];
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = 2;
    tr.delay_usecs = delay_spi;
    tr.speed_hz = speed_spi;
    tr.bits_per_word = bits_spi;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        printf("错误是%s\n",strerror(errno));
        printf("can't write spi message\n");
        return -1;
    }
    unsigned r = rx[1];
    return r; 
}
