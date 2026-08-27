#ifndef _DRIVER_COMMON_H_
#define _DRIVER_COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*******************************************************************************
* Definitions
******************************************************************************/

/**
 * @brief Common Status Error Code in Driver
 */
#define ARM_DRIVER_OK                     0              /* Operation succeeded */
#define ARM_DRIVER_ERROR                 -1              /* Unspecified error */
#define ARM_DRIVER_ERROR_BUSY            -2              /* Driver is busy */
#define ARM_DRIVER_ERROR_TIMEOUT         -3              /* Timeout occurred */
#define ARM_DRIVER_ERROR_UNSUPPORTED     -4              /* Operation not supported */
#define ARM_DRIVER_ERROR_PARAMETER       -5              /* Parameter error */
#define ARM_DRIVER_ERROR_SPECIFIC        -6              /* Start of driver specific errors */

#define ARM_DRIVER_VERSION_MAJOR_MINOR(major,minor) (((major) << 8) | (minor))

/********************************************************************************************
* Typedefs & Enumerations                                                                   *
*********************************************************************************************/

/**
 * @brief Driver Version
 */
typedef struct _ARM_DRIVER_VERSION {
    uint16_t api;                                      /* API version */
    uint16_t drv;                                      /* Driver verion */
} ARM_DRIVER_VERSION;

/**
 *@brief General Power State
 */
typedef enum _ARM_POWER_STATE {
    ARM_POWER_OFF,                                     /* Power off: no operation possible */
    ARM_POWER_LOW,                                     /* Low Power mode: retain state, detect and signal wake-up events */
    ARM_POWER_FULL                                     /* Power on: full operation at maximum performance */
} ARM_POWER_STATE;

#endif /* _DRIVER_COMMON_H_ */
