################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ServoTest.lss \

FLASH_IMAGE += \
ServoTest.hex \

EEPROM_IMAGE += \
ServoTest.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ServoTest.elf secondary-outputs

# Tool invocations
ServoTest.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ServoTest.map -L/Users/jimparker/Dropbox/Jim_Parker_Labs/AVR/includes -mmcu=atmega168 -o "ServoTest.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ServoTest.lss: ServoTest.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ServoTest.elf  >"ServoTest.lss"
	@echo 'Finished building: $@'
	@echo ' '

ServoTest.hex: ServoTest.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ServoTest.elf  "ServoTest.hex"
	@echo 'Finished building: $@'
	@echo ' '

ServoTest.eep: ServoTest.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ServoTest.elf  "ServoTest.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ServoTest.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega168 ServoTest.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) ServoTest.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
