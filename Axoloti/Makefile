PROJECT = Axoloti
BUILDROOT = .
OPENWARE ?= $(BUILDROOT)/..
LDSCRIPT = $(OPENWARE)/Hardware/axoloti.ld

include $(OPENWARE)/Hardware/owl2.mk

C_SRC = $(filter-out Src/test.c, $(wildcard Src/*.c))
CPP_SRC = $(wildcard Src/*.cpp)
C_SRC += $(OPENWARE)/Source/sdram.c
C_SRC += $(OPENWARE)/Source/adau1961.c
C_SRC += $(OPENWARE)/Source/bsp_driver_sd.c
C_SRC += $(OPENWARE)/Source/sd_diskio.c
C_SRC += $(OPENWARE)/Source/fatfs.c
C_SRC += $(FATFS_DIR)/diskio.c
C_SRC += $(FATFS_DIR)/ff.c
C_SRC += $(FATFS_DIR)/ff_gen_drv.c
C_SRC += $(FATFS_DIR)/option/ccsbcs.c
C_SRC += $(FATFS_DIR)/option/syscall.c
CPP_SRC += $(OPENWARE)/Source/usbh_midi.cpp
CPP_SRC += $(OPENWARE)/Source/usbd_audio.cpp
CPP_SRC += $(OPENWARE)/Source/uart.cpp
CPP_SRC += $(OPENWARE)/Source/uart_midi.cpp
CPP_SRC += $(OPENWARE)/Source/MidiStreamReader.cpp

include $(OPENWARE)/Hardware/sources.mk

C_SRC += $(C_SRC_SAI)
C_SRC += $(C_SRC_DAC)
C_SRC += $(C_SRC_SDRAM)
C_SRC += $(C_SRC_USBD)
C_SRC += $(C_SRC_USBH)
C_SRC += $(C_SRC_DSP)
C_SRC += $(C_SRC_OS)
C_SRC += $(C_SRC_UART)
C_SRC += $(C_SRC_RTC)
C_SRC += $(C_SRC_I2C)
C_SRC += $(C_SRC_SD)


include $(OPENWARE)/Hardware/f4.mk
