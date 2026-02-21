################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib_robot/src_lib/ax_controller.c \
../lib_robot/src_lib/encoders.c \
../lib_robot/src_lib/logicfuncs.c \
../lib_robot/src_lib/mathfuncs.c \
../lib_robot/src_lib/motor_asserv.c \
../lib_robot/src_lib/motors.c \
../lib_robot/src_lib/pid.c 

OBJS += \
./lib_robot/src_lib/ax_controller.o \
./lib_robot/src_lib/encoders.o \
./lib_robot/src_lib/logicfuncs.o \
./lib_robot/src_lib/mathfuncs.o \
./lib_robot/src_lib/motor_asserv.o \
./lib_robot/src_lib/motors.o \
./lib_robot/src_lib/pid.o 

C_DEPS += \
./lib_robot/src_lib/ax_controller.d \
./lib_robot/src_lib/encoders.d \
./lib_robot/src_lib/logicfuncs.d \
./lib_robot/src_lib/mathfuncs.d \
./lib_robot/src_lib/motor_asserv.d \
./lib_robot/src_lib/motors.d \
./lib_robot/src_lib/pid.d 


# Each subdirectory must supply rules for building sources it contributes
lib_robot/src_lib/%.o lib_robot/src_lib/%.su lib_robot/src_lib/%.cyclo: ../lib_robot/src_lib/%.c lib_robot/src_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../User/inc -I../lib_robot/inc_lib -I../CANopenNode/ -I../CANopenNode_STM32/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib_robot-2f-src_lib

clean-lib_robot-2f-src_lib:
	-$(RM) ./lib_robot/src_lib/ax_controller.cyclo ./lib_robot/src_lib/ax_controller.d ./lib_robot/src_lib/ax_controller.o ./lib_robot/src_lib/ax_controller.su ./lib_robot/src_lib/encoders.cyclo ./lib_robot/src_lib/encoders.d ./lib_robot/src_lib/encoders.o ./lib_robot/src_lib/encoders.su ./lib_robot/src_lib/logicfuncs.cyclo ./lib_robot/src_lib/logicfuncs.d ./lib_robot/src_lib/logicfuncs.o ./lib_robot/src_lib/logicfuncs.su ./lib_robot/src_lib/mathfuncs.cyclo ./lib_robot/src_lib/mathfuncs.d ./lib_robot/src_lib/mathfuncs.o ./lib_robot/src_lib/mathfuncs.su ./lib_robot/src_lib/motor_asserv.cyclo ./lib_robot/src_lib/motor_asserv.d ./lib_robot/src_lib/motor_asserv.o ./lib_robot/src_lib/motor_asserv.su ./lib_robot/src_lib/motors.cyclo ./lib_robot/src_lib/motors.d ./lib_robot/src_lib/motors.o ./lib_robot/src_lib/motors.su ./lib_robot/src_lib/pid.cyclo ./lib_robot/src_lib/pid.d ./lib_robot/src_lib/pid.o ./lib_robot/src_lib/pid.su

.PHONY: clean-lib_robot-2f-src_lib

