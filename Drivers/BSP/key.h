#ifndef __KEY_H_
#define __KEY_H_
//ʵ�ְ���FIFO����

#include <stdint.h>

#define KEY_NUM 4 //Ӳ���������������޸�Ҫ�޸���Ӧenum
#define KEY_FIFO_SIZE 10
#define KEY_FILTER_TIME 10 //�˲�������

typedef struct
{
    uint8_t buffer[KEY_FIFO_SIZE];//������
    uint8_t read;//��������ָ��
    uint8_t write;//������дָ��
}KEY_FIFO_Type;

//��ö��enum�����ֵ,����define��Ϊ�˱����޸�
typedef enum
{
    KEY_NONE = 0,//�ް����¼�

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

//����״̬�ṹ��
typedef struct
{
    //����ָ��ָ���жϰ����Ƿ��µĺ���,���·���1
    uint8_t (*IsKeyDownFunc)(void);
    
    uint8_t count;//�˲�������

    uint8_t state;//����״̬��0δ���£�1����

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

