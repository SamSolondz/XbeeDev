################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/EFM32PG12B/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PatientTracker_Xbee2.axf

# Tool invocations
PatientTracker_Xbee2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "PatientTracker_Xbee2.ld" -Xlinker --gc-sections -Xlinker -Map="PatientTracker_Xbee2.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o PatientTracker_Xbee2.axf "./CMSIS/EFM32PG12B/startup_efm32pg12b.o" "./CMSIS/EFM32PG12B/system_efm32pg12b.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_ldma.o" "./emlib/em_letimer.o" "./emlib/em_leuart.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./src/ATCommandsLib.o" "./src/My_CMU.o" "./src/My_LEUART.o" "./src/My_Logger.o" "./src/My_Sleep.o" "./src/My_UART.o" "./src/letimer.o" "./src/log.o" "./src/main.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: PatientTracker_Xbee2.hex'
	arm-none-eabi-objcopy -O ihex "PatientTracker_Xbee2.axf" "PatientTracker_Xbee2.hex"
	@echo ' '

	@echo 'Building bin file: PatientTracker_Xbee2.bin'
	arm-none-eabi-objcopy -O binary "PatientTracker_Xbee2.axf" "PatientTracker_Xbee2.bin"
	@echo ' '

	@echo 'Building s37 file: PatientTracker_Xbee2.s37'
	arm-none-eabi-objcopy -O srec "PatientTracker_Xbee2.axf" "PatientTracker_Xbee2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "PatientTracker_Xbee2.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) PatientTracker_Xbee2.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
