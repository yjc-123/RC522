# RC522
Rc522 card recognition based on SPI

### SPI
##### 1、简介
SPI才用master/slave模式，一个master管理多个slave，通过SS片选信号实现主机控制某一个从机，SPI属于同步全双工传输协议。spi工作机制是master跟slave都有移位寄存器，也就是说master发送一位在FIFODATA中slave也会相应的发送一位到FIFODATA中，发送完一个字节之后，master发送完成，svale也相应的给FIFODATA写了一个字节，master直接读取就行了。但是真正读数据的时候data sheet中是这样告诉我们的
![image](https://github.com/yjc-123/RC522/blob/master/images/%E8%AF%BB.jpg)

我们要先下寄存器，然后再发送一个字节，这个字节数据可以随意一般为0，才能够获取到数据，下面再spi数据接收中写到了，回来的rx，我们直接收rx[1]，将rx[0]丢掉了。
SPI有四根线分别是：
+ MOSI：Master Output Slave Input，顾名思义，即主设备输出/从设备输入。数据从主机输出到从机，主机发送数据。
+ MISO：Master Iutput Slave Onput，主设备输入/从设备输出，数据由从机输出到主机，主机接收数据。
+ SCK：即时钟信号线，用于通讯同步。该信号由主机产生，其支持的最高通讯速率为系统时钟频率的1/2，即所挂载总线速率的一半。如SPI2挂载在APB1总线上，则其最高速率为36MHz / 2 = 18MHz。类似木桶效应，两个设备之间通讯时，通讯速率受限于较低速的设备。
+ NSS：即片选信号线，用于选择通讯的从设备，也可用CS表示。每个从设备都有一条独立的NSS信号线，主机通过将某个设备的NSS线置低电平来选择与之通讯的从设备。所以SPI通讯以NSS线电平置低为起始信号，以NSS线电平被拉高为停止信号。

SPI有四种传输模式：
CPOL：时钟极性选择，即为0时SPI总线空闲为低电平，为1时SPI总线空闲为高电平。
CPHA：时钟相位选择，即为0时在SCK第一个跳变沿采样,第二个跳变发送，为1时在SCK第二个跳变沿采样，第一个跳变沿发送。
    `工作方式1：`
　　当CPHA=0、CPOL=0时SPI总线工作在方式1。MISO引脚上的数据在第一个SCK沿跳变之前已经上线了，而为了保证正确传输，MOSI引脚的MSB位必须与SCK的第一个边沿同步，在SPI传输过程中，首先将数据上线，然后在同步时钟信号的上升沿时，SPI的接收方捕捉位信号，在时钟信号的下降沿时，SPI发送数据，下一位数据信号上线，再重复上述过程，直到一个字节的8位信号传输结束。

　　`工作方式2：`
　　当CPHA=0、CPOL=1时SPI总线工作在方式2。与前者唯一不同之处只是在同步时钟信号的下降沿时捕捉位信号，上升沿时下一位数据上线。

　　`工作方式3：`
　　当CPHA=1、CPOL=0时SPI总线工作在方式3。MISO引脚和MOSI引脚上的数据的MSB位必须与SCK的第一个边沿同步，在SPI传输过程中，在同步时钟信号周期开始时（上升沿）数据上线，然后在同步时钟信号的下降沿时，SPI的接收方捕捉位信号，在时钟信号的一个周期结束时（上升沿），下一位数据信号上线，再重复上述过程，直到一个字节的8位信号传输结束。

　　`工作方式4：`
　　当CPHA=1、CPOL=1时SPI总线工作在方式4。与前者唯一不同之处只是在同步时钟信号的上升沿时捕捉位信号，下降沿时下一位数据上线。
##### 2、SPI在linux下应用层代码
我们需要知道两个重要的结构体：
```
struct spi_device {  
         structdevice        dev;  
         structspi_master *master;  
         u32                            max_speed_hz;    /* 通信时钟最大频率 */  
         u8                             chip_select;    /* 片选号 */  
         u8                             mode;           /*SPI设备的模式，下面的宏是它各bit的含义  */  
        #define       SPI_CPHA          0x01                /* 采样的时钟相位                            */  
        #define       SPI_CPOL          0x02               /* 时钟信号起始相位：高或者是低电平*/  
        #define       SPI_MODE_0        (0|0)                      
        #define       SPI_MODE_1        (0|SPI_CPHA)  
        #define       SPI_MODE_2        (SPI_CPOL|0)  
        #define       SPI_MODE_3        (SPI_CPOL|SPI_CPHA)  
        #define       SPI_CS_HIGH       0x04                    /* 为1时片选的有效信号是高电平*/  
        #define       SPI_LSB_FIRST     0x08            /* 发送时低比特在前  */  
        #define       SPI_3WIRE         0x10                 /* 输入输出信号使用同一根信号线 */  
        #define       SPI_LOOP          0x20                 /* 回环模式 */  
         u8                             bits_per_word;    /* 每个通信字的字长（比特数） */  
         int                            irq;             /*使用到的中断 */  
         void                           *controller_state;  
         void                           *controller_data;  
         char                           modalias[32];      /* 设备驱动的名字*/  
};  
```
```
struct spi_ioc_transfer {  
            __u64               tx_buf;                   /* 写数据缓冲  */  
            __u64               rx_buf;                   /* 读数据缓冲  */  
            __u32               len;                      /* 缓冲的长度 */  
            __u32               speed_hz;                 /* 通信的时钟频率 */  
            __u16               delay_usecs;    /* 两个spi_ioc_transfer之间的延时 */  
            __u8                bits_per_word;           /* 字长（比特数）  */  
            __u8                cs_change;               /* 是否改变片选 */  
            __u32               pad;                                
};  
```

有初始化和写数据，初始化代码就不写了，在上面的代码里面有，我只说一下过程：
```
1、打开设备获取描述符
2、初始化传输方式，有四种
3、初始化位模式
4、初始化传输速度
```
下面的代码是spi读取的代码。
```
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
    unsigned r = rx[1];  //发送两个字节回来也是两个字节，但是第一个字节是空，第二个字节才是数据
    return r; 
}
```
下面的代码是spi传输代码。
```
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
```
+ 我对上面的代码进行一下讲解首先创建两个字节大小的发送和接收buf
```
unsigned char  tx[2];
unsigned char  rx[2];
```
+ 然后将传入的参数赋值给发送buf
```
tx[0] = opt[0];
tx[1] = opt[1];
```
+ 创建发送结构体spi_ioc_transfer,并赋值为0
```
struct spi_ioc_transfer tr;
memset(&tr, 0, sizeof(tr));
```
+ 给spi_ioc_transfer赋值
```
tr.len = 2;
tr.delay_usecs = delay_spi;
tr.speed_hz = speed_spi;
tr.bits_per_word = bits_spi;
```
+ 发送
```
ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
```
---
---
---
### RC522
##### 1、简介
其实rc522才是重头戏，我们做这个东西从0-1一共花费了三周，最开始是用python代码编写的，直接github一克隆就下来，但是我自己用c写了一遍，很多代码是跟网上差不多的，因为工作原理就差不多，但是都是再单片机上的我需要自己移植，所以这个过程就很麻烦，不过还好后面做出来了。我就在这里简述一下rc522的工作流程。这个过程很麻烦，具体的自己看ISO14443文档。
###### 2、RC522工作机制
+ 工作流程

    1、寻卡
    2、防冲撞
    3、选卡
    4、三轮认证
    ![image](https://github.com/yjc-123/RC522/blob/master/images/2.png)
在这个过程中，我们需要要知道两个概念，一个是PCD一个是PICC，PCD是用电感耦合给邻近卡提供能量并控制与邻近卡的数据交换的读/写设备，PICC一种卡型号，在通信过程中实际上是使用PCD命令控制RC522发出PICC命令与卡进行交互。具体的在这个链接里面还有 https://blog.csdn.net/wlwl0071986/article/details/48394297

+ rc522的构造图

![image](https://github.com/yjc-123/RC522/blob/master/images/1.jpg) 

上面这张图片就是rc522卡片结构图，可以看见有一个天线接口，作用是当卡片过来将磁信号转换为电信号，FIFO是内部缓存，我们写数据和读数据都是在这里进行，后面就是寄存器，然后就是跟主机通信，可以用i2c、spi、uart三种通信方式，本文用的spi。

+ rc522寄存器

![image](https://github.com/yjc-123/RC522/blob/master/images/3.png)

    1、其中第0扇区的块0是用于存放厂商代码的，已经固化，不可更改，为32位（4Bytes）；
    2、每个扇区的块0、块1和块2位数据块，可用于存储数据，每块16个字节（只有S50卡是这样）；
    3、每个扇区的块3位控制块，包含了密码A、存取控制、密码B，具体结构如下图所示；

