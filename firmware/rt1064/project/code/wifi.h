#ifndef _wifi_h
#define _wifi_h
#include "zf_common_headfile.h"
#include "math.h"

#define WIFI_SSID_TEST          "11223344"
#define WIFI_PASSWORD_TEST      "20040718"  // 如果需要连接的WIFI 没有密码则需要将 "12345678" 替换为 NULL

//void wifi_sand(void);
void wifi_set_parameter(void);
void wifi_part(void);

#endif