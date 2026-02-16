################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/ax_controller.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/encoders.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/logicfuncs.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/mathfuncs.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/motor_asserv.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/motors.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/pid.c 

OBJS += \
./src_lib/ax_controller.o \
./src_lib/encoders.o \
./src_lib/logicfuncs.o \
./src_lib/mathfuncs.o \
./src_lib/motor_asserv.o \
./src_lib/motors.o \
./src_lib/pid.o 

C_DEPS += \
./src_lib/ax_controller.d \
./src_lib/encoders.d \
./src_lib/logicfuncs.d \
./src_lib/mathfuncs.d \
./src_lib/motor_asserv.d \
./src_lib/motors.d \
./src_lib/pid.d 


# Each subdirectory must supply rules for building sources it contributes
src_lib/ax_controller.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/ax_controller.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/encoders.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/encoders.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/logicfuncs.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/logicfuncs.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/mathfuncs.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/mathfuncs.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/motor_asserv.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/motor_asserv.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/motors.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/motors.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src_lib/pid.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/lib_robot/src_lib/pid.c src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -I../../lib_robot/inc_lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src_lib

clean-src_lib:
	-$(RM) ./src_lib/ax_controller.cyclo ./src_lib/ax_controller.d ./src_lib/ax_controller.o ./src_lib/ax_controller.su ./src_lib/encoders.cyclo ./src_lib/encoders.d ./src_lib/encoders.o ./src_lib/encoders.su ./src_lib/logicfuncs.cyclo ./src_lib/logicfuncs.d ./src_lib/logicfuncs.o ./src_lib/logicfuncs.su ./src_lib/mathfuncs.cyclo ./src_lib/mathfuncs.d ./src_lib/mathfuncs.o ./src_lib/mathfuncs.su ./src_lib/motor_asserv.cyclo ./src_lib/motor_asserv.d ./src_lib/motor_asserv.o ./src_lib/motor_asserv.su ./src_lib/motors.cyclo ./src_lib/motors.d ./src_lib/motors.o ./src_lib/motors.su ./src_lib/pid.cyclo ./src_lib/pid.d ./src_lib/pid.o ./src_lib/pid.su

.PHONY: clean-src_lib

