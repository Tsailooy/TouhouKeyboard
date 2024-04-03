#include "main.h"
#include "key.h"

static KEY_FIFO_Type KeyFIFO;//����FIFO�ṹ�����
static KEY_Type Keys[KEY_NUM] = {0};//���尴������Ľṹ������

//д��һ����
void KEY_FIFO_put(uint8_t _keyCode)
{
    KeyFIFO.buffer[KeyFIFO.write] = _keyCode;//�Ѱ�������д�뻺������дָ���λ��
    KeyFIFO.write++;//дָ��+1
    if(KeyFIFO.write >= KEY_FIFO_SIZE)
    {
        KeyFIFO.write = 0;//��дָ�볬����������Χ�����
    }
}

//��ȡһ����
uint8_t KEY_FIFO_get(void)
{
    uint8_t ret;//����ֵ

    if(KeyFIFO.read == KeyFIFO.write)
    {
        return KEY_NONE;
    }//дָ�����ָ���ص�˵��û�а���ֵ�ˣ�����KEY_NONE
    else
    {
        ret = KeyFIFO.buffer[KeyFIFO.read];//��ȡ�������ж�ָ��λ�õļ�ֵ
        KeyFIFO.read++;//дָ��+1
        if(KeyFIFO.read >= KEY_FIFO_SIZE)
        {
            KeyFIFO.read = 0;//��дָ�볬����������Χ�����
        }
        return ret;//���ؼ�ֵ
    }
}

void KEY_Init(void)
{
    KEY_FIFO_Init();//��ʼ����������ṹ�����
    KEY_GPIO_Config();//���ð�����GPIO
}

static void KEY_FIFO_Init(void)
{
    uint8_t idx;

    //��ʼ��buffer����ָ��
    KeyFIFO.read = 0;
    KeyFIFO.write = 0;

    //��ʼ����������ṹ��
    for(idx = 0;idx < KEY_NUM;idx++)
    {
        Keys[idx].count = 5/2;//������Ϊ�˲�ʱ���һ�� ע��count��u8���� 5/2=2
        Keys[idx].state = 0;//0��ʾδ����
    }

    //������⺯��ָ��
    Keys[0].IsKeyDownFunc = IsKey1Down;
    Keys[1].IsKeyDownFunc = IsKey2Down;
    Keys[2].IsKeyDownFunc = IsKey3Down;
    Keys[3].IsKeyDownFunc = IsKey4Down;
}

//������⺯������ȡGPIO����
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

//���ð�����GPIO��
static void KEY_GPIO_Config(void)
{
    //��ʼ����Ϣ�ṹ��
    GPIO_InitTypeDef gpioinitstruct;

    //��GPIOʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //������������ģʽ
    gpioinitstruct.Mode = GPIO_MODE_INPUT;
    gpioinitstruct.Pull = GPIO_PULLUP;
    gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

    //ʹ�ܸ�������
    gpioinitstruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);

    gpioinitstruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);
}

//��systick�жϷ��������ִ�е�RunPer10ms()����
void RunPer10ms(void)
{
    KEY_Scan();
}

//ÿ10msɨ��һ�鰴��
void KEY_Scan(void)
{
    uint8_t idx;
    for(idx = 0;idx < KEY_NUM;idx++)
    {
        KEY_Detect(idx);
    }
}

//������⺯��
static void KEY_Detect(uint8_t idx)
{
    KEY_Type *pKeys;
    pKeys = &Keys[idx];//ָ��ָ��Keys�ṹ��

    if(pKeys->IsKeyDownFunc() == 1)//��������
    {
        if(pKeys->count < KEY_FILTER_TIME)
        {
            pKeys->count = KEY_FILTER_TIME;   
        }
        else if(pKeys->count < 2*KEY_FILTER_TIME)
        {
            pKeys->count++;
        }//�ӳ�һ���˲�ʱ�䣬���жϰ���
        else
        {
            if(pKeys->state == 0)
            {
                pKeys->state = 1;
                KEY_FIFO_put((uint8_t)3*idx+2);//ö�����͵õ���ֵ,+2Ϊ����
            }//������δ�������Ͱ�����Ϣ

        }
    }
    else //�����ɿ�����û�а���
    {
        if(pKeys->count > KEY_FILTER_TIME)
        {
            pKeys->count = KEY_FILTER_TIME;
        }
        else if(pKeys->count != 0)
        {
            pKeys->count--;
        }//ͬ��Ҳ���ӳ�һ���˲�ʱ��
        else //��count������Ͱ����ɿ�����Ϣ
        {
            if(pKeys->state == 1)
            {
                pKeys->state = 0;
                KEY_FIFO_put((uint8_t)3*idx+1);//+1Ϊ�ɿ�����
            }
        }
    }
}

