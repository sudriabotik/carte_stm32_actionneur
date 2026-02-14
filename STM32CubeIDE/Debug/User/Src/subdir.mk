################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/ax_controller.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/coordonne_absolue.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/encoders.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/logicfuncs.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mathfuncs.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/motor_asserv.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/motors.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/movement_statemachine.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mv_statemachine.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mv_statemachine_states.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/pid.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/pid_config.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/recorder.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/robot_data.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/state_machine.c \
C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/uart_messenger.c 

OBJS += \
./User/Src/ax_controller.o \
./User/Src/coordonne_absolue.o \
./User/Src/encoders.o \
./User/Src/logicfuncs.o \
./User/Src/mathfuncs.o \
./User/Src/motor_asserv.o \
./User/Src/motors.o \
./User/Src/movement_statemachine.o \
./User/Src/mv_statemachine.o \
./User/Src/mv_statemachine_states.o \
./User/Src/pid.o \
./User/Src/pid_config.o \
./User/Src/recorder.o \
./User/Src/robot_data.o \
./User/Src/state_machine.o \
./User/Src/uart_messenger.o 

C_DEPS += \
./User/Src/ax_controller.d \
./User/Src/coordonne_absolue.d \
./User/Src/encoders.d \
./User/Src/logicfuncs.d \
./User/Src/mathfuncs.d \
./User/Src/motor_asserv.d \
./User/Src/motors.d \
./User/Src/movement_statemachine.d \
./User/Src/mv_statemachine.d \
./User/Src/mv_statemachine_states.d \
./User/Src/pid.d \
./User/Src/pid_config.d \
./User/Src/recorder.d \
./User/Src/robot_data.d \
./User/Src/state_machine.d \
./User/Src/uart_messenger.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/ax_controller.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/ax_controller.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/coordonne_absolue.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/coordonne_absolue.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/encoders.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/encoders.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/logicfuncs.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/logicfuncs.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/mathfuncs.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mathfuncs.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/motor_asserv.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/motor_asserv.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/motors.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/motors.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/movement_statemachine.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/movement_statemachine.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/mv_statemachine.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mv_statemachine.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/mv_statemachine_states.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/mv_statemachine_states.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/pid.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/pid.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/pid_config.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/pid_config.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/recorder.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/recorder.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/robot_data.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/robot_data.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/state_machine.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/state_machine.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Src/uart_messenger.o: C:/Users/PAUL/OneDrive\ -\ ESME/Documents/botique/2026/info/stm32_project/stm32_carte_actionneur/carte_stm32_actionneur/User/Src/uart_messenger.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../User/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/ax_controller.cyclo ./User/Src/ax_controller.d ./User/Src/ax_controller.o ./User/Src/ax_controller.su ./User/Src/coordonne_absolue.cyclo ./User/Src/coordonne_absolue.d ./User/Src/coordonne_absolue.o ./User/Src/coordonne_absolue.su ./User/Src/encoders.cyclo ./User/Src/encoders.d ./User/Src/encoders.o ./User/Src/encoders.su ./User/Src/logicfuncs.cyclo ./User/Src/logicfuncs.d ./User/Src/logicfuncs.o ./User/Src/logicfuncs.su ./User/Src/mathfuncs.cyclo ./User/Src/mathfuncs.d ./User/Src/mathfuncs.o ./User/Src/mathfuncs.su ./User/Src/motor_asserv.cyclo ./User/Src/motor_asserv.d ./User/Src/motor_asserv.o ./User/Src/motor_asserv.su ./User/Src/motors.cyclo ./User/Src/motors.d ./User/Src/motors.o ./User/Src/motors.su ./User/Src/movement_statemachine.cyclo ./User/Src/movement_statemachine.d ./User/Src/movement_statemachine.o ./User/Src/movement_statemachine.su ./User/Src/mv_statemachine.cyclo ./User/Src/mv_statemachine.d ./User/Src/mv_statemachine.o ./User/Src/mv_statemachine.su ./User/Src/mv_statemachine_states.cyclo ./User/Src/mv_statemachine_states.d ./User/Src/mv_statemachine_states.o ./User/Src/mv_statemachine_states.su ./User/Src/pid.cyclo ./User/Src/pid.d ./User/Src/pid.o ./User/Src/pid.su ./User/Src/pid_config.cyclo ./User/Src/pid_config.d ./User/Src/pid_config.o ./User/Src/pid_config.su ./User/Src/recorder.cyclo ./User/Src/recorder.d ./User/Src/recorder.o ./User/Src/recorder.su ./User/Src/robot_data.cyclo ./User/Src/robot_data.d ./User/Src/robot_data.o ./User/Src/robot_data.su ./User/Src/state_machine.cyclo ./User/Src/state_machine.d ./User/Src/state_machine.o ./User/Src/state_machine.su ./User/Src/uart_messenger.cyclo ./User/Src/uart_messenger.d ./User/Src/uart_messenger.o ./User/Src/uart_messenger.su

.PHONY: clean-User-2f-Src

