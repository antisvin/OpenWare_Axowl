#include "main.h"
#include "stm32f4xx_hal.h"

#define OWL_AXOLOTI
#define HARDWARE_ID                  AXOLOTI_HARDWARE
#define HARDWARE_VERSION             "Axoloti"

/* USB audio settings */
#define AUDIO_BITS_PER_SAMPLE       16
#define AUDIO_BYTES_PER_SAMPLE      (AUDIO_BITS_PER_SAMPLE/8)
#define AUDIO_CHANNELS              2
#define AUDIO_INT32_TO_SAMPLE(x)    ((x)>>8)
#define AUDIO_SAMPLE_TO_INT32(x)    ((int32_t)(x)<<8)

#define USE_USBD_AUDIO
#define USE_USBD_AUDIO_TX  // microphone
/* #define USE_USBD_AUDIO_RX // speaker */
#define USE_USBD_FS
#define USBD_HANDLE hUsbDeviceFS
#define USBH_HANDLE hUsbHostHS
#define USE_USB_HOST
#define USB_HOST_RX_BUFF_SIZE 256  /* Max Received data 64 bytes */
#define USE_USBH_MIDI

#define USE_BKPSRAM

#define ADC_A 5
#define ADC_B 6
#define ADC_C 7
#define ADC_D 2
#define ADC_E 3
#define ADC_F 4
#define ADC_G 1
#define ADC_H 0
#define AUDIO_OUTPUT_GAIN            103

#define USE_ADC
#define ADC_PERIPH hadc1
//#define USE_CODEC

#define NOF_ADC_VALUES               8
#define NOF_PARAMETERS               40
#define NOF_BUTTONS                  (4+4)
