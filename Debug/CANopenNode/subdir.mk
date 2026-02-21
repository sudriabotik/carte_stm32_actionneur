################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CANopenNode/CANopen.c 

OBJS += \
./CANopenNode/CANopen.o 

C_DEPS += \
./CANopenNode/CANopen.d 


# Each subdirectory must supply rules for building sources it contributes
CANopenNode/%.o CANopenNode/%.su CANopenNode/%.cyclo: ../CANopenNode/%.c CANopenNode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../User/inc -I../lib_robot/inc_lib -I../CANopenNode/ -I../CANopenNode_STM32/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CANopenNode

clean-CANopenNode:
	-$(RM) ./CANopenNode/CANopen.cyclo ./CANopenNode/CANopen.d ./CANopenNode/CANopen.o ./CANopenNode/CANopen.su

.PHONY: clean-CANopenNode

