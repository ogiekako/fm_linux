################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/jkmiscsrc/dosopt.c \
../src/jkmiscsrc/getcpjk.c \
../src/jkmiscsrc/getopt.c \
../src/jkmiscsrc/jis2sj.c \
../src/jkmiscsrc/nthctype.c \
../src/jkmiscsrc/optvar.c \
../src/jkmiscsrc/strchr.c \
../src/jkmiscsrc/strpbrk.c \
../src/jkmiscsrc/strrchr.c \
../src/jkmiscsrc/strstr.c \
../src/jkmiscsrc/strzeq.c 

OBJS += \
./src/jkmiscsrc/dosopt.o \
./src/jkmiscsrc/getcpjk.o \
./src/jkmiscsrc/getopt.o \
./src/jkmiscsrc/jis2sj.o \
./src/jkmiscsrc/nthctype.o \
./src/jkmiscsrc/optvar.o \
./src/jkmiscsrc/strchr.o \
./src/jkmiscsrc/strpbrk.o \
./src/jkmiscsrc/strrchr.o \
./src/jkmiscsrc/strstr.o \
./src/jkmiscsrc/strzeq.o 

C_DEPS += \
./src/jkmiscsrc/dosopt.d \
./src/jkmiscsrc/getcpjk.d \
./src/jkmiscsrc/getopt.d \
./src/jkmiscsrc/jis2sj.d \
./src/jkmiscsrc/nthctype.d \
./src/jkmiscsrc/optvar.d \
./src/jkmiscsrc/strchr.d \
./src/jkmiscsrc/strpbrk.d \
./src/jkmiscsrc/strrchr.d \
./src/jkmiscsrc/strstr.d \
./src/jkmiscsrc/strzeq.d 


# Each subdirectory must supply rules for building sources it contributes
src/jkmiscsrc/%.o: ../src/jkmiscsrc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


