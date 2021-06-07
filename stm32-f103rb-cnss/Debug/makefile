################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32-f103rb-cnss.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32-f103rb-cnss.list \

OBJCOPY_BIN += \
stm32-f103rb-cnss.bin \


# All Target
all: stm32-f103rb-cnss.elf secondary-outputs

# Tool invocations
stm32-f103rb-cnss.elf: $(OBJS) $(USER_OBJS) C:\Mayan\Repository\ cnss\cnss-embedded\stm32-f103rb-cnss\STM32F103RBTX_FLASH.ld
	arm-none-eabi-gcc -o "stm32-f103rb-cnss.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Mayan\Repository cnss\cnss-embedded\stm32-f103rb-cnss\STM32F103RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32-f103rb-cnss.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32-f103rb-cnss.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32-f103rb-cnss.list"
	@echo 'Finished building: $@'
	@echo ' '

stm32-f103rb-cnss.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "stm32-f103rb-cnss.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
