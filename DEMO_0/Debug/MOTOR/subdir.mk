################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MOTOR/MOTOR_GLOBAL.cpp \
../MOTOR/PID.cpp \
../MOTOR/PWM.cpp \
../MOTOR/motor.cpp 

OBJS += \
./MOTOR/MOTOR_GLOBAL.o \
./MOTOR/PID.o \
./MOTOR/PWM.o \
./MOTOR/motor.o 

CPP_DEPS += \
./MOTOR/MOTOR_GLOBAL.d \
./MOTOR/PID.d \
./MOTOR/PWM.d \
./MOTOR/motor.d 


# Each subdirectory must supply rules for building sources it contributes
MOTOR/%.o: ../MOTOR/%.cpp MOTOR/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/sun20/Desktop/PID_BrushMotor/DEMO_0/MOTOR" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MOTOR

clean-MOTOR:
	-$(RM) ./MOTOR/MOTOR_GLOBAL.d ./MOTOR/MOTOR_GLOBAL.o ./MOTOR/PID.d ./MOTOR/PID.o ./MOTOR/PWM.d ./MOTOR/PWM.o ./MOTOR/motor.d ./MOTOR/motor.o

.PHONY: clean-MOTOR

