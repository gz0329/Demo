################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/app_tick.c \
../Src/ethernetif.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/hrtim.c \
../Src/lwip.c \
../Src/main.c \
../Src/mbedtls.c \
../Src/net_sockets.c \
../Src/spi.c \
../Src/stm32h7xx_hal_msp.c \
../Src/stm32h7xx_hal_timebase_TIM.c \
../Src/stm32h7xx_it.c \
../Src/synthesizer_driver.c \
../Src/syscalls.c \
../Src/system_stm32h7xx.c \
../Src/tim.c \
../Src/time_to_service.c \
../Src/usart.c 

OBJS += \
./Src/app_tick.o \
./Src/ethernetif.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/hrtim.o \
./Src/lwip.o \
./Src/main.o \
./Src/mbedtls.o \
./Src/net_sockets.o \
./Src/spi.o \
./Src/stm32h7xx_hal_msp.o \
./Src/stm32h7xx_hal_timebase_TIM.o \
./Src/stm32h7xx_it.o \
./Src/synthesizer_driver.o \
./Src/syscalls.o \
./Src/system_stm32h7xx.o \
./Src/tim.o \
./Src/time_to_service.o \
./Src/usart.o 

C_DEPS += \
./Src/app_tick.d \
./Src/ethernetif.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/hrtim.d \
./Src/lwip.d \
./Src/main.d \
./Src/mbedtls.d \
./Src/net_sockets.d \
./Src/spi.d \
./Src/stm32h7xx_hal_msp.d \
./Src/stm32h7xx_hal_timebase_TIM.d \
./Src/stm32h7xx_it.d \
./Src/synthesizer_driver.d \
./Src/syscalls.d \
./Src/system_stm32h7xx.d \
./Src/tim.d \
./Src/time_to_service.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H743xx '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Inc" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/STM32H7xx_HAL_Driver/Inc" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/CMSIS/Include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/system" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/mbedTLS/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/include" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/lwip/prot" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/netif" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/posix" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/LwIP/system/arch" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Middlewares/Third_Party/mbedTLS/include/mbedtls" -I"E:/Dev/ellbayind-mcfly/Local_Oscillator/Software/LO/Drivers/BSP/Components/lan8742"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


