#ifndef _wifi_h
#define _wifi_h
#include "zf_common_headfile.h"
#include "math.h"

#define WIFI_SSID_TEST          "YOUR_WIFI_SSID"
#define WIFI_PASSWORD_TEST      "YOUR_WIFI_PASSWORD"  // Replace with local test credentials, or set to NULL for an open network.

//void wifi_sand(void);
void wifi_set_parameter(void);
void wifi_part(void);

#endif