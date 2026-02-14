################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mbedtls/port/ksdk/aes_alt.c \
../mbedtls/port/ksdk/des_alt.c \
../mbedtls/port/ksdk/ecp_alt.c \
../mbedtls/port/ksdk/ecp_alt_ksdk.c \
../mbedtls/port/ksdk/ecp_curves_alt.c \
../mbedtls/port/ksdk/ksdk_mbedtls.c 

C_DEPS += \
./mbedtls/port/ksdk/aes_alt.d \
./mbedtls/port/ksdk/des_alt.d \
./mbedtls/port/ksdk/ecp_alt.d \
./mbedtls/port/ksdk/ecp_alt_ksdk.d \
./mbedtls/port/ksdk/ecp_curves_alt.d \
./mbedtls/port/ksdk/ksdk_mbedtls.d 

OBJS += \
./mbedtls/port/ksdk/aes_alt.o \
./mbedtls/port/ksdk/des_alt.o \
./mbedtls/port/ksdk/ecp_alt.o \
./mbedtls/port/ksdk/ecp_alt_ksdk.o \
./mbedtls/port/ksdk/ecp_curves_alt.o \
./mbedtls/port/ksdk/ksdk_mbedtls.o 


# Each subdirectory must supply rules for building sources it contributes
mbedtls/port/ksdk/%.o: ../mbedtls/port/ksdk/%.c mbedtls/port/ksdk/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1 -DFREESCALE_KSDK_BM -DMBEDTLS_CONFIG_FILE='"ksdk_mbedtls_config.h"' -DFRDM_K64F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\source" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\drivers" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mmcau" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\include" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\library" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\mbedtls\port\ksdk" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\utilities" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\device" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\uart" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\serial_manager" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\component\lists" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\CMSIS" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\board" -I"C:\Users\Familia\Documents\Especialidad_ITESO\Semestre_1\Redes_comunicacion\K64F_Practices\FRDM-K64F_Sandbox\frdmk64f\mbedtls_examples\mbedtls_benchmark" -O0 -fno-common -g3 -gdwarf-4 -fomit-frame-pointer -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-mbedtls-2f-port-2f-ksdk

clean-mbedtls-2f-port-2f-ksdk:
	-$(RM) ./mbedtls/port/ksdk/aes_alt.d ./mbedtls/port/ksdk/aes_alt.o ./mbedtls/port/ksdk/des_alt.d ./mbedtls/port/ksdk/des_alt.o ./mbedtls/port/ksdk/ecp_alt.d ./mbedtls/port/ksdk/ecp_alt.o ./mbedtls/port/ksdk/ecp_alt_ksdk.d ./mbedtls/port/ksdk/ecp_alt_ksdk.o ./mbedtls/port/ksdk/ecp_curves_alt.d ./mbedtls/port/ksdk/ecp_curves_alt.o ./mbedtls/port/ksdk/ksdk_mbedtls.d ./mbedtls/port/ksdk/ksdk_mbedtls.o

.PHONY: clean-mbedtls-2f-port-2f-ksdk

