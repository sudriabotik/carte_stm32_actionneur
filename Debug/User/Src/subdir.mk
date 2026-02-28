################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/can_debug.c \
../User/Src/canopen_command_processor.c \
../User/Src/coordonne_absolue.c \
../User/Src/elevator_states.c \
../User/Src/mv_statemachine.c \
../User/Src/mv_statemachine_states.c \
../User/Src/pid_config.c \
../User/Src/recorder.c \
../User/Src/robot_data.c \
../User/Src/robot_sequences.c \
../User/Src/sequencer.c \
../User/Src/state_machine.c \
../User/Src/uart_messenger.c 

OBJS += \
./User/Src/can_debug.o \
./User/Src/canopen_command_processor.o \
./User/Src/coordonne_absolue.o \
./User/Src/elevator_states.o \
./User/Src/mv_statemachine.o \
./User/Src/mv_statemachine_states.o \
./User/Src/pid_config.o \
./User/Src/recorder.o \
./User/Src/robot_data.o \
./User/Src/robot_sequences.o \
./User/Src/sequencer.o \
./User/Src/state_machine.o \
./User/Src/uart_messenger.o 

C_DEPS += \
./User/Src/can_debug.d \
./User/Src/canopen_command_processor.d \
./User/Src/coordonne_absolue.d \
./User/Src/elevator_states.d \
./User/Src/mv_statemachine.d \
./User/Src/mv_statemachine_states.d \
./User/Src/pid_config.d \
./User/Src/recorder.d \
./User/Src/robot_data.d \
./User/Src/robot_sequences.d \
./User/Src/sequencer.d \
./User/Src/state_machine.d \
./User/Src/uart_messenger.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../User/inc -I../lib_robot/inc_lib -I../CANopenNode/ -I../CANopenNode_STM32/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/can_debug.cyclo ./User/Src/can_debug.d ./User/Src/can_debug.o ./User/Src/can_debug.su ./User/Src/canopen_command_processor.cyclo ./User/Src/canopen_command_processor.d ./User/Src/canopen_command_processor.o ./User/Src/canopen_command_processor.su ./User/Src/coordonne_absolue.cyclo ./User/Src/coordonne_absolue.d ./User/Src/coordonne_absolue.o ./User/Src/coordonne_absolue.su ./User/Src/elevator_states.cyclo ./User/Src/elevator_states.d ./User/Src/elevator_states.o ./User/Src/elevator_states.su ./User/Src/mv_statemachine.cyclo ./User/Src/mv_statemachine.d ./User/Src/mv_statemachine.o ./User/Src/mv_statemachine.su ./User/Src/mv_statemachine_states.cyclo ./User/Src/mv_statemachine_states.d ./User/Src/mv_statemachine_states.o ./User/Src/mv_statemachine_states.su ./User/Src/pid_config.cyclo ./User/Src/pid_config.d ./User/Src/pid_config.o ./User/Src/pid_config.su ./User/Src/recorder.cyclo ./User/Src/recorder.d ./User/Src/recorder.o ./User/Src/recorder.su ./User/Src/robot_data.cyclo ./User/Src/robot_data.d ./User/Src/robot_data.o ./User/Src/robot_data.su ./User/Src/robot_sequences.cyclo ./User/Src/robot_sequences.d ./User/Src/robot_sequences.o ./User/Src/robot_sequences.su ./User/Src/sequencer.cyclo ./User/Src/sequencer.d ./User/Src/sequencer.o ./User/Src/sequencer.su ./User/Src/state_machine.cyclo ./User/Src/state_machine.d ./User/Src/state_machine.o ./User/Src/state_machine.su ./User/Src/uart_messenger.cyclo ./User/Src/uart_messenger.d ./User/Src/uart_messenger.o ./User/Src/uart_messenger.su

.PHONY: clean-User-2f-Src

