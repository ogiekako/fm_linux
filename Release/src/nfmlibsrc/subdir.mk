################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/nfmlibsrc/console.c \
../src/nfmlibsrc/mt19937i.c 

OBJS += \
./src/nfmlibsrc/console.o \
./src/nfmlibsrc/mt19937i.o 

C_DEPS += \
./src/nfmlibsrc/console.d \
./src/nfmlibsrc/mt19937i.d 


# Each subdirectory must supply rules for building sources it contributes
src/nfmlibsrc/%.o: ../src/nfmlibsrc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


