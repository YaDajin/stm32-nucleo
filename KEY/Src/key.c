#include "key.h"
#include "gpio.h"
/**
 * ����������
 * mode��0:��֧����������1:֧��������
 * ���ذ���״̬
 */
int KeyScan(int mode)
{
	static int key_up=1; //�����ɿ���־
	if (mode==1)key_up=1;//֧������
	if (key_up && (KEY==0))
	{
		HAL_Delay(10);
		key_up=0;
		return KEY_DOWN;
	}
	else if(KEY==1)
	{
		key_up=1;
	}
	return KEY_UP;//��������
}