#include "ApplicationSettings.h"
#include "Codec.h"
#include "OpenWareMidiControl.h"
#include "Owl.h"
#include "PatchRegistry.h"
#include "Pin.h"
#include "ProgramManager.h"
#include "TakeoverControls.h"
#include "device.h"
#include "errorhandlers.h"
#include "message.h"
#include "qint.h"
#include "usb_device.h"
#include "usb_host.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef abs
#define abs(x) ((x) > 0 ? (x) : -(x))
#endif

// 12x12 bit multiplication with unsigned operands and result
#define U12_MUL_U12(a, b) (__USAT(((uint32_t)(a) * (b)) >> 12, 12))

#define CV_ATTENUATION_DEFAULT 2186 // calibrated to provide 1V/oct over 5V

// LEDs
// 1, 2, 3: LED buttons
// 4: Joystick press

// Buttons
// SW1, SW2, SW3, SW4 : exti
// SW4, SW5/mode: poll

Pin led1(LED1_GPIO_Port, LED1_Pin);
Pin led2(LED2_GPIO_Port, LED2_Pin);
Pin led3(LED3_GPIO_Port, LED3_Pin);
Pin led4(LED4_GPIO_Port, LED4_Pin);
Pin led_green(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
Pin led_red(LED_RED_GPIO_Port, LED_RED_Pin);

Pin sw1(SW1_GPIO_Port, SW1_Pin);
Pin sw2(SW2_GPIO_Port, SW2_Pin);
Pin sw3(SW3_GPIO_Port, SW3_Pin);
Pin sw_joy(SWJOY_GPIO_Port, SWJOY_Pin);
Pin s1(S1_GPIO_Port, S1_Pin);
Pin s2(S2_GPIO_Port, S2_Pin);

TakeoverControls<8, int16_t> takeover;
bool button_led_values[4] = {false};
volatile uint8_t patchselect;

extern int16_t parameter_values[NOF_PARAMETERS];

int16_t getParameterValue(uint8_t pid) {
  if (pid < 8)
    return takeover.get(pid);
  else if (pid < NOF_PARAMETERS)
    return parameter_values[pid];
  return 0;
}

// called from program, MIDI, or (potentially) digital bus
void setParameterValue(uint8_t pid, int16_t value) {
  if (pid < 8) {
    takeover.set(pid, value);
    takeover.reset(pid, false);
  } else if (pid < NOF_PARAMETERS) {
    parameter_values[pid] = value;
  }
}

bool updatePin(size_t bid, Pin pin) {
  // button id 'bid' goes from 1 to 4
  bool state = pin.get();
  static bool both_pressed = false;
  if (owl.getOperationMode() == RUN_MODE) {
    if (bid <= 4) {
      setButtonValue(bid + 3, state);
      setLed(bid, state ? RED_COLOUR : NO_COLOUR);
    } else {
      both_pressed = false;
      owl.setOperationMode(CONFIGURE_MODE);
    }
  } else if (owl.getOperationMode() == CONFIGURE_MODE) {
    if (state) {
      if (s1.get() && s2.get()) {
        both_pressed = true;
        led_green.set(true);
      }
      if (bid == 6)
        patchselect++;
      else
        patchselect--;
      patchselect &= 0b1111;
    } else if (!both_pressed) {
      // Next/prev patch selection with a single click
      if (bid == 6)
        patchselect++;
      else
        patchselect--;
      patchselect &= 0b1111;
    }
  }
  return state;
}

void onChangePin(uint16_t pin) {
  switch (pin) {
  case SW1_Pin:
    updatePin(1, sw1);
    break;
  case SW2_Pin:
    updatePin(2, sw2);
    break;
  case SW3_Pin:
    updatePin(3, sw3);
    break;
  case SWJOY_Pin:
    setButtonValue(PUSHBUTTON, updatePin(4, sw_joy));
    break;
  case S1_Pin:
    updatePin(5, s1);
    break;
  case S2_Pin:
    updatePin(6, s2);
    break;
  }
}

void setGateValue(uint8_t ch, int16_t value) {
  if (owl.getOperationMode() == RUN_MODE) {
    switch (ch) {
    case BUTTON_A:
      setLed(1, value ? RED_COLOUR : NO_COLOUR);
      break;
    case BUTTON_B:
      setLed(2, value ? RED_COLOUR : NO_COLOUR);
      break;
    case BUTTON_C:
      setLed(3, value ? RED_COLOUR : NO_COLOUR);
      break;
    case PUSHBUTTON:
    case BUTTON_D:
      setLed(4, value ? RED_COLOUR : NO_COLOUR);
      break;
    }
  }
}

void setLed(uint8_t led, uint32_t rgb) {
  switch (led) {
  case 0:
    break;
  case 1:
    led1.set(rgb == RED_COLOUR);
    break;
  case 2:
    led2.set(rgb == RED_COLOUR);
    break;
  case 3:
    led3.set(rgb == RED_COLOUR);
    break;
  case 4:
    led4.set(rgb == RED_COLOUR);
    break;
  case 5:
    led_green.set(rgb == RED_COLOUR);
    break;
  case 6:
    led_red.set(rgb == RED_COLOUR);
    break;
  }
}

static uint16_t smooth_adc_values[NOF_ADC_VALUES];
extern "C" {
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  // this runs at apprx 7.5kHz
  // with 144 cycles sample time and PCLK2 = 84MHz, div 8
  // giving a filter settling time of less than 3ms
  extern uint16_t adc_values[NOF_ADC_VALUES];
  for (size_t i = 0; i < NOF_ADC_VALUES; ++i) {
    // IIR exponential filter with lambda 0.75: y[n] = 0.75*y[n-1] + 0.25*x[n]
    smooth_adc_values[i] = (smooth_adc_values[i] * 3 + adc_values[i]) >> 2;
  }
}
}

