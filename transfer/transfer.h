/*********************************************************************************
 *      Copyright:  (C) 2021 9305
 *                  All rights reserved.
 *
 *       Filename:  transfer.h
 *    Description:  The function of the file is realized by calling rc522 function
 *
 *        Version:  1.0.0(04/16/2021)
 *         Author:  yjc <6762641@qq.com>
 *      ChangeLog:  1, Release initial version on "04/16/2021 11:40:15 PM"
 *
 ********************************************************************************/
#ifndef __MIFARE_H__
#define __MIFARE_H__

#define MI_READ 0x01
#define MI_WRITE 0x02


void block_write (unsigned char block_addr);
void block_read (unsigned char block_addr);


#endif
