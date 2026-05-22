#include "beep.h"
#include "camera.h"
#include "ips200.h"

void beep_ctr(int mode)
{
	if(mode==1){gpio_set_level(BEEP, GPIO_HIGH);}// BEEP Ïì
	else if(mode==0){gpio_set_level(BEEP, GPIO_LOW);}// BEEP Í£
}

