#ifndef __HASH_CMD_H
#define __HASH_CMD_H

#include <stdint.h>

#define HASH_MAX 10  //数组大小
#define HASH_DETECT_TIME 5 //二次检测次数

#define GET_STRUCT_ADDR(ptr, type, member) ((type *)((char *)(ptr) - (size_t)&(((type*)0)->member)))

typedef enum {
    successful_execution=0, //成功执行
    successful_register,    //成功注册
    cmd_not_registered,     //命令没注册
    multiple_register,      //重复注册
    not_found_empty_space,  //找不到哈希表空间了
}hash_code;

uint8_t register_cmd(char * str,void (*func)(void));
uint8_t execute_cmd(char * str);

#endif