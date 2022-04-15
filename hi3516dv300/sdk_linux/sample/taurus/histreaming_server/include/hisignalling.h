/**/
#ifndef __HISIGNALLING_H_
#define __HISIGNALLING_H_
#include <stdio.h>
#define HISIGNALLING_MSG_HEADER_LEN         (1)
#define HISGNALLING_MSG_FRAME_HEADER_LEN    (2)
#define HISIGNALLING_MSG_HEADER_TAIL_LEN    (3)
#define HISGNALLING_FREE_TASK_TIME          (10)
#define HISIGNALLING_MSG_MOTOR_ENGINE_LEN	(11)
#define HISIGNALLING_MSG_ONE_FRAME_LEN		(16)
#define HISIGNALLING_MSG_BUFF_LEN           (512)

extern unsigned char hisignalling_send_buf[HISIGNALLING_MSG_BUFF_LEN];
extern unsigned int hisignalling_package_len;

/**
*@brief Adapter plate selection
*使用时选择打开宏，使用外设扩展板打开#define BOARD_SELECT_IS_EXPANSION_BOARD这个宏，使用Robot板打开#define BOARD_SELECT_IS_ROBOT_BOARD这个宏
**/
#define BOARD_SELECT_IS_ROBOT_BOARD
//#define BOARD_SELECT_IS_EXPANSION_BOARD
#ifdef BOARD_SELECT_IS_EXPANSION_BOARD
#define EXPANSION_BOARD
#else 
#define ROBOT_BOARD
#endif

typedef struct hisignalling_protocal {
    unsigned char frame_header[HISGNALLING_MSG_FRAME_HEADER_LEN];
    unsigned char hisignalling_msg_buf[HISIGNALLING_MSG_BUFF_LEN];
    unsigned int hisigalling_msg_len;
    unsigned char end_of_frame;
    unsigned int hisignalling_crc32_check; 
}hisignalling_protocal_type;

typedef enum hisignalling_return_val{
    HISIGNALLING_RET_VAL_CORRECT = 0,
    HISIGNALLING_RET_VAL_ERROR,
    HISGNALLING_RET_VAL_MAX
}hisignalling_error_type;
/*
*hisignalling protocal Function declaration
*/
unsigned int switch_1_trigger_send_msg(unsigned char *write_buffer);
// unsigned int switch_2_trigger_send_msg(unsigned char *write_buffer2);
#endif