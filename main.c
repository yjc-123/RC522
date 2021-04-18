/**************************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  rfid.c
 *    Description:  This file is main file of the program.
 *                 
 *        Version:  1.0.0(03/02/2014)
 *         Author:  fulinux <fulinux@sina.com>
 *      ChangeLog:  1, Release initial version on "03/02/2014 05:46:18 AM"
 *                 
 *************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"
#include "rc522.h"
#include "transfer.h"

static unsigned char mode;
static unsigned char addr = 4;
static unsigned char bits = 8;
static unsigned int speed = 500000;
static const char *device = "/dev/spidev0.0";

extern void  spi_write(int fd,unsigned char * opt);
extern unsigned char  spi_read(int fd, unsigned char *opt);
extern void  spi_close(int fd);
extern void spi_init (int fd, unsigned char mode1, unsigned char bits1, unsigned int speed1);

extern int rfid_read (unsigned char, unsigned char *data);
extern int rfid_write (unsigned char addr, unsigned char *data);


int main (int argc, char **argv)
{
    int fd              = 0;


    fd = open(device, O_RDWR);
    if(fd < 0)
    {
        perror("open the device failed\n");
        exit(1);
    }

    /* 初始化spi */
    spi_init(fd, mode, bits, speed);

    /* 初始化RC522 */
    rfid_init(fd);

    while(1)
    {
        
        block_read(addr);
    }

    return 0;
} /* ----- End of main() ----- */
