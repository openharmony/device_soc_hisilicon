/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <poll.h>

int uartFd = 0;

/* 串口设置 */
int Uart1Config(int fd)
{
    struct termios newtio = {0}, oldtio = {0};
    /* 获取原有串口配置 */
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("SetupSerial 1");
        return -1;
    }
    (void)memset_s(&newtio, sizeof(newtio), 0, sizeof(newtio));
    /* CREAD 开启串行数据接收，CLOCAL并打开本地连接模式 */
    newtio.c_cflag  |=  CLOCAL | CREAD;

    /* 设置数据位8 */
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    /* 设置奇偶校验位 */
    newtio.c_cflag &= ~PARENB; // 无奇偶校验
    /* 设置波特率 115200 */
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);

    /* 设置停止位 */
    newtio.c_cflag &=  ~CSTOPB; /* 默认为一位停止位 */
    /* 设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时 */
    newtio.c_cc[VTIME]  = 0; /* 非规范模式读取时的超时时间 */
    newtio.c_cc[VMIN] = 0; /* 非规范模式读取时的最小字符数 */
    /* tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
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
int UartSend(int fd, char *buf, int len)
{
    int ret = 0;
    int count = 0;
    char *sendBuf = buf;
    int sendLen = len;

    tcflush(fd, TCIFLUSH);

    while (sendLen > 0) {
        ret = write(fd, (char*)sendBuf + count, sendLen);
        if (ret < 1) {
            printf("write data below 1 byte % d\r\n", ret);
            break;
        }
        count += ret;
        sendLen = sendLen - ret;
    }

    return count;
}
/**
    @berf uart read
    @param int uart_fd: uart file descriptor
    @param void *buf:read data buf
    @param int len:data buf len
    @param int timeoutMs: read data time
*/
int UartRead(int uartFd, char *buf, int len, int timeoutMs)
{
    int ret = 0;
    size_t  rsum = 0;
    ret = 0;
    fd_set rset;
    struct timeval time;
    int timeout = timeoutMs;
    char *readBuf = buf;
    int readLen = len;

    while (rsum < readLen) {
        time.tv_sec = timeout / 1000; /* 1000:转换成秒 */
        time.tv_usec = (timeout - time.tv_sec * 1000) * 1000; /* 1000, 1000:转换为微秒 */
        FD_ZERO(&rset);
        FD_SET(uartFd, &rset);
        ret = select(uartFd + 1, &rset, NULL, NULL, &time); // 非阻塞式读取数据
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
            ret = read(uartFd, (char *)readBuf + rsum, readLen - rsum);
            if (ret < 0) {
                printf("read data failed\r\n");
                return ret;
            } else {
                rsum += ret;
            }
        }
    }
    return rsum;
}

unsigned int UartOpenInit(void)
{
    int fd;
    char *uart1 = "/dev/ttyAMA1";

    if ((fd = open(uart1, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        printf("open %s is failed", uart1);
        return -1;
    } else {
        Uart1Config(fd);
    }
    return fd;
}

void UartClose(int fd)
{
    close(fd);
}

int main(void)
{
    char *gestureName = NULL;
    gestureName = "gesture first !!!!!!!!"; /* 对应手势里面字符串 */
    unsigned char uartReadBuff[2048] = {0}; /* 读取buff大小为2048 */
    unsigned int RecvLen = strlen(gestureName);
    unsigned int ret = 0;
    int readLen = 0;
    /* 串口初始化定义 */
    uartFd = UartOpenInit();
    if (uartFd < 0) {
        printf("uart1 open failed\r\n");
    } else {
        printf("uart1 open successed\r\n");
    }
    while (1) {
        ret = UartSend(uartFd, (unsigned char*)gestureName, strlen(gestureName));
        readLen = UartRead(uartFd, uartReadBuff, RecvLen, 1000); /* 1000 :time out */
        if (readLen > 0) {
            printf("Uart read data:%s\r\n", uartReadBuff);
        }
        if (getchar() == 'q') { /* 退出AI时, 调用clsoe函数 */
            close(uartFd);
            break;
        }
    }
    return 0;
}