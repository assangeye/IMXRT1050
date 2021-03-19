################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/IMXRT1050_Demo.c \
../source/semihost_hardfault.c 

OBJS += \
./source/IMXRT1050_Demo.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/IMXRT1050_Demo.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DDATA_SECTION_IS_CACHEABLE=1 -DSKIP_SYSCLK_INIT -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\board" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\source" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\drivers" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\device" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\CMSIS" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\xip" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\utilities" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\component\serial_manager" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\component\lists" -I"C:\Users\edz\Documents\MCUXpressoIDE_11.2.1_4149\workspace\IMXRT1050_Demo\component\uart" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


