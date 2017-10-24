################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Snake/subdir.mk
-include LCD5110/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
LCD_Snake.lss \

FLASH_IMAGE += \
LCD_Snake.hex \

EEPROM_IMAGE += \
LCD_Snake.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: LCD_Snake.elf secondary-outputs

# Tool invocations
LCD_Snake.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,LCD_Snake.map -mmcu=atmega328p -o "LCD_Snake.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD_Snake.lss: LCD_Snake.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S LCD_Snake.elf  >"LCD_Snake.lss"
	@echo 'Finished building: $@'
	@echo ' '

LCD_Snake.hex: LCD_Snake.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex LCD_Snake.elf  "LCD_Snake.hex"
	@echo 'Finished building: $@'
	@echo ' '

LCD_Snake.eep: LCD_Snake.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex LCD_Snake.elf  "LCD_Snake.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: LCD_Snake.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p LCD_Snake.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) LCD_Snake.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
