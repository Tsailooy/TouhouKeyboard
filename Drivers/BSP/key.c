#include "main.h"
#include "key.h"

static KEY_FIFO_Type KeyFIFO;//定义FIFO结构体变量
static KEY_Type Keys[KEY_NUM] = {0};//定义按键对象的结构体数组

//写入一个键
void KEY_FIFO_put(uint8_t _keyCode)
{
    KeyFIFO.buffer[KeyFIFO.write] = _keyCode;//把按键代码写入缓冲区中写指针的位置
    KeyFIFO.write++;//写指针+1
    if(KeyFIFO.write >= KEY_FIFO_SIZE)
    {
        KeyFIFO.write = 0;//若写指针超过缓冲区范围则归零
    }
}

//读取一个键
uint8_t KEY_FIFO_get(void)
{
    uint8_t ret;//返回值

    if(KeyFIFO.read == KeyFIFO.write)
    {
        return KEY_NONE;
    }//写指针与读指针重叠说明没有按键值了，返回KEY_NONE
    else
    {
        ret = KeyFIFO.buffer[KeyFIFO.read];//读取缓冲区中读指针位置的键值
        KeyFIFO.read++;//写指针+1
        if(KeyFIFO.read >= KEY_FIFO_SIZE)
        {
            KeyFIFO.read = 0;//若写指针超过缓冲区范围则归零
        }
        return ret;//返回键值
    }
}

void KEY_Init(void)
{
    KEY_FIFO_Init();//初始化按键对象结构体变量
    KEY_GPIO_Config();//配置按键与GPIO
}

static void KEY_FIFO_Init(void)
{
    uint8_t idx;

    //初始化buffer区的指针
    KeyFIFO.read = 0;
    KeyFIFO.write = 0;

    //初始化按键对象结构体
    for(idx = 0;idx < KEY_NUM;idx++)
    {
        Keys[idx].count = 5/2;//计数器为滤波时间的一半 注意count是u8类型 5/2=2
        Keys[idx].state = 0;//0表示未按下
    }

    //按键检测函数指针
    Keys[0].IsKeyDownFunc = IsKey1Down;
    Keys[1].IsKeyDownFunc = IsKey2Down;
    Keys[2].IsKeyDownFunc = IsKey3Down;
    Keys[3].IsKeyDownFunc = IsKey4Down;
}

//按键检测函数，读取GPIO引脚
static uint8_t IsKey1Down(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_RESET)
        return 1;
    else 
        return 0;

}

static uint8_t IsKey2Down(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == GPIO_PIN_RESET)
        return 1;
    else 
        return 0;
}

static uint8_t IsKey3Down(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET)
        return 1;
    else 
        return 0;
}

static uint8_t IsKey4Down(void)
{
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6) == GPIO_PIN_RESET)
        return 1;
    else 
        return 0;
}

//配置按键与GPIO口
static void KEY_GPIO_Config(void)
{
    //初始化信息结构体
    GPIO_InitTypeDef gpioinitstruct;

    //打开GPIO时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //上拉电阻输入模式
    gpioinitstruct.Mode = GPIO_MODE_INPUT;
    gpioinitstruct.Pull = GPIO_PULLUP;
    gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

    //使能各个引脚
    gpioinitstruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);
}

//在systick中断服务程序里执行的RunPer10ms()函数
void RunPer10ms(void)
{
    KEY_Scan();
}

//每10ms扫描一遍按键
void KEY_Scan(void)
{
    uint8_t idx;
    for(idx = 0;idx < KEY_NUM;idx++)
    {
        KEY_Detect(idx);
    }
}

//按键检测函数
static void KEY_Detect(uint8_t idx)
{
    KEY_Type *pKeys;
    pKeys = &Keys[idx];//指针指向Keys结构体

    if(pKeys->IsKeyDownFunc() == 1)//按键按下
    {
        if(pKeys->count < KEY_FILTER_TIME)
        {
            pKeys->count = KEY_FILTER_TIME;   
        }
        else if(pKeys->count < 2*KEY_FILTER_TIME)
        {
            pKeys->count++;
        }//延迟一个滤波时间，再判断按键
        else
        {
            if(pKeys->state == 0)
            {
                pKeys->state = 1;
                KEY_FIFO_put((uint8_t)3*idx+2);//枚举类型得到键值,+2为按下
            }//若按键未按下则发送按下信息

        }
    }
    else //按键松开或者没有按下
    {
        if(pKeys->count > KEY_FILTER_TIME)
        {
            pKeys->count = KEY_FILTER_TIME;
        }
        else if(pKeys->count != 0)
        {
            pKeys->count--;
        }//同理也是延迟一个滤波时间
        else //当count归零后发送按键松开的消息
        {
            if(pKeys->state == 1)
            {
                pKeys->state = 0;
                KEY_FIFO_put((uint8_t)3*idx+1);//+1为松开按键
            }
        }
    }
}

