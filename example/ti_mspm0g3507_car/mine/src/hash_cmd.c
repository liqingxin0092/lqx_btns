#include "hash_cmd.h"
#include <stdint.h>

/*��ϣ��*/
struct hash_struct{
    uint32_t hash2;
    void (*func)(void);
};
struct hash_struct hash_table[HASH_MAX];

//Ѱַ
static uint8_t hash_func1(const char *str) {
    int temp = 0;
    while (*str) {
        temp += *str++;
    }
    return temp ;
}

//��ʶ
static uint32_t hash_func2(const char *str) {
    uint32_t temp = 0xdead;  // ��ʼֵ
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

/*ע������*/
uint8_t register_cmd(char * str,void (*func)(void)) {
    int i = 1;
    uint8_t temp=hash_func1(str)%HASH_MAX;
    uint8_t h1=temp;
    uint32_t h2=hash_func2(str);

    /*���h1��ע����*/
    while (hash_table[h1].func != 0)
    {
        h1=temp;
        /*���h2��ͬ*/
        if (hash_table[h1].hash2 == h2)
        {
            /*�ظ�ע����*/
            return multiple_register;
        }
        h1 += i ;
        h1 %= HASH_MAX;
        /*�޸�h1����û��ע��*/
        if (hash_table[h1].func == 0) {
            /*ע��������*/
            hash_table[h1].hash2 = h2;
            hash_table[h1].func = func;
            return successful_register;
        }
        /*�޸���֮���Ǳ�ע����*/
        /*���޸�*/
        h1 -= i<<1 ;
        h1 %= HASH_MAX;
        i++;
        if (i==HASH_DETECT_TIME) {
            return not_found_empty_space;
        }
    }
    /*h1λ��û��ע��*/
    /*ע��������*/
    hash_table[h1].hash2 = h2;
    hash_table[h1].func = func;
    return successful_register;
}

/*�������ִ�����Ӧ�ĺ���*/
uint8_t execute_cmd(char * str) 
{
    uint8_t i=1;
    uint8_t temp=hash_func1(str)%HASH_MAX;
    uint8_t h1=temp;
    uint32_t h2=hash_func2(str);

    /*���h1��ע����*/
    while (hash_table[h1].func != 0)
    {
        h1=temp;
        /*���h2��ͬ*/
        if (hash_table[h1].hash2 == h2)
        {
            /*ִ�к���*/
            hash_table[h1].func();
            return successful_execution;
        }
        /*�������*/
        h1 += i ;
        h1 %= HASH_MAX;
        /*��� �������λ��ע����*/
        if (hash_table[h1].func != 0 ) {
            /*hash2��ͬ*/
            if (hash_table[h1].hash2 == h2) {
                /*ִ�к���*/
                hash_table[h1].func();
                return successful_execution;
            }
            /*hash��ͬ*/
            /*�ٵ���*/
            h1 -= i<<1;
            h1 %= HASH_MAX;
            if (hash_table[h1].hash2 == h2)
            {
                /*ִ�к���*/
                hash_table[h1].func();
                return successful_execution;
            }
            i++;
            if (HASH_DETECT_TIME==i) {
                return cmd_not_registered;
            }
        }
        /*+�������λ��û��ע��*/
        else {
            return cmd_not_registered;
        }
    }
    /*h1���λ��ûע��*/
    return cmd_not_registered;
}