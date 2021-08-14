#include "main.h"
#include "stm32f4xx_hal.h"
#include "hardware_ids.h"

#define USE_BOOTLOADER_MODE
#define USE_USBD_MIDI
#define USBD_MAX_POWER              100 // 100mA for iPad compatibility
#ifdef USE_USBD_FS
#define USBD_HANDLE hUsbDeviceFS
#else
#define USBD_HANDLE hUsbDeviceHS
#endif

#if defined OWL_AXOLOTI
  #define HARDWARE_VERSION    "OWL Axoloti Boot"
  #define HARDWARE_ID         AXOLOTI_HARDWARE
  #define APPLICATION_ADDRESS 0x08010000
  #define BOOT1_GPIO_Port GPIOB
  #define BOOT1_Pin  GPIO_PIN_8
  #define USE_LED
  #define LED1_Pin GPIO_PIN_6
  #define LED1_GPIO_Port GPIOG
  #define LED2_Pin GPIO_PIN_6
  #define LED2_GPIO_Port GPIOC
#else
  #error Invalid configuration
#endif

/* #define USE_IWDG */
/* #define INIT_FMC */
