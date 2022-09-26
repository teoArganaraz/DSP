################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_uart.c 

C_DEPS += \
./component/uart/fsl_adapter_uart.d 

OBJS += \
./component/uart/fsl_adapter_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\drivers" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\device" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\CMSIS" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\utilities" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\serial_manager" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\uart" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\lists" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\board" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\CMSIS\DSP\Include" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\board" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\source" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\drivers" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\device" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\CMSIS" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\utilities" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\serial_manager" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\uart" -I"C:\Users\teoas\Desktop\Teo\Academico\FCEFYN\Optativas\DSP\DSP\Lab_01_VelezNicolas\Lab_01_VelezNicolas\component\lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/fsl_adapter_uart.d ./component/uart/fsl_adapter_uart.o

.PHONY: clean-component-2f-uart

