#ifndef _dspi_dotprod_platform_H_
#define _dspi_dotprod_platform_H_

#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ARCH_XTENSA
#include <xtensa/config/core-isa.h>
#include <xtensa/config/core-matmap.h>
#endif


#if CONFIG_IDF_TARGET_ESP32S3
#define dspi_dotprod_aes3_enabled 1
#endif 

#endif // _dspi_dotprod_platform_H_
