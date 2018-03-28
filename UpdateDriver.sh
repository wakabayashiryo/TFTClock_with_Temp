#!/bin/bash

check_exist_f() 
{
    local FILE_NUM=$(find $1 -type f -name $2 | wc -l)
    if [ ${FILE_NUM} -gt 0 ] ; then
        echo "Find ${FILE_NUM} Files in $1" 
    else
        echo "Not Find Files in $1"
    fi
}

# Update Destination Directory pathes
MAIN_HAL_SRC="./Drivers/STM32F4xx_HAL_Driver/Src"
MAIN_HAL_INC="./Drivers/STM32F4xx_HAL_Driver/Inc"

MAIN_LINKERS="./Drivers/LinkerScript"

MAIN_CMSIS_CORE="./Drivers/CMSIS/Inc"

MAIN_CMSIS_DEV_INC="./Drivers/CMSIS/Device/Inc"
MAIN_CMSIS_DEV_SRC="./Drivers/CMSIS/Device/Src"
MAIN_CMSIS_DEV_GCC="./Drivers/CMSIS/Device/Src/gcc"

echo "***** Check Directories Place and File Existence at update Destination *****"

check_exist_f ${MAIN_HAL_SRC} stm32f*.c
check_exist_f ${MAIN_HAL_INC} stm32*.h
check_exist_f ${MAIN_LINKERS} STM32F4*.ld
check_exist_f ${MAIN_CMSIS_CORE} *.h
check_exist_f ${MAIN_CMSIS_DEV_INC} *.h
check_exist_f ${MAIN_CMSIS_DEV_SRC} system_stm32f4xx.c
check_exist_f ${MAIN_CMSIS_DEV_GCC} startup_stm32f4*.s
echo 

if [ ! -d $1 ];then 
    echo "Not Exist Specified Directory:$1"
    exit 1
fi

echo "***** Search Directories Place and Specified update File Existence *****"

# Search New Driver Directories
DEVICE_HOME=$(find $1 -type d -name STM32F4xx) 

NEW_CMSIS_DEV_INC_D=$(find ${DEVICE_HOME} -type d -name Include)
NEW_CMSIS_DEV_GCC_D=$(find ${DEVICE_HOME} -type d -name gcc )
NEW_CMSIS_DEV_SRC_F=$(find ${DEVICE_HOME} -type f -name system_stm32f4xx.c)

NEW_CMSIS_CORE_F=$(find $1 -type f -name cmsis_gcc.h )
NEW_CMSIS_CORE_D=${NEW_CMSIS_CORE_F%/*} # Extract only Directory path

NEW_HAL_D=$(find $1 -type d -name STM32F4xx_HAL_Driver )
NEW_LINKER_F=$(find $1 -type f -name *.ld )

# Check Existence of Files
check_exist_f ${NEW_HAL_D}/Src stm32f*.c
check_exist_f ${NEW_HAL_D}/Inc stm32*.h
check_exist_f ${NEW_CMSIS_CORE_D} *.h
check_exist_f ${NEW_CMSIS_DEV_INC_D} *.h
check_exist_f ${NEW_CMSIS_DEV_GCC_D} startup_stm32f4*.s
echo

read -p "Updata Driver? (y/N): " yn
case "$yn" in 
    [yY]*) ;; 
    *) exit 1 ;;
esac

# Move Files from New Driver to Project Direvtory
cp ${NEW_HAL_D}/Src/* ${MAIN_HAL_SRC}
cp -R ${NEW_HAL_D}/Inc/* ${MAIN_HAL_INC}
cp ${NEW_CMSIS_DEV_GCC_D}/* ${MAIN_CMSIS_DEV_GCC}
cp ${NEW_CMSIS_DEV_INC_D}/* ${MAIN_CMSIS_DEV_INC}
cp ${NEW_CMSIS_DEV_SRC_F} ${MAIN_CMSIS_DEV_SRC}
cp ${NEW_CMSIS_CORE_D}/* ${MAIN_CMSIS_CORE} 
cp ${NEW_LINKER_F} ${MAIN_LINKERS} 2>/dev/null # igonre overwrite messages 

rm $(find ./Drivers/ -name *template.*) # remove template files in hal driver

# check differnce after updated
git add .
git status