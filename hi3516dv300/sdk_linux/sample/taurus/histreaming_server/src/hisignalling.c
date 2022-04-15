#include <stdio.h>
#include "hisignalling.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <poll.h>


#define MSG(args...) printf(args)
pthread_t hisignalling_meg_handle_id;
unsigned char hisignalling_send_buf[HISIGNALLING_MSG_BUFF_LEN] = {0};
// unsigned char hisignalling_send_buf2[HISIGNALLING_MSG_BUFF_LEN] = {0};
unsigned int hisignalling_package_len = 0;
// unsigned int hisignalling_package_len2 = 0;
/**
	@berf GPIO enable of key
	@param pin: gpio port
*/
static int gpio_export(int pin)  
{  
    char buffer[64] = {0};  
    int len = -1;  
    int fd = -1;  

    fd = open("/sys/class/gpio/export", O_WRONLY);  
    if (fd < 0) { 
        MSG("Failed to open export for writing!\n");  
		close(fd);
        return(-1);  
    }  
	
    len = snprintf(buffer, sizeof(buffer), "%d", pin);  
    if (write(fd, buffer, len) < 0){  
        MSG("Failed to export gpio!");  
		close(fd);
        return -1;  
    } 
	
    close(fd);  
    return 0; 
}  
/**
	@berf GPIO disable of key
	@param pin: gpio port
*/
static int gpio_unexport(int pin)  
{  
    char buffer[64] = {0};  
    int len = -1;  
    int fd = -1;   

    fd = open("/sys/class/gpio/unexport", O_WRONLY);  
    if (fd < 0) {  
        MSG("Failed to open unexport for writing!\n");  
		close(fd);
        return -1;  
    }  

    len = snprintf(buffer, sizeof(buffer), "%d", pin);  
    if (write(fd, buffer, len) < 0) {  
        MSG("Failed to unexport gpio!");  
		close(fd);
        return -1;  
    }  

    close(fd);  
    return 0;  
} 

//dir: 0-->IN, 1-->OUT
static int gpio_direction(int pin, int dir)  
{
    static const char dir_str[] = "in\0out";  
    char path[64] = {0};  
    int fd = -1;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0){  
        MSG("Failed to open gpio direction for writing!\n");  
		close(fd);
        return -1;  
    }  

    if (write(fd, &dir_str[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0){  
        MSG("Failed to set direction!\n");  
		close(fd);
        return -1;  
    }  
	
    close(fd);  
    return 0;  
}  

//value: 0-->LOW, 1-->HIGH
// static int gpio_write(int pin, int value)  
// {  
//     static const char values_str[] = "01";  
//     char path[64] = {0};  
//     int fd = -1;  

//     snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
//     fd = open(path, O_WRONLY);  
//     if (fd < 0){  
//         MSG("Failed to open gpio value for writing!\n");  
// 		close(fd);
//         return -1;  
//     }  

//     if (write(fd, &values_str[value == 0 ? 0 : 1], 1) < 0){  
//         MSG("Failed to write value!\n");  
// 		close(fd);
//         return -1;  
//     } 
	
//     close(fd);  
//     return 0;  
// }

// static int gpio_read(int pin)  
// {  
//     char path[64] = {0};  
//     char value_str[3] = {0};  
//     int fd = -1;  

//     snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
//     fd = open(path, O_RDONLY);  
//     if (fd < 0) {  
//         MSG("Failed to open gpio value for reading!\n");  
// 		close(fd); 
//         return -1;  
//     }  

//     if (read(fd, value_str, 3) < 0) {  
//         MSG("Failed to read value!\n");  
// 		close(fd); 
//         return -1;  
//     }  

//     close(fd);  
//     return (atoi(value_str));
// }  

// none表示引脚为输入，不是中断引脚
// rising表示引脚为中断输入，上升沿触发
// falling表示引脚为中断输入，下降沿触发
// both表示引脚为中断输入，边沿触发
// 0-->none, 1-->rising, 2-->falling, 3-->both
static int gpio_edge(int pin, int edge)
{
	const char dir_str[] = "none\0rising\0falling\0both"; 
	int ptr;
	char path[64] = {0};  
	int fd = -1; 

	switch(edge){
		case 0:
			ptr = 0;
			break;
		case 1:
			ptr = 5;
			break;
		case 2:
			ptr = 12;
			break;
		case 3:
			ptr = 20;
			break;
		default:
			ptr = 0;
	} 
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);  
	
	fd = open(path, O_WRONLY);  
	if (fd < 0) {  
		MSG("Failed to open gpio edge for writing!\n");  
		close(fd); 
		return -1;  
	}  

	if (write(fd, &dir_str[ptr], strlen(&dir_str[ptr])) < 0) {  
		MSG("Failed to set edge!\n");  
		close(fd); 
		return -1;  
	}  

	close(fd);  
	return 0;  
}

