################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/event_queue.c \
../Src/gpio_signal.c \
../Src/hc-sr501pir_sensor.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/usart.c 

OBJS += \
./Src/event_queue.o \
./Src/gpio_signal.o \
./Src/hc-sr501pir_sensor.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/usart.o 

C_DEPS += \
./Src/event_queue.d \
./Src/gpio_signal.d \
./Src/hc-sr501pir_sensor.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/event_queue.o: ../Src/event_queue.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/event_queue.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/gpio_signal.o: ../Src/gpio_signal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio_signal.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/hc-sr501pir_sensor.o: ../Src/hc-sr501pir_sensor.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/hc-sr501pir_sensor.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/usart.o: ../Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -DDEBUG -c -I../Inc -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Core_A/Include" -I"C:/Users/mayan/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -I"C:/Users/Owner/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usart.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

