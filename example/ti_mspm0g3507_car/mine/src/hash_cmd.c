#include "hash_cmd.h"
#include <stdint.h>

/*哈希表*/
struct hash_struct{
    uint32_t hash2;
    void (*func)(void);
};
struct hash_struct hash_table[HASH_MAX];

//寻址
static uint8_t hash_func1(const char *str) {
    int temp = 0;
    while (*str) {
        temp += *str++;
    }
    return temp ;
}

//标识
static uint32_t hash_func2(const char *str) {
    uint32_t temp = 0xdead;  // 初始值
    uint8_t i=0;
    while (*str) {
        if (i%2==0) {
            temp+=*str<<i;
        }
        else {
            temp-=*str>>i;
        }
        str++;
        i++;
    }
    return temp;
}

/*注册命令*/
uint8_t register_cmd(char * str,void (*func)(void)) {
    int i = 1;
    uint8_t temp=hash_func1(str)%HASH_MAX;
    uint8_t h1=temp;
    uint32_t h2=hash_func2(str);

    /*如果h1被注册了*/
    while (hash_table[h1].func != 0)
    {
        h1=temp;
        /*如果h2相同*/
        if (hash_table[h1].hash2 == h2)
        {
            /*重复注册了*/
            return multiple_register;
        }
        h1 += i ;
        h1 %= HASH_MAX;
        /*修改h1后发现没被注册*/
        if (hash_table[h1].func == 0) {
            /*注册新命令*/
            hash_table[h1].hash2 = h2;
            hash_table[h1].func = func;
            return successful_register;
        }
        /*修改了之后还是被注册了*/
        /*再修改*/
        h1 -= i<<1 ;
        h1 %= HASH_MAX;
        i++;
        if (i==HASH_DETECT_TIME) {
            return not_found_empty_space;
        }
    }
    /*h1位置没被注册*/
    /*注册新命令*/
    hash_table[h1].hash2 = h2;
    hash_table[h1].func = func;
    return successful_register;
}

/*查找命令并执行相对应的函数*/
uint8_t execute_cmd(char * str) 
{
    uint8_t i=1;
    uint8_t temp=hash_func1(str)%HASH_MAX;
    uint8_t h1=temp;
    uint32_t h2=hash_func2(str);

    /*如果h1被注册了*/
    while (hash_table[h1].func != 0)
    {
        h1=temp;
        /*如果h2相同*/
        if (hash_table[h1].hash2 == h2)
        {
            /*执行函数*/
            hash_table[h1].func();
            return successful_execution;
        }
        /*否则调整*/
        h1 += i ;
        h1 %= HASH_MAX;
        /*如果 调整后的位置注册了*/
        if (hash_table[h1].func != 0 ) {
            /*hash2相同*/
            if (hash_table[h1].hash2 == h2) {
                /*执行函数*/
                hash_table[h1].func();
                return successful_execution;
            }
            /*hash不同*/
            /*再调整*/
            h1 -= i<<1;
            h1 %= HASH_MAX;
            if (hash_table[h1].hash2 == h2)
            {
                /*执行函数*/
                hash_table[h1].func();
                return successful_execution;
            }
            i++;
            if (HASH_DETECT_TIME==i) {
                return cmd_not_registered;
            }
        }
        /*+调整完的位置没被注册*/
        else {
            return cmd_not_registered;
        }
    }
    /*h1这个位置没注册*/
    return cmd_not_registered;
}