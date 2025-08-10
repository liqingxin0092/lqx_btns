#include "hmi.h"
#include "lqx_btns.h"
#include "motor.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include <stdio.h>
#include "uart.h"

/*mode��Χ*/
//0~MODE_MAX

#define MODE_MAX 5

volatile menu_t menu;

// key1:�ı�ģʽ
//mode: 0,�趨Ȧ����   key2����:Ȧ�� ,key3:�� (base1),key5:(base2),key6:(base3) 
//    : 1,����ݮ�ɷ�abc, key2:A ,key3:B.  key5: fahui1
void change_mode(void)
{
    if(++menu.mode>MODE_MAX)
         menu.mode=0;
}

key_code_e key_code=0;

timer_t car_delay_start_timer;
void hmi_task(void)
{ 
    char string[50];
    if(get_key_event(&key_code))
    {
         switch(key_code)
         {
             /* key1 */
             case  key0_pressed:  
                   change_mode(); //�ı�ģʽ
             break;
             
             /* key2 */
             case  key1_pressed: 
                   switch (menu.mode)
                   {
                       case 0:
                           menu.turn_point_num=5;
                           menu.num_of_turns=1;
                           break;
                       case 1:                           
                           uart2_send("A\r\n",3);    
                           break; 
                   }
             break;
             case  key1_double_combo:  
                   switch (menu.mode)
                   {
                       case 0:
                       menu.num_of_turns=2;
                       menu.turn_point_num=9;
                       break;
                   }
             break;
             case  key1_treble_combo:            
                   switch (menu.mode)
                   {
                       case 0:
                       menu.num_of_turns=3;
                       menu.turn_point_num=13;
                       break;
                   }
             break;
             case  key1_four_hit_combo:     
                   switch (menu.mode)
                   {
                       case 0:
                           menu.num_of_turns=4;
                       menu.turn_point_num=17;
                       break;
                   }
             break;
             case  key1_five_hit_combo:            
                   switch (menu.mode)
                   {
                       case 0:
                       menu.num_of_turns=5;
                       menu.turn_point_num=21;
                       break;
                   }
             break;
                   
             /*  key3  */ 
             case  key2_pressed:  
                   switch (menu.mode)
                   {
                       case 0:
                           set_gloab_speed(1);   //���ٶ�
                           car_instance.cur_turn_point_num=0;//��ǰ�յ�����0
                           menu.mode_0_start=1;  //����һ�αȽ�
                       break;
                       case 1:
                           uart2_send("B\r\n",3);
                           break;
                   }
             break;
             
             /* key5 */
             case  key3_pressed: 
                   switch (menu.mode)
                   {
                       case 0:
                           while(1)
                           {
                                uart2_send("a\r\n",3);
                           }
                           break;
                       case 1:
                            menu.fahui1_start=1;  //����1ģʽ
                            car_instance.cur_turn_point_num=0;//��ǰ�յ�����0
                            menu.mode_0_start=1;  //����һ�αȽ�
                            menu.turn_point_num=5;//Ŀ��Ȧ����ֵ
                            uart2_send("A\r\n",3); //׷����
                            car_delay_start_timer.is_start=1; //��ʱ���ٶ�
                            break;
                   }
             break;
             
             /* key6 */
             case  key4_pressed:  
                   switch (menu.mode)
                   {
                       case 0:
                           while(1)
                           {
                                uart2_send("b\r\n",3);
                           }
                           break;
                   }
             break;
         }
    } 
    
    sprintf(string,"m:%d,n:%d,q:%d   ",menu.mode,menu.num_of_turns,car_instance.cur_turn_point_num);
    Gui_DrawFont_GBK16(0, 0, YELLOW,BLACK, (uint8_t*)string);
}