void updateParameters(int16_t *parameter_values, size_t parameter_len,
                      uint16_t *adc_values, size_t adc_len) {

  for (size_t i = 0; i < 8; ++i) {
    takeover.update(i, smooth_adc_values[i], 31);
    parameter_values[i] = __USAT(takeover.get(i), 12);
  }
}

#define PATCH_RESET_COUNTER (600 / MAIN_LOOP_SLEEP_MS)
uint16_t progress = 0;
void setProgress(uint16_t value, const char *msg) {
  progress = value == 4095 ? 0 : value * 6;
}

bool isModeButtonPressed() { return s1.get() || s2.get(); }

void showPatchNumber() {
  // Use leds to display binary value, LED order is 4, 1, 2, 3
  setLed(1, 0 + RED_COLOUR * ((patchselect & 0b0100) >> 2));
  setLed(2, 0 + RED_COLOUR * ((patchselect & 0b0010) >> 1));
  setLed(3, 0 + RED_COLOUR * (patchselect & 0b0001));
  setLed(4, 0 + RED_COLOUR * ((patchselect & 0b1000) >> 3));
}

static uint32_t counter = 0;
static void updatePreset() {
  switch (owl.getOperationMode()) {
  case STARTUP_MODE:
  case STREAM_MODE:
  case LOAD_MODE: {
    setLed(4, (progress > 0) ? RED_COLOUR : NO_COLOUR);
    setLed(1, (progress > 4095 / 4) ? RED_COLOUR : NO_COLOUR);
    setLed(2, (progress > 4095 / 2) ? RED_COLOUR : NO_COLOUR);
    setLed(3, (progress > 4095 * 3 / 4) ? RED_COLOUR : NO_COLOUR);
    if (getErrorStatus() != NO_ERROR && !isModeButtonPressed())
      owl.setOperationMode(ERROR_MODE);
    break;
  }
  case RUN_MODE:
    if (isModeButtonPressed()) {
      owl.setOperationMode(CONFIGURE_MODE);
    } else if (getErrorStatus() != NO_ERROR) {
      owl.setOperationMode(ERROR_MODE);
    }
    break;
  case CONFIGURE_MODE:
    if (isModeButtonPressed()) {
      showPatchNumber();
      // TODO: volume control?
    } else {
      if (program.getProgramIndex() != patchselect &&
          registry.hasPatch(patchselect)) {
        // change patch on mode button release
        program.loadProgram(
            patchselect); // enters load mode (calls onChangeMode)
        program.resetProgram(false);
      } else {
        owl.setOperationMode(RUN_MODE);
      }
      takeover.reset(false);
    }
    break;
  case ERROR_MODE:
    setLed(1, counter > PATCH_RESET_COUNTER * 0.5 ? 4095 : 0);
    setLed(3, counter > PATCH_RESET_COUNTER * 0.5 ? 4095 : 0);
    setLed(2, counter < PATCH_RESET_COUNTER * 0.5 ? 4095 : 0);
    setLed(4, counter < PATCH_RESET_COUNTER * 0.5 ? 4095 : 0);
    if (isModeButtonPressed()) {
      setErrorStatus(NO_ERROR);
      owl.setOperationMode(CONFIGURE_MODE);
    }
    break;
  }
  if (++counter >= PATCH_RESET_COUNTER)
    counter = 0;
}

