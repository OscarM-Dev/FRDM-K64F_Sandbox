################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK64F12.c 

C_DEPS += \
./device/system_MK64F12.d 

OBJS += \
./device/system_MK64F12.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1 -DFREESCALE_KSDK_BM -DMBEDTLS_CONFIG_FILE='"ksdk_mbedtls_config.h"' -DFRDM_K64F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\source" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\drivers" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mmcau" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\include" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\library" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\port\ksdk" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\utilities" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\device" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\uart" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\serial_manager" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\lists" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\CMSIS" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\board" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\frdmk64f\mbedtls_examples\mbedtls_benchmark" -O0 -fno-common -g3 -gdwarf-4 -fomit-frame-pointer -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MK64F12.d ./device/system_MK64F12.o

.PHONY: clean-device

