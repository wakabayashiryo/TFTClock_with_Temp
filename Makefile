#
#  File-Name: Makefile
#
#  Created in: 2016/9/15
#  Author: Wakabayashi Ryo
#
#	File-History:
# 	>date Detail
#	>2016/9/14 generated this file 
#   >2016/9/16 Complete to compile device driver
#   >2017/2/23 arrange 
#	reference to worning http://stackoverflow.com/questions/13060106/getting-rid-of-wchar-t-size-linker-warning
#	*Have to delete template file in hal_driver

#project configuration
PROJECT := F4xx_Project
OPTIMIZE := -O0 -g3
HARDFP := 0
DEVICENAME := STM32F407xx
STARTUP_FILE := startup_stm32f407xx.s
LINKERSCRIPT_FILE := STM32F407VGTx_FLASH.ld

# command of compile and remove
CC := arm-none-eabi-gcc
OBJDUMP := arm-none-eabi-objdump
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm

RM := rm -f -r 

########################################*DIRECTORIES GROUP*##############################################
#project directories
DRV_DIR := ./Drivers
MIDLEWARE_DIR := ./MidleWare
CMSIS_DIR := $(DRV_DIR)/CMSIS
CMSISDEV_DIR := $(CMSIS_DIR)/Device
HAL_DIR := $(DRV_DIR)/STM32F4xx_HAL_Driver
APP_DIR := ./Apprication

#source directories
SRCDIR := ./Src
MIDLEWARE_SRCDIR := $(shell find $(MIDLEWARE_DIR) -name Src)
CMSISDEV_SRCDIR := $(CMSISDEV_DIR)/Src
APP_SRCDIR := $(shell find ./Apprication -name Src)
HAL_SRCDIR := $(HAL_DIR)/Src

#include directories
INCDIR := ./Inc
MIDLEWARE_INCDIR := $(shell find $(MIDLEWARE_DIR) -name Inc)
CMSISDEV_INCDIR := $(CMSISDEV_DIR)/Inc
APP_INCDIR := $(shell find Apprication -name Inc)
HAL_INCDIR := $(HAL_DIR)/Inc
CMSIS_INCDIR := $(CMSIS_DIR)/Inc

#output directries
OBJDIR := ./Debug
BINDIR := ./Bin

#device file directories
LINKERSCRIPT_DIR := $(DRV_DIR)/LinkerScript
STARTUP_DIR := $(CMSISDEV_SRCDIR)/gcc
###############################################################################################

#####################*SOURCE AND HEADER , ASM , OBJECT FILES GROUP*############################
SRCS := $(wildcard $(SRCDIR)/*.c)\
 $(wildcard $(CMSISDEV_SRCDIR)/*.c)\
 $(wildcard $(HAL_SRCDIR)/*.c)\
 $(foreach dir,$(APP_SRCDIR),$(wildcard $(dir)/*.c))\
 $(foreach dir,$(MIDLEWARE_SRCDIR),$(wildcard $(dir)/*.c))

ASMS := $(STARTUP_DIR)/$(STARTUP_FILE)
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.c=%.o) $(ASMS:%.s=%.o)))

# directory of headerfiles
INCPATH := -I $(INCDIR) \
		-I $(HAL_INCDIR)\
		-I $(CMSIS_INCDIR) \
		-I $(CMSISDEV_INCDIR)\
		$(addprefix -I ,$(APP_INCDIR))\
		$(addprefix -I ,$(MIDLEWARE_INCDIR))
###############################################################################################

####################################*COMMAND OPTIONS GROUP*####################################
ifeq ($(HARDFP),1)
	FLOATABI := hard
else
	FLOATABI := softfp
endif

CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOATABI) 
LINKERSCRIPT := $(LINKERSCRIPT_DIR)/$(LINKERSCRIPT_FILE)

#  compile options
WORNINGS := -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Winit-self -Wcast-qual

CFLAGS := -std=gnu99 -c -fmessage-length=0 $(WORNINGS) -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections \
-funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(OPTIMIZE) $(CPU) -MMD -MP -D $(DEVICENAME)  

LDFLAGS := -Wl,--no-wchar-size-warning -Wl,--gc-sections --specs=nano.specs -Wl,-Map=$(OBJDIR)/$(PROJECT).map,-cref $(CPU) 

ASMFLAGS := -c -x assembler-with-cpp -c $(WORNINGS) -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections \
-fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(OPTIMIZE) $(CPU)

LIBFLAGS := -lm -lc -lgcc -lnosys
###############################################################################################

vpath %.c $(SRCDIR) $(CMSISDEV_SRCDIR) $(APP_SRCDIR) $(HAL_SRCDIR) $(MIDLEWARE_SRCDIR)
vpath %.s $(STARTUP_DIR)

.PHONY: all size lst 

all: checkfile $(BINDIR)/$(PROJECT).elf lst hex bin size

#generate binary-file
$(BINDIR)/$(PROJECT).elf: $(OBJS)
	-@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -T $(LINKERSCRIPT) -o $@ $^ -Wl,--start-group $(LIBFLAGS) -Wl,--end-group

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
	$(SIZE) $<

$(OBJDIR)/$(PROJECT).lst: $(BINDIR)/$(PROJECT).elf
	$(OBJDUMP) -Sdh $< > $@

clean: 
	$(RM) $(OBJDIR) $(BINDIR)

checkfile:
#check file name 
ifeq ($(shell find . -name $(LINKERSCRIPT_FILE)),$(LINKERSCRIPT_DIR)/$(LINKERSCRIPT_FILE))
	@echo 'Found LinkerScript_file'	 
else 
	$(error Not found LinkerScript_file)
endif
ifeq ($(shell find . -name $(STARTUP_FILE)),$(STARTUP_DIR)/$(STARTUP_FILE))
	@echo 'Found StartUp_file'
else 
	$(error Not found StartUp_file)
endif

	@if [ ! -d $(APP_DIR) ]; \
		then mkdir -p $(APP_DIR); \
	fi
	@if [ ! -d $(MIDLEWARE_DIR) ]; \
		then mkdir -p $(MIDLEWARE_DIR); \
	fi

#dependency file
DEPS := $(OBJS:%.o=%.d)
-include $(DEPS)
