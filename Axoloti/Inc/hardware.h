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

// Serial MIDI
#define USE_UART_MIDI_RX
#define USE_UART_MIDI_TX
#define UART_MIDI_HANDLE huart6
#define UART_MIDI_RX_BUFFER_SIZE 256


#define USE_BKPSRAM

#define ADC_A 0
#define ADC_B 1
#define ADC_C 2
#define ADC_D 3
#define ADC_E 4
#define ADC_F 5
#define ADC_G 6
#define ADC_H 7
#define AUDIO_OUTPUT_GAIN            103

#define USE_ADC
#define ADC_PERIPH hadc1
#define USE_CODEC
#define USE_ADAU1961
#define CODEC_I2C_HANDLE hi2c3
#define CODEC_ADDRESS    0x38

#define NOF_ADC_VALUES               8
#define NOF_PARAMETERS               40
#define NOF_BUTTONS                  (4+4)
