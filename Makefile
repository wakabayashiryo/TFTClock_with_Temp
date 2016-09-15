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

#command of compile and compile options , remove 
CC = arm-none-eabi-gcc
COPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

RM = rm -f -R 

OPTIMIZE := -O0
CFLAGS := -c -g $(OPTIMIZE) -std=c99 -MMD -MP -Wall -Wextra -Winit-self


#target name
TARGET := project
			
#directory macros
SRCDIR := Src
INCDIR := Inc
CMSISDIR := Driver/CMSIS 
HALDIR := Driver/STM32F4xx_HAL_Driver
LIBSRC := $(shell find $(LIBDIR) -name Src)s
OBJ := Debug
BIN := Bin


SRCF := $(wildcard $(SRCDIR)/*.c)
LIBSRCF := $(foreach dir,$(LIBSRC),$(wildcard $(dir)/*.c))
OBJF := $(addprefix $(OBJ)/,$(notdir $(SRCF:%.c=%.o)))\
	 $(addprefix $(OBJ)/,$(notdir $(LIBSRCF:%.c=%.o)))
#directory of headerfiles macros
INCLUDE :=  -I Inc \
		$(addprefix -I ,$(shell find lib -name Inc))
		
#dependency file
DEPF := $(OBJF:%.o=%.d)

VPATH := $(LIBSRC)

.PHONY: all clean

all: $(BIN)/$(TARGET)

#generate binary-file
$(BIN)/$(TARGET): $(OBJF)
	-@mkdir -p $(BIN)
	$(CC) -o $@ $^

#sources are generated to objectfile in Maindirectry 
$(OBJ)/%.o: $(SRCDIR)/%.c
	-@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

#sources are generated to objectfile in Subdirectry 
$(OBJ)/%.o: %.c
	-@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<
	
-include $(DEPF) #-includeは.PHONYとallの間に入れないように

#delete object binary directries
clean: 
	$(RM) $(OBJ) $(BIN) 