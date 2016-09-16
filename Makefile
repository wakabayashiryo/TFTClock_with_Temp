#
#  File-Name: Makefile
#
#  Created on: 2016/9/15
#  Author: Wakabayashi Ryo
#
#	File-History:
# 	>date Detail
#	>2016/9/15 generate this file 
#

#target name
PROJECT := name1
DEBUG := 0
HARDFP := 0
DEVICENAME := STM32F407xx
STARTUP := startup_stm32f407xx.s
# command of compile and remove 
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm

RM = rm -f -r 

# directory macros
SRCDIR := Src
INCDIR := Inc
DRVDIR := Drivers
CMSISDIR := $(DRVDIR)/CMSIS
CMSISDEVDIR := $(CMSISDIR)/Device
HALDIR := $(DRVDIR)/STM32F4xx_HAL_Driver
APPDIR := $(shell find Apprication -name Src)
OBJDIR := Debug
BINDIR := Bin

# Device 
ifeq ($(HARDFP),1)
	FLOATABI := hard
else
	FLOATABI := softfp
endif
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOATABI) 
LINKERSCRIPT := $(shell find . -name $(addsuffix *,STM32F407VGT))

#  compile options
WORNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Winit-self -Wcast-qual
CFLAGS := -c -fmessage-length=0 $(WORNINGS) -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections \
-funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(CPU) -Os -std=gnu99 -MMD -MP -D $(DEVICENAME)  
LDFLAGS := -Wl,--gc-sections -Wl,--wrap,main $(CPU) 
ASMFLAGS := -x assembler-with-cpp -c $(WORNINGS) -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections \
-fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer $(CPU) -Os 
LIBFLAGS = -lm 
# Cpp option? -lstdc++ -lsupc++
# -lc -lgcc -lnosys
ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -O0 -g3
else
  CFLAGS += -DNDEBUG -Os -g0
endif

SRCS := $(wildcard $(SRCDIR)/*.c) $(CMSISDEVDIR)/Src/system_stm32f4xx.c
HALSRCS := $(wildcard $(HALDIR)/Src/*.c)
ASMS := $(CMSISDEVDIR)/Src/gcc/$(STARTUP)
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.c=%.o)) \
								$(notdir $(HALSRCS:%.c=%.o))\
								$(notdir $(ASMSRCS:%.s=%.o)))
# directory of headerfiles macros
INCPATH := -I Inc \
		-I $(CMSISDIR)/Inc \
		-I $(HALDIR)/Inc \
		$(addprefix -I ,$(shell find Apprication -name Inc)) \
		-I $(CMSISDEVDIR)/Inc/

#dependency file
DEPS := $(OBJS:%.o=%.d)

VPATH := $(SRCDIR) $(HALDIR) $(APPDIR) $(CMSISDEVDIR)/Src/

.PHONY: all print

all: $(BINDIR)/$(PROJECT).elf

#generate binary-file
$(BINDIR)/$(PROJECT).elf: $(OBJS)
	-@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

#sources are generated to objectfile in Subdirectry 
$(OBJDIR)/%.o: %.c $(ASMS)
	-@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCPATH) -o $@ $< $(LIBFLAGS)

-include $(DEPS) #-includeは.PHONYとallの間に入れないように

#delete object binary directries
clean: 
	$(RM) $(OBJDIR) $(BINDIR) 

print:
	@echo $(OBJS)
