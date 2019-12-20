################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 7
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BlinkLED_PIT_Z4_0.elf \

EXECUTABLES_QUOTED += \
"BlinkLED_PIT_Z4_0.elf" \

EXECUTABLES_OS_FORMAT += \
BlinkLED_PIT_Z4_0.elf \

SECONDARY_SIZE += \
BlinkLED_PIT_Z4_0.siz \

SECONDARY_SIZE_QUOTED += \
"BlinkLED_PIT_Z4_0.siz" \

SECONDARY_SIZE_OS_FORMAT += \
BlinkLED_PIT_Z4_0.siz \


# All Target
all: warning-messages explicit-dependencies BlinkLED_PIT_Z4_0.elf secondary-outputs

# Tool invocations
BlinkLED_PIT_Z4_0.elf: $(OBJS) /home/hosni/workspaceS32DS.Power.2017.R1/BlinkLED_PIT/BlinkLED_PIT_Z4_0/Project_Settings/Linker_Files/57xx_flash.ld /home/hosni/workspaceS32DS.Power.2017.R1/BlinkLED_PIT/BlinkLED_PIT_Z4_0/Project_Settings/Linker_Files/libs.ld $(OSPT) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #8 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "BlinkLED_PIT_Z4_0.elf" "@BlinkLED_PIT_Z4_0.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) $(OSPT)
	@echo 'Finished building target: $@'
	@echo ' '

BlinkLED_PIT_Z4_0.siz: $(EXECUTABLES)
	@echo 'Executing target #9 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	powerpc-eabivle-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d  ./*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

