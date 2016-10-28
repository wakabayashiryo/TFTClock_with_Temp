#
#  File-Name: Makefile
#
#  Created on: 2016/9/15
#  Author: Wakabayashi Ryo
#
#	File-History:
# 	>date Detail
#	>2016/9/14 generate this file 
#   >2016/9/16 Complete to compile device drier 
#	reference to worning http://stackoverflow.com/questions/13060106/getting-rid-of-wchar-t-size-linker-warning
#must delete template file in hal_driver

#define STM32F405xx */   /*!< STM32F405RG, STM32F405VG and STM32F405ZG Devices */
#define STM32F415xx */   /*!< STM32F415RG, STM32F415VG and STM32F415ZG Devices */
#define STM32F407xx */   /*!< STM32F407VG, STM32F407VE, STM32F407ZG, STM32F407ZE, STM32F407IG  and STM32F407IE Devices */
#define STM32F417xx */   /*!< STM32F417VG, STM32F417VE, STM32F417ZG, STM32F417ZE, STM32F417IG and STM32F417IE Devices */
#define STM32F427xx */   /*!< STM32F427VG, STM32F427VI, STM32F427ZG, STM32F427ZI, STM32F427IG and STM32F427II Devices */
#define STM32F437xx */   /*!< STM32F437VG, STM32F437VI, STM32F437ZG, STM32F437ZI, STM32F437IG and STM32F437II Devices */
#define STM32F429xx */   /*!< STM32F429VG, STM32F429VI, STM32F429ZG, STM32F429ZI, STM32F429BG, STM32F429BI, STM32F429NG, 
                                   #STM32F439NI, STM32F429IG  and STM32F429II Devices */
#define STM32F439xx */   /*!< STM32F439VG, STM32F439VI, STM32F439ZG, STM32F439ZI, STM32F439BG, STM32F439BI, STM32F439NG, 
                                   #STM32F439NI, STM32F439IG and STM32F439II Devices */
#define STM32F401xC */   /*!< STM32F401CB, STM32F401CC, STM32F401RB, STM32F401RC, STM32F401VB and STM32F401VC Devices */
#define STM32F401xE */   /*!< STM32F401CD, STM32F401RD, STM32F401VD, STM32F401CE, STM32F401RE and STM32F401VE Devices */
#define STM32F410Tx */   /*!< STM32F410T8 and STM32F410TB Devices */
#define STM32F410Cx */   /*!< STM32F410C8 and STM32F410CB Devices */
#define STM32F410Rx */   /*!< STM32F410R8 and STM32F410RB Devices */
#define STM32F411xE */   /*!< STM32F411CC, STM32F411RC, STM32F411VC, STM32F411CE, STM32F411RE and STM32F411VE Devices */
#define STM32F446xx */   /*!< STM32F446MC, STM32F446ME, STM32F446RC, STM32F446RE, STM32F446VC, STM32F446VE, STM32F446ZC, 
                                   #and STM32F446ZE Devices */
#define STM32F469xx */   /*!< STM32F469AI, STM32F469II, STM32F469BI, STM32F469NI, STM32F469AG, STM32F469IG, STM32F469BG, 
                                   #STM32F469NG, STM32F469AE, STM32F469IE, STM32F469BE and STM32F469NE Devices */
#define STM32F479xx */   /*!< STM32F479AI, STM32F479II, STM32F479BI, STM32F479NI, STM32F479AG, STM32F479IG, STM32F479BG 
                                   #and STM32F479NG Devices */
#define STM32F412Cx */   /*!< STM32F412CEU and STM32F412CGU Devices */
#define STM32F412Zx */   /*!< STM32F412ZET, STM32F412ZGT, STM32F412ZEJ and STM32F412ZGJ Devices */
#define STM32F412Vx */   /*!< STM32F412VET, STM32F412VGT, STM32F412VEH and STM32F412VGH Devices */
#define STM32F412Rx */   /*!< STM32F412RET, STM32F412RGT, STM32F412REY and STM32F412RGY Devices */

#project name
PROJECT := project1
DEBUG := 1
HARDFP := 0
DEVICENAME := STM32F407xx
STARTUP_FILE := startup_stm32f407xx.s
LINKERSCRIPT_FILE := STM32F407VGTx_FLASH.ld

# command of compile and remove 
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm

RM = rm -f -r 

#project directories
DRV_DIR := ./Drivers
MIDLEWARE_DIR := ./MidleWare
CMSIS_DIR := $(DRV_DIR)/CMSIS
CMSISDEV_DIR := $(CMSIS_DIR)/Device
HAL_DIR := $(DRV_DIR)/STM32F4xx_HAL_Driver
APP_DIR := ./Apprication

#source directory macros
SRCDIR := ./Src
MIDLEWARE_SRCDIR := $(shell find $(MIDLEWARE_DIR) -name Src)
CMSISDEV_SRCDIR := $(CMSISDEV_DIR)/Src
APP_SRCDIR := $(shell find ./Apprication -name Src)
HAL_SRCDIR := $(HAL_DIR)/Src