void onStartProgram() {
  // new patch selected or loaded
  takeover.set(0, getAnalogValue(ADC_A));
  takeover.set(1, getAnalogValue(ADC_B));
  takeover.set(2, getAnalogValue(ADC_C));
  takeover.set(3, getAnalogValue(ADC_D));
  takeover.set(4, getAnalogValue(ADC_E));
  takeover.set(5, getAnalogValue(ADC_F));
  takeover.set(6, getAnalogValue(ADC_G));
  takeover.set(7, getAnalogValue(ADC_H));
  takeover.reset(0, true);
  takeover.reset(1, true);
  takeover.reset(2, true);
  takeover.reset(3, true);
  takeover.reset(4, true);
  takeover.reset(5, true);
  takeover.reset(6, true);
  takeover.reset(7, true);
  memset(button_led_values, 0, sizeof(button_led_values)); // reset leds
}

void onChangeMode(uint8_t new_mode, uint8_t old_mode) {  
  if (new_mode == CONFIGURE_MODE) {
    // entering config mode
    setLed(5, GREEN_COLOUR);
    setLed(6, RED_COLOUR);
    takeover.reset(false);
    patchselect = program.getProgramIndex();
    // store current LED settings
    button_led_values[0] = led1.get();
    button_led_values[1] = led2.get();
    button_led_values[2] = led3.get();
    button_led_values[3] = led4.get();
  } else if (new_mode == RUN_MODE) {
    // we are either returning to the same patch or starting a new one
    // ledpwm.high(); // switch button leds to red
    for (int i = 1; i <= 4; ++i)
      setLed(i, button_led_values[i - 1] ? RED_COLOUR : NO_COLOUR);
    for (int i = 5; i <= 6; ++i)
      setLed(i, NO_COLOUR);
  }
  counter = 0;
}

void initLed() {
  for (uint8_t i = 1; i <= 6; i++)
    setLed(i, NO_COLOUR);
}

void onSetup() {
  initLed();
//  takeover.setOffset(6, getAnalogValue(ADC_G) - 4095 / 2);
//  takeover.setOffset(7, getAnalogValue(ADC_H) - 4095 / 2);
  led_green.set(false);
  led_red.set(false);
  for (size_t i = 5; i < 8; ++i) {
    takeover.set(i, CV_ATTENUATION_DEFAULT);
    takeover.reset(i, false);
  }
  // takeover.set(9, settings.audio_output_gain << 5);
  // takeover.reset(9, false);
  patchselect = program.getProgramIndex();

  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* init code for USB_HOST */
  MX_USB_HOST_Init();
}

void onLoop() {
  MX_USB_HOST_Process(); // todo: enable PWR management
  updatePreset();
}
