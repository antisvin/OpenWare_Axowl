#include "adau1961.h"
#include "device.h"
#include "errorhandlers.h"

#ifndef CODEC_TIMEOUT
#define CODEC_TIMEOUT 1000000
#endif

static uint8_t i2ctxbuf[8];
static uint8_t i2crxbuf[6];
extern I2C_HandleTypeDef CODEC_I2C_HANDLE;

void codec_write(uint16_t RegisterAddr, uint8_t RegisterValue) {
  // interrupt version
  i2ctxbuf[0] = RegisterAddr >> 8;
  i2ctxbuf[1] = RegisterAddr;
  i2ctxbuf[2] = RegisterValue;
  HAL_Delay(10);
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(
      &CODEC_I2C_HANDLE, CODEC_ADDRESS << 1, i2ctxbuf, 3, CODEC_TIMEOUT);
  // should suspend thread and resume when transmit is done...
  if (r != HAL_OK)
    error(CONFIG_ERROR, "I2C transmit failed");
  // but for now we just sleep a little and assume transmission is finished
  // after...
  HAL_Delay(10);
}

void codec_write_block(uint16_t RegisterAddr, const uint8_t *RegisterValues) {
  i2ctxbuf[0] = RegisterAddr >> 8;
  i2ctxbuf[1] = RegisterAddr;
  i2ctxbuf[2] = RegisterValues[0];
  i2ctxbuf[3] = RegisterValues[1];
  i2ctxbuf[4] = RegisterValues[2];
  i2ctxbuf[5] = RegisterValues[3];
  i2ctxbuf[6] = RegisterValues[4];
  i2ctxbuf[7] = RegisterValues[5];
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(
      &CODEC_I2C_HANDLE, CODEC_ADDRESS << 1, i2ctxbuf, 8, CODEC_TIMEOUT);
  if (r != HAL_OK)
    error(CONFIG_ERROR, "I2C transmit failed");
  HAL_Delay(10);
}

void codec_read(uint16_t RegisterAddr) {
  i2ctxbuf[0] = RegisterAddr >> 8;
  i2ctxbuf[1] = RegisterAddr;
  HAL_Delay(1);
  HAL_I2C_Master_Transmit(&CODEC_I2C_HANDLE, CODEC_ADDRESS << 1, i2ctxbuf, 2,
                          CODEC_TIMEOUT);
  HAL_Delay(1);
  HAL_StatusTypeDef r = HAL_I2C_Master_Receive(
      &CODEC_I2C_HANDLE, (CODEC_ADDRESS << 1) + 1, i2crxbuf, 6, CODEC_TIMEOUT);
  if (r != HAL_OK)
    error(CONFIG_ERROR, "I2C receive failed");
}

void picosleep(void) {
  volatile int i = 100;
  while (i--) {
  }
}

