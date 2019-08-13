################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H743xx '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Inc" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/STM32H7xx_HAL_Driver/Inc" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/system" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/mbedTLS/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/prot" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/netif" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/posix" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/system/arch" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/mbedTLS/include/mbedtls" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/BSP/Components/lan8742"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


