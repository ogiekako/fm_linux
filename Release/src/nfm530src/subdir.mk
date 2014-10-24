################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/nfm530src/check.c \
../src/nfm530src/data.c \
../src/nfm530src/debug.c \
../src/nfm530src/escape.c \
../src/nfm530src/getprob.c \
../src/nfm530src/hash.c \
../src/nfm530src/hist.c \
../src/nfm530src/main.c \
../src/nfm530src/mate.c \
../src/nfm530src/message.c \
../src/nfm530src/output.c \
../src/nfm530src/phase.c \
../src/nfm530src/sub.c 

OBJS += \
./src/nfm530src/check.o \
./src/nfm530src/data.o \
./src/nfm530src/debug.o \
./src/nfm530src/escape.o \
./src/nfm530src/getprob.o \
./src/nfm530src/hash.o \
./src/nfm530src/hist.o \
./src/nfm530src/main.o \
./src/nfm530src/mate.o \
./src/nfm530src/message.o \
./src/nfm530src/output.o \
./src/nfm530src/phase.o \
./src/nfm530src/sub.o 

C_DEPS += \
./src/nfm530src/check.d \
./src/nfm530src/data.d \
./src/nfm530src/debug.d \
./src/nfm530src/escape.d \
./src/nfm530src/getprob.d \
./src/nfm530src/hash.d \
./src/nfm530src/hist.d \
./src/nfm530src/main.d \
./src/nfm530src/mate.d \
./src/nfm530src/message.d \
./src/nfm530src/output.d \
./src/nfm530src/phase.d \
./src/nfm530src/sub.d 


# Each subdirectory must supply rules for building sources it contributes
src/nfm530src/%.o: ../src/nfm530src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