void codec_init() {
  // ADAU_I2C_Init();
  //HAL_Delay(5);

  /*
   * 1. Power down the PLL.
   * 2. Reset the PLL control register.
   * 3. Start the PLL.
   * 4. Poll the lock bit.
   * 5. Assert the core clock enable bit after the PLL lock is acquired.
   */

  codec_write(ADAU1961_REG_R0_CLKC, 0x00); // disable core

  unsigned char pllreg[6];
  // reg setting 0x007D 0012 3101
  pllreg[0] = 0x1F;
  pllreg[1] = 0x40;
  pllreg[2] = 0x04;
  pllreg[3] = 0x81;
  pllreg[4] = 0x31;
  pllreg[5] = 0x01;

  codec_write_block(ADAU1961_REG_R1_PLLC, &pllreg[0]);

  int i = 100;
  while (i) {
    // wait for PLL
    codec_read(ADAU1961_REG_R1_PLLC);
    if (i2crxbuf[5] & 0x02)
      break;
    HAL_Delay(1);
    i--;
  }
  if (!i) {
    error(CONFIG_ERROR, "Error initializing codec");
    return;
  }

  codec_write(ADAU1961_REG_R0_CLKC, 0x09); // PLL = clksrc

  // i2s2_sd (dac) is a confirmed connection, i2s2_ext_sd (adc) is not however
  // bclk and lrclk are ok too
  codec_write(ADAU1961_REG_R2_DMICJ, 0x00);
  codec_write(ADAU1961_REG_R3_RES, 0x00);
  codec_write(ADAU1961_REG_R4_RMIXL0, 0x00);
  codec_write(ADAU1961_REG_R5_RMIXL1, 0x00);
  codec_write(ADAU1961_REG_R6_RMIXR0, 0x00);
  codec_write(ADAU1961_REG_R7_RMIXR1, 0x00);
  codec_write(ADAU1961_REG_R8_LDIVOL, 0x00);
  codec_write(ADAU1961_REG_R9_RDIVOL, 0x00);
  codec_write(ADAU1961_REG_R10_MICBIAS, 0x00);
  codec_write(ADAU1961_REG_R11_ALC0, 0x00);
  codec_write(ADAU1961_REG_R12_ALC1, 0x00);
  codec_write(ADAU1961_REG_R13_ALC2, 0x00);
  codec_write(ADAU1961_REG_R14_ALC3, 0x00);

  codec_write(ADAU1961_REG_R15_SERP0, 0x01); // codec is master

  codec_write_block(ADAU1961_REG_R1_PLLC, &pllreg[0]);
  // dmaStreamClearInterrupt(sai_b_dma);
  // dmaStreamEnable(sai_b_dma);

  // dmaStreamClearInterrupt(sai_a_dma);
  // dmaStreamEnable(sai_a_dma);
  // SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;
  // SAI1_Block_B->CR1 |= SAI_xCR1_SAIEN;

  codec_write(ADAU1961_REG_R16_SERP1, 0x00); // 32bit samples
  //  codec_write(ADAU1961_REG_R16_SERP1,0x60); // 64bit samples,
  //  spdif clock!

  codec_write(ADAU1961_REG_R17_CON0, 0x00);
  codec_write(ADAU1961_REG_R18_CON1, 0x00);
  codec_write(ADAU1961_REG_R19_ADCC, 0x10);
  codec_write(ADAU1961_REG_R20_LDVOL, 0x00);
  codec_write(ADAU1961_REG_R21_RDVOL, 0x00);
  codec_write(ADAU1961_REG_R22_PMIXL0, 0x00);
  codec_write(ADAU1961_REG_R23_PMIXL1, 0x00);
  codec_write(ADAU1961_REG_R24_PMIXR0, 0x00);
  codec_write(ADAU1961_REG_R25_PMIXR1, 0x00);
  codec_write(ADAU1961_REG_R26_PLRML, 0x00);
  codec_write(ADAU1961_REG_R27_PLRMR, 0x00);
  codec_write(ADAU1961_REG_R28_PLRMM, 0x00);
  codec_write(ADAU1961_REG_R29_PHPLVOL, 0x02);
  codec_write(ADAU1961_REG_R30_PHPRVOL, 0x02);
  codec_write(ADAU1961_REG_R31_PLLVOL, 0x02);
  codec_write(ADAU1961_REG_R32_PLRVOL, 0x02);
  codec_write(ADAU1961_REG_R33_PMONO, 0x02);
  codec_write(ADAU1961_REG_R34_POPCLICK, 0x00);
  codec_write(ADAU1961_REG_R35_PWRMGMT, 0x00);
  codec_write(ADAU1961_REG_R36_DACC0, 0x00);
  codec_write(ADAU1961_REG_R37_DACC1, 0x00);
  codec_write(ADAU1961_REG_R38_DACC2, 0x00);
  codec_write(ADAU1961_REG_R39_SERPP, 0x00);
  codec_write(ADAU1961_REG_R40_CPORTP0, 0xAA);
  codec_write(ADAU1961_REG_R41_CPORTP1, 0xAA);
  codec_write(ADAU1961_REG_R42_JACKDETP, 0x00);

  HAL_Delay(10);

  codec_write(ADAU1961_REG_R19_ADCC, 0x03);  // ADC enable
  codec_write(ADAU1961_REG_R36_DACC0, 0x03); // DAC enable

  codec_write(ADAU1961_REG_R31_PLLVOL,
              0xE7); // Playback Line Output Left Volume
  codec_write(ADAU1961_REG_R32_PLRVOL,
              0xE7); // Playback Right Output Left Volume

  codec_write(ADAU1961_REG_R26_PLRML,
              0x05); // unmute Mixer5, 6dB gain
  codec_write(ADAU1961_REG_R27_PLRMR,
              0x11); // unmute Mixer6, 6dB gain
  codec_write(ADAU1961_REG_R22_PMIXL0,
              0x21); // unmute DAC, no aux mix
  codec_write(ADAU1961_REG_R24_PMIXR0,
              0x41); // unmute DAC, no aux mix

  codec_write(ADAU1961_REG_R35_PWRMGMT, 0x03); // enable L&R

  codec_write(ADAU1961_REG_R4_RMIXL0,
              0x01); // mixer1 enable, mute LINP and LINR
  codec_write(ADAU1961_REG_R5_RMIXL1,
              0x08); // unmute PGA, aux mute, 0 dB boost
  codec_write(ADAU1961_REG_R6_RMIXR0,
              0x01); // mixer2 enable, mute LINP and LINR
  codec_write(ADAU1961_REG_R7_RMIXR1,
              0x08); // unmute PGA, aux mute, 0 dB boost

  codec_write(ADAU1961_REG_R8_LDIVOL, 0x43); // 0dB gain
  codec_write(ADAU1961_REG_R9_RDIVOL, 0x43); // 0dB gain

  // capless headphone config
  codec_write(ADAU1961_REG_R33_PMONO, 0x03); // MONOM+MOMODE
  codec_write(ADAU1961_REG_R28_PLRMM,
              0x01); // MX7EN, COMMON MODE OUT
  codec_write(ADAU1961_REG_R29_PHPLVOL, 0xC3);
  codec_write(ADAU1961_REG_R30_PHPRVOL, 0xC3);

  HAL_Delay(10);
}

void codec_bypass(int bypass) {};

void codec_set_gain_in(int8_t volume) {};

void codec_set_gain_out(int8_t volume) {};