//初始化按键1
void init_gpio1(void)
{
	MSG("\n =============== init_gpio1 start ========== \n");
	gpio_unexport(1);
	gpio_export(1);
	gpio_direction(1, 0);
	gpio_edge(1, 2);
	MSG("\n =============== init_gpio1 end ========== \n");
}

//初始化按键2
void init_gpio2(void)
{
	MSG("\n =============== init_gpio2 start ========== \n");
	gpio_unexport(2);
	gpio_export(2);
	gpio_direction(2, 0);
	gpio_edge(2, 2);
	MSG("\n =============== init_gpio2 end ========== \n");
}

/*串口设置*/
int uart1_config(int fd)
{
    struct termios newtio,oldtio;
    /*获取原有串口配置*/
    if  ( tcgetattr(fd, &oldtio)  !=  0) { 
        perror("SetupSerial 1");
        return -1;
    }
    memset( &newtio, 0, sizeof(newtio) );
    /*CREAD 开启串行数据接收，CLOCAL并打开本地连接模式*/
    newtio.c_cflag  |=  CLOCAL | CREAD;

    /*设置数据位8*/
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    /* 设置奇偶校验位 */
    newtio.c_cflag &= ~PARENB; //无奇偶校验
    /* 设置波特率 115200*/
	cfsetispeed(&newtio, B115200);
	cfsetospeed(&newtio, B115200);

    /*设置停止位*/
    newtio.c_cflag &=  ~CSTOPB;/*默认为一位停止位； */
    /*设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/
    newtio.c_cc[VTIME]  = 0;/*非规范模式读取时的超时时间；*/
    newtio.c_cc[VMIN] = 0;/*非规范模式读取时的最小字符数*/
    /*tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */
    tcflush(fd, TCIFLUSH);
    if((tcsetattr(fd, TCSANOW,&newtio))!=0) {
        perror("com set error");
        return -1;
    }
    return 0;
}
/**
	@berf uart send
	@param int fd: uart file descriptor
	@param void *buf:send data buf
	@param int len:data buf len
*/
int uart_send(int fd, void *buf, int len)
{
    int ret = 0;
    int count = 0;

    tcflush(fd, TCIFLUSH);

    while (len > 0) {
        ret = write(fd, (char*)buf + count, len);
        if (ret < 1) {
			printf("write data below 1 byte % d\r\n", ret);
            break;
        }
        count += ret;
        len = len - ret;
    }

    return count;
}
/**
	@berf uart read
	@param int uart_fd: uart file descriptor
	@param void *buf:read data buf
	@param int len:data buf len
	@param int timeout_ms: read data time
*/
int uart_read(int uart_fd, void *buf, int len, int timeout_ms)
{
    int ret;
    size_t  rsum = 0;
    ret = 0;
    fd_set rset;
    struct timeval time;

    while ((int)rsum < len) {
        time.tv_sec = timeout_ms/1000;
        time.tv_usec = (timeout_ms - time.tv_sec*1000)*1000;
        FD_ZERO(&rset);
        FD_SET(uart_fd, &rset);
        ret = select(uart_fd+1, &rset, NULL, NULL, &time);
        if (ret <= 0) {
            if (ret == 0) {
				printf("time over!\r\n");
                return -1;
            }
            if (ret == -1) {
				printf("select error!\r\n");
                // 信号中断
                continue;
            }
            return -1;
        } else {
            ret = read(uart_fd, (char *)buf + rsum, len - rsum);
            if (ret < 0){
				printf("read data failed\r\n");
                return ret;
            } else {
                rsum += ret;
            }
        }
    }

    return rsum;
}

