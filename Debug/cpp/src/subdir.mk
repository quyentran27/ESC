################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cpp/src/main.cpp 

OBJS += \
./cpp/src/main.o 

CPP_DEPS += \
./cpp/src/main.d 


# Each subdirectory must supply rules for building sources it contributes
cpp/src/%.o cpp/src/%.su cpp/src/%.cyclo: ../cpp/src/%.cpp cpp/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/UD/Do_An/ESC/cpp/src" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-cpp-2f-src

clean-cpp-2f-src:
	-$(RM) ./cpp/src/main.cyclo ./cpp/src/main.d ./cpp/src/main.o ./cpp/src/main.su

.PHONY: clean-cpp-2f-src

