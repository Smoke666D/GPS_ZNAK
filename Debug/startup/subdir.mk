################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f051x8.s 

OBJS += \
./startup/startup_stm32f051x8.o 

S_DEPS += \
./startup/startup_stm32f051x8.d 


# Each subdirectory must supply rules for building sources it contributes
startup/startup_stm32f051x8.o: ../startup/startup_stm32f051x8.s
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -x assembler-with-cpp -MMD -MP -MF"startup/startup_stm32f051x8.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

