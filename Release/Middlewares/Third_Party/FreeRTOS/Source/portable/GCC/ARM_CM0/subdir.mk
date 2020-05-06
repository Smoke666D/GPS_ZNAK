################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F051x8 -c -I../Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I/ZNAK_GPS_UP/Middlewares/Third_Party -I/ZNAK_GPS_UP/Middlewares/Third_Party/FreeRTOS -I/ZNAK_GPS_UP/Middlewares/Third_Party/FreeRTOS/Source -I/ZNAK_GPS_UP/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/ZNAK_GPS_UP/Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/dymov/Desktop/SOFTWARE/ZNAK_GPS_UP/Middlewares/Third_Party/FreeRTOS/Source/include" -Og -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