/*
 *crc32 Verification implementation
*/
static const unsigned int crc32table[] = {
 0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
 0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
 0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
 0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
 0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
 0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
 0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
 0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
 0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
 0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
 0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
 0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
 0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
 0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
 0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
 0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
 0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
 0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
 0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
 0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
 0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
 0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};
 
/**
	@berf CRC check
	@param const unsigned char *buf: Data to be verified buff
	@param unsigned int size: Data to be verified length
*/
static unsigned int crc32( const unsigned char *buf, unsigned int size)
{
    unsigned int  i, crc = 0xFFFFFFFF;
 
    for (i = 0; i < size; i++) {
        crc = crc32table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    }
    return crc^0xFFFFFFFF;
}

/*hisignal Hi3516 message send*/
static unsigned int hisignalling_data_package(hisignalling_protocal_type *buf, unsigned int len, unsigned char *hisignalling_data_buf)
{
    unsigned int crc_check_send;
	unsigned int package_len = 0;

    memcpy(hisignalling_data_buf, buf->frame_header, HISGNALLING_MSG_FRAME_HEADER_LEN);
    memcpy(&hisignalling_data_buf[HISGNALLING_MSG_FRAME_HEADER_LEN], buf->hisignalling_msg_buf, len);
    memcpy(&hisignalling_data_buf[HISGNALLING_MSG_FRAME_HEADER_LEN + len], &(buf->end_of_frame), HISIGNALLING_MSG_HEADER_LEN);

    crc_check_send = crc32(hisignalling_data_buf, (len + HISIGNALLING_MSG_HEADER_TAIL_LEN));
    hisignalling_data_buf[len + HISIGNALLING_MSG_HEADER_TAIL_LEN] = 	(unsigned char)((crc_check_send & 0xff000000)>>24);
    hisignalling_data_buf[len + HISIGNALLING_MSG_HEADER_TAIL_LEN + 1] = (unsigned char)((crc_check_send & 0x00ff0000)>>16);
    hisignalling_data_buf[len + HISIGNALLING_MSG_HEADER_TAIL_LEN + 2] = (unsigned char)((crc_check_send & 0x0000ff00)>>8);
    hisignalling_data_buf[len + HISIGNALLING_MSG_HEADER_TAIL_LEN + 3] = (unsigned char)crc_check_send;
    package_len = len + HISIGNALLING_MSG_HEADER_TAIL_LEN + 4;
    return package_len;
}

/*hisignalling Hi3561 message recevice*/
// static hisignalling_error_type hisignalling_msg_receive(int fd, unsigned char *buf, unsigned int len)
// {
//     unsigned int crc_check_received;
// 	unsigned int i = 0,read_len = 0;
// 	/*Hi3516dv300 uart read*/
// 	read_len = uart_read(fd, buf, len, 1000);
// 	if (read_len <= 0) {
// 		printf("uart_read data failed\r\n");
// 		return HISIGNALLING_RET_VAL_ERROR;
// 	}
// 	printf("read_len=%d\r\n", read_len);
//     /*校验crc*/
// 	if (len > 4) {
// 		crc_check_received = crc32(buf, len-4);
// 		if (((unsigned char)((crc_check_received & 0xff000000)>>24) != buf[len-4]) &&
// 			((unsigned char)((crc_check_received & 0x00ff0000)>>16) != buf[len-3]) &&
// 			((unsigned char)((crc_check_received & 0x0000ff00)>>8)  != buf[len-2]) &&
// 			((unsigned char)crc_check_received != buf[len-1])) {
// 			printf("<Hi3516dv300>: crc32 Verification failed!\r\n");
// 			printf("<Hi3516dv300>: crc_4=0x%x, crc_3=0x%x, crc_2=0x%x, crc_1=0x%x\r\n", buf[len-4], buf[len-3], buf[len-2], buf[len-1]);
// 			return HISIGNALLING_RET_VAL_ERROR;
// 		}
// 	}
//     /*输出收到的数据*/ 
//     for (i = 0; i < len; i++) {
//         printf("0x%x ", buf[i]);
//     }
//     printf("\r\n");

//     return HISIGNALLING_RET_VAL_CORRECT;
// }

/**
	@berf hisignalling protocol send msg
	@param void *buf: send data buff
	@param unsigned int data_len: send data length
*/
static unsigned int hisignalling_msg_send(void *buf, unsigned int data_len)
{
	// unsigned int ret;
    hisignalling_protocal_type hisignalling_msg;
    // unsigned char *hisignalling_send_buf = NULL;
    // unsigned int hisignalling_package_len = 0;
    //unsigned int write_data_len = 0;

    hisignalling_msg.frame_header[0]= 0xAA;
    hisignalling_msg.frame_header[1]= 0x55; 
    memcpy(hisignalling_msg.hisignalling_msg_buf, buf, data_len);
    hisignalling_msg.end_of_frame = 0xff;

    hisignalling_package_len = hisignalling_data_package(&hisignalling_msg, data_len, hisignalling_send_buf);
	if (!hisignalling_package_len) {
        printf("hisignalling_data_package failed = %d\r\n", hisignalling_package_len);
        return -1;
    }
    if (*hisignalling_send_buf == 0) {
        printf("hisignalling send buf is null!\r\n");
        return -1;
    }
    
	// ret = uart_send(fd, hisignalling_send_buf, hisignalling_package_len);
	// if (ret < 0) {
	// 	printf("write data failed\r\n");
	// 	return -1;
	// }
	return 0;
}
/**
	@berf switch1 trigger and send message
	@param unsigned char *write_buffer: send data buff
*/
unsigned int switch_1_trigger_send_msg(unsigned char *write_buffer)
{
	// int ret2     = -1;
	// int gpio2_fd = -1;
    // char buff[10] = {0};
	// struct pollfd fds2[1];
    // unsigned char *hisignalling_package_ptr;
	/*按键操作*/
	// init_gpio2();

    // gpio2_fd = open("/sys/class/gpio/gpio2/value", O_RDONLY);
	// if (gpio2_fd < 0)
	// {
	// 	MSG("Failed to open gpio1 !\n");
	// }
	// fds2[0].fd     = gpio2_fd;
	// fds2[0].events = POLLPRI;

	// ret2 = read(gpio2_fd, buff, 10);
	// if (ret2 == -1){
	// 	MSG("gpio2 read error\n");
	// }
	
	// ret2 = poll(fds2, 1, 0);
	// if (ret2 == -1){
	// 	MSG("gpio2 poll error\n");
	// }
	
	// if (fds2[0].revents & POLLPRI)
    {
		// ret2 = lseek(gpio2_fd, 0, SEEK_SET);
		// if (ret2 == -1){
		// 	MSG("gpio1 lseek error\n");
		// }
		// MSG("sw1 Pressed \n");
		/*按键触发发送*/
		hisignalling_msg_send(write_buffer, sizeof(write_buffer)/sizeof(write_buffer[0]));			
	}
    return 0;
}
/**
	@berf switch2 trigger and send message
	@param unsigned char *write_buffer2: send data buff
*/
unsigned int switch_2_trigger_send_msg(unsigned char *write_buffer2)
{
	// int ret1 	 = -1;
    // int gpio1_fd = -1;
    // char buff[10] = {0};
    // struct pollfd fds1[1];
    // // unsigned char *hisignalling_package_2_ptr;
	// init_gpio1();

    // gpio1_fd = open("/sys/class/gpio/gpio1/value", O_RDONLY);
	// if (gpio1_fd < 0)
	// {
	// 	MSG("Failed to open gpio1 !\n");
	// }
	// fds1[0].fd     = gpio1_fd;
	// fds1[0].events = POLLPRI;

	// /*按键操作*/
	// ret1 = read(gpio1_fd, buff, 10);
	// if (ret1 == -1){
	// 	MSG("gpio1 read error\n");
	// }

	// ret1 = poll(fds1, 1, 0);
	// if (ret1 == -1){
	// 	MSG("gpio1 poll error\n");
	// }

	// if (fds1[0].revents & POLLPRI)
    {
		// ret1 = lseek(gpio1_fd, 0, SEEK_SET);
		// if (ret1 == -1){
		// 	MSG("gpio1 lseek error\n");
		// }
		// MSG("sw2 Pressed \n");
		/*按键触发发送*/
	    hisignalling_msg_send(write_buffer2, sizeof(write_buffer2)/sizeof(write_buffer2[0]));
	}
    return 0;
}














