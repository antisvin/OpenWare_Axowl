# Library path
LIBROOT=$(BUILDROOT)/Drivers/

# Build path
BUILD=$(BUILDROOT)/Build

# Code Paths
DRIVERS=$(LIBROOT)/STM32F7xx_HAL_Driver
CMSIS_DEVICE=$(LIBROOT)/CMSIS/Device/ST/STM32F7xx
CMSIS_CORE=$(LIBROOT)/CMSIS/Include
DSPLIB=$(LIBROOT)/CMSIS/DSP_Lib/Source
USB_DEVICE_FILE=$(BUILDROOT)/Middlewares/ST/STM32_USB_Device_Library
USB_HOST_FILE=$(BUILDROOT)/Middlewares/ST/STM32_USB_Host_Library
USB_OTG_FILE=$(LIBROOT)/STM32_USB_OTG_Driver
FREERTOS_DIR=$(BUILDROOT)/Middlewares/Third_Party/FreeRTOS/Source

INC_FLAGS = -I$(CMSIS_CORE) -I$(CMSIS_DEVICE)/Include -I$(DRIVERS)/Inc 
INC_FLAGS += -I$(BUILDROOT)/Source -I$(BUILDROOT)/Inc
INC_FLAGS += -I$(USB_HOST_FILE)/Core/Inc
INC_FLAGS += -I$(USB_DEVICE_FILE)/Core/Inc
INC_FLAGS += -I$(USB_HOST_FILE)/Class/CDC/Inc -I$(USB_HOST_FILE)/Class/AUDIO/Inc
INC_FLAGS += -I$(USB_DEVICE_FILE)/Class/CDC/Inc -I$(USB_DEVICE_FILE)/Class/AUDIO/Inc
INC_FLAGS += -I$(USB_OTG_FILE)/inc
INC_FLAGS += -I$(FREERTOS_DIR)/include
INC_FLAGS += -I$(FREERTOS_DIR)/portable/GCC/ARM_CM7/r0p1
INC_FLAGS += -I$(FREERTOS_DIR)/CMSIS_RTOS

CPPFLAGS += -mtune=cortex-m7
CFLAGS += $(ARCH_FLAGS) $(INC_FLAGS) $(DEF_FLAGS)
CXXFLAGS += $(ARCH_FLAGS) $(INC_FLAGS) $(DEF_FLAGS)
LDFLAGS += -T$(LDSCRIPT) $(ARCH_FLAGS)

include $(OPENWARE)/common.mk
