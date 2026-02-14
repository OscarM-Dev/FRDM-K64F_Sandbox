################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include mmcau/subdir.mk
-include mbedtls/port/ksdk/subdir.mk
-include mbedtls/library/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := MQTT_practice
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: MQTT_practice.axf

# Tool invocations
MQTT_practice.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mmcau\asm-cm4-cm7" -Xlinker -Map="MQTT_practice.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T frdmk64f_mbedtls_benchmark_Debug.ld -o "MQTT_practice.axf" $(OBJS) $(USER_OBJS) $(LIBS) -l_mmcau
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) MQTT_practice.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MQTT_practice.axf"; # arm-none-eabi-objcopy -v -O binary "MQTT_practice.axf" "MQTT_practice.bin" ; # checksum -p MK64FN1M0xxx12 -d "MQTT_practice.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
