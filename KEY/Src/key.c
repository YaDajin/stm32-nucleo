#include "key.h"
#include "gpio.h"
/**
 * 按键处理函数
 * mode：0:不支持连续按；1:支持连续按
 * 返回按键状态
 */
int KeyScan(int mode)
{
	static int key_up=1; //按键松开标志
	if (mode==1)key_up=1;//支持连按
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
	return KEY_UP;//按键弹起
}