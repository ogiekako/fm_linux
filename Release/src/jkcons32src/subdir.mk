################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/jkcons32src/bschar32.c \
../src/jkcons32src/clrln32.c \
../src/jkcons32src/clrscr32.c \
../src/jkcons32src/cr32.c \
../src/jkcons32src/curbk32.c \
../src/jkcons32src/curon32.c \
../src/jkcons32src/curup32.c \
../src/jkcons32src/delay32.c \
../src/jkcons32src/erscsb32.c \
../src/jkcons32src/erseof32.c \
../src/jkcons32src/erseos32.c \
../src/jkcons32src/getcls32.c \
../src/jkcons32src/getcon32.c \
../src/jkcons32src/getcp32.c \
../src/jkcons32src/getmem32.c \
../src/jkcons32src/getnor32.c \
../src/jkcons32src/inicon32.c \
../src/jkcons32src/inscon32.c \
../src/jkcons32src/locate32.c \
../src/jkcons32src/pushcp32.c \
../src/jkcons32src/putcon32.c \
../src/jkcons32src/setcon32.c \
../src/jkcons32src/setttl32.c \
../src/jkcons32src/settxa32.c 

OBJS += \
./src/jkcons32src/bschar32.o \
./src/jkcons32src/clrln32.o \
./src/jkcons32src/clrscr32.o \
./src/jkcons32src/cr32.o \
./src/jkcons32src/curbk32.o \
./src/jkcons32src/curon32.o \
./src/jkcons32src/curup32.o \
./src/jkcons32src/delay32.o \
./src/jkcons32src/erscsb32.o \
./src/jkcons32src/erseof32.o \
./src/jkcons32src/erseos32.o \
./src/jkcons32src/getcls32.o \
./src/jkcons32src/getcon32.o \
./src/jkcons32src/getcp32.o \
./src/jkcons32src/getmem32.o \
./src/jkcons32src/getnor32.o \
./src/jkcons32src/inicon32.o \
./src/jkcons32src/inscon32.o \
./src/jkcons32src/locate32.o \
./src/jkcons32src/pushcp32.o \
./src/jkcons32src/putcon32.o \
./src/jkcons32src/setcon32.o \
./src/jkcons32src/setttl32.o \
./src/jkcons32src/settxa32.o 

C_DEPS += \
./src/jkcons32src/bschar32.d \
./src/jkcons32src/clrln32.d \
./src/jkcons32src/clrscr32.d \
./src/jkcons32src/cr32.d \
./src/jkcons32src/curbk32.d \
./src/jkcons32src/curon32.d \
./src/jkcons32src/curup32.d \
./src/jkcons32src/delay32.d \
./src/jkcons32src/erscsb32.d \
./src/jkcons32src/erseof32.d \
./src/jkcons32src/erseos32.d \
./src/jkcons32src/getcls32.d \
./src/jkcons32src/getcon32.d \
./src/jkcons32src/getcp32.d \
./src/jkcons32src/getmem32.d \
./src/jkcons32src/getnor32.d \
./src/jkcons32src/inicon32.d \
./src/jkcons32src/inscon32.d \
./src/jkcons32src/locate32.d \
./src/jkcons32src/pushcp32.d \
./src/jkcons32src/putcon32.d \
./src/jkcons32src/setcon32.d \
./src/jkcons32src/setttl32.d \
./src/jkcons32src/settxa32.d 


# Each subdirectory must supply rules for building sources it contributes
src/jkcons32src/%.o: ../src/jkcons32src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


