#ifndef __KEY_H_
#define __KEY_H_
//实现按键FIFO输入

#include <stdint.h>

#define KEY_NUM 4 //硬件按键数量，若修改要修改相应enum
#define KEY_FIFO_SIZE 10
#define KEY_FILTER_TIME 10 //滤波计数器

typedef struct
{
    uint8_t buffer[KEY_FIFO_SIZE];//缓冲区
    uint8_t read;//缓冲区读指针
    uint8_t write;//缓冲区写指针
}KEY_FIFO_Type;

//用枚举enum定义键值,不用define是为了便于修改
typedef enum
{
    KEY_NONE = 0,//无按键事件

    KEY_1_UP,
    KEY_1_DOWN,
    KEY_1_LONG,

    KEY_2_UP,
    KEY_2_DOWN,
    KEY_2_LONG,

    KEY_3_UP,
    KEY_3_DOWN,
    KEY_3_LONG,

    KEY_4_UP,
    KEY_4_DOWN,
    KEY_4_LONG,
}KEY_ENUM;

//按键状态结构体
typedef struct
{
    //函数指针指向判断按键是否按下的函数,按下返回1
    uint8_t (*IsKeyDownFunc)(void);
    
    uint8_t count;//滤波计数器

    uint8_t state;//按键状态，0未按下，1按下

}KEY_Type;


void KEY_FIFO_put(uint8_t _keyCode);
uint8_t KEY_FIFO_get(void);

void KEY_Init(void);
void KEY_FIFO_Init(void);
void KEY_GPIO_Config(void);

uint8_t IsKey1Down(void);
uint8_t IsKey2Down(void);
uint8_t IsKey3Down(void);
uint8_t IsKey4Down(void);

void KEY_Scan(void);
void KEY_Detect(uint8_t idx);

#endif