#include directory macros
INCDIR := ./Inc
MIDLEWARE_INCDIR := $(shell find $(MIDLEWARE_DIR) -name Inc)
CMSISDEV_INCDIR := $(CMSISDEV_DIR)/Inc
APP_INCDIR := $(shell find Apprication -name Inc)
HAL_INCDIR := $(HAL_DIR)/Inc
CMSIS_INCDIR := $(CMSIS_DIR)/Inc

#output drectries macros
OBJDIR := ./Debug
BINDIR := ./Bin

#device file directoriess
LINKERSCRIPT_DIR := $(DRV_DIR)/LinkerScript
STARTUP_DIR := $(CMSISDEV_SRCDIR)/gcc

ifeq ($(HARDFP),1)
	FLOATABI := hard
else
	FLOATABI := softfp
endif
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOATABI) 
LINKERSCRIPT := $(LINKERSCRIPT_DIR)/$(LINKERSCRIPT_FILE)

#  compile options
WORNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Winit-self -Wcast-qual
CFLAGS := -c -fmessage-length=0 $(WORNINGS) -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections \
-funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(CPU) -Os -std=gnu99 -MMD -MP -D $(DEVICENAME)  
LDFLAGS := $(CPU) -Wl,--no-wchar-size-warning -Wl,--gc-sections --specs=nano.specs -Wl,-Map=$(OBJDIR)/$(PROJECT).map,-cref
ASMFLAGS := -c -x assembler-with-cpp -c $(WORNINGS) -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections \
-fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(CPU) -Os 
LIBFLAGS = -lm -lc -lgcc -lnosys
# Cpp option? -lstdc++ -lsupc++

ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -O0 -g3
else
  CFLAGS += -DNDEBUG -Os -g0
endif

SRCS := $(wildcard $(SRCDIR)/*.c)\
 $(wildcard $(CMSISDEV_SRCDIR)/*.c)\
 $(wildcard $(HAL_SRCDIR)/*.c)\
 $(foreach dir,$(APP_SRCDIR),$(wildcard $(dir)/*.c))\
 $(foreach dir,$(MIDLEWARE_SRCDIR),$(wildcard $(dir)/*.c))

ASMS := $(STARTUP_DIR)/$(STARTUP_FILE)
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.c=%.o) $(ASMS:%.s=%.o)))

# directory of headerfiles macros
INCPATH := -I $(INCDIR) \
		-I $(HAL_INCDIR)\
		-I $(CMSIS_INCDIR) \
		-I $(CMSISDEV_INCDIR)\
		$(addprefix -I ,$(APP_INCDIR))\
		$(addprefix -I ,$(MIDLEWARE_INCDIR))

vpath %.c $(SRCDIR) $(CMSISDEV_SRCDIR) $(APP_SRCDIR) $(HAL_SRCDIR) $(MIDLEWARE_SRCDIR)
vpath %.s $(STARTUP_DIR)

.PHONY: all size lst 

all: checkfile $(BINDIR)/$(PROJECT).elf lst hex bin size

#generate binary-file
$(BINDIR)/$(PROJECT).elf: $(OBJS)
	-@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -T$(LINKERSCRIPT) -o $@ $^ -Wl,--start-group $(LIBFLAGS) -Wl,--end-group

$(OBJDIR)/%.o: %.c 
	-@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCPATH) -o $@ $< $(LIBFLAGS)

$(OBJDIR)/%.o: %.s
	-@mkdir -p $(OBJDIR)
	$(CC) $(ASMFLAGS) $(INCPATH) -o $@ $< $(LIBFLAGS)

hex: $(BINDIR)/$(PROJECT).hex
bin: $(BINDIR)/$(PROJECT).bin	
lst: $(OBJDIR)/$(PROJECT).lst

$(BINDIR)/$(PROJECT).bin: $(BINDIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(BINDIR)/$(PROJECT).hex: $(BINDIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

size: $(BINDIR)/$(PROJECT).elf
	$(SIZE) -A $<

$(OBJDIR)/$(PROJECT).lst: $(BINDIR)/$(PROJECT).elf
	$(OBJDUMP) -Sdh $< > $@

clean: 
	$(RM) $(OBJDIR) $(BINDIR)

checkfile:
#check file name 
ifeq ($(shell find . -name $(LINKERSCRIPT_FILE)),$(LINKERSCRIPT_DIR)/$(LINKERSCRIPT_FILE))
	@echo 'found LinkerScript_file'	 
else 
	$(error not found LinkerScript_file)
endif
ifeq ($(shell find . -name $(STARTUP_FILE)),$(STARTUP_DIR)/$(STARTUP_FILE))
	@echo 'found StartUp_file'
else 
	$(error not found StartUp_file)
endif

#dependency file
DEPS := $(OBJS:%.o=%.d)
-include $(DEPS)
