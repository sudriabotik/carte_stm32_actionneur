################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/CANopenNode/303/CO_LEDs.c 

OBJS += \
./CANopenNode/303/CO_LEDs.o 

C_DEPS += \
./CANopenNode/303/CO_LEDs.d 


# Each subdirectory must supply rules for building sources it contributes
CANopenNode/303/CO_LEDs.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/CANopenNode/303/CO_LEDs.c CANopenNode/303/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -I../../CANopenNode/ -I../../CANopenNode_STM32/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CANopenNode-2f-303

clean-CANopenNode-2f-303:
	-$(RM) ./CANopenNode/303/CO_LEDs.cyclo ./CANopenNode/303/CO_LEDs.d ./CANopenNode/303/CO_LEDs.o ./CANopenNode/303/CO_LEDs.su

.PHONY: clean-CANopenNode-2f-303

