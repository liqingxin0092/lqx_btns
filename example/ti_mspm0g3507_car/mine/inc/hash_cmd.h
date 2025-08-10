#ifndef __HASH_CMD_H
#define __HASH_CMD_H

#include <stdint.h>

#define HASH_MAX 10  //�����С
#define HASH_DETECT_TIME 5 //���μ�����

#define GET_STRUCT_ADDR(ptr, type, member) ((type *)((char *)(ptr) - (size_t)&(((type*)0)->member)))

typedef enum {
    successful_execution=0, //�ɹ�ִ��
    successful_register,    //�ɹ�ע��
    cmd_not_registered,     //����ûע��
    multiple_register,      //�ظ�ע��
    not_found_empty_space,  //�Ҳ�����ϣ��ռ���
}hash_code;

uint8_t register_cmd(char * str,void (*func)(void));
uint8_t execute_cmd(char * str);

#endif