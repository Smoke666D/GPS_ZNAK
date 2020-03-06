################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/KL3333.c \
../Src/LPS25HB_Driver.c \
../Src/NMEA.c \
../Src/adc.c \
../Src/dac.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f0xx_hal_msp.c \
../Src/stm32f0xx_it.c \
../Src/system_stm32f0xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/KL3333.o \
./Src/LPS25HB_Driver.o \
./Src/NMEA.o \
./Src/adc.o \
./Src/dac.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/stm32f0xx_hal_msp.o \
./Src/stm32f0xx_it.o \
./Src/system_stm32f0xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/KL3333.d \
./Src/LPS25HB_Driver.d \
./Src/NMEA.d \
./Src/adc.d \
./Src/dac.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/stm32f0xx_hal_msp.d \
./Src/stm32f0xx_it.d \
./Src/system_stm32f0xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F051x8 -I"C:/Users/dymov/Documents/ZNAK_GPS_UP/Inc" -I"C:/Users/dymov/Documents/ZNAK_GPS_UP/Drivers/STM32F0xx_HAL_Driver/Inc" -I"C:/Users/dymov/Documents/ZNAK_GPS_UP/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy" -I"C:/Users/dymov/Documents/ZNAK_GPS_UP/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"C:/Users/dymov/Documents/ZNAK_GPS_UP/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


