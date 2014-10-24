################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fmza6005019src/analyze.c \
../src/fmza6005019src/arrange.c \
../src/fmza6005019src/d_check.c \
../src/fmza6005019src/d_escape.c \
../src/fmza6005019src/data.c \
../src/fmza6005019src/diff.c \
../src/fmza6005019src/fmza.c \
../src/fmza6005019src/h_check.c \
../src/fmza6005019src/h_escape.c \
../src/fmza6005019src/hist.c \
../src/fmza6005019src/hs_check.c \
../src/fmza6005019src/hs_escape.c \
../src/fmza6005019src/main.c \
../src/fmza6005019src/mate.c \
../src/fmza6005019src/move.c \
../src/fmza6005019src/option.c \
../src/fmza6005019src/piece.c \
../src/fmza6005019src/piece_data.c \
../src/fmza6005019src/print.c \
../src/fmza6005019src/restore.c \
../src/fmza6005019src/sub.c \
../src/fmza6005019src/sub2.c \
../src/fmza6005019src/sub3.c 

OBJS += \
./src/fmza6005019src/analyze.o \
./src/fmza6005019src/arrange.o \
./src/fmza6005019src/d_check.o \
./src/fmza6005019src/d_escape.o \
./src/fmza6005019src/data.o \
./src/fmza6005019src/diff.o \
./src/fmza6005019src/fmza.o \
./src/fmza6005019src/h_check.o \
./src/fmza6005019src/h_escape.o \
./src/fmza6005019src/hist.o \
./src/fmza6005019src/hs_check.o \
./src/fmza6005019src/hs_escape.o \
./src/fmza6005019src/main.o \
./src/fmza6005019src/mate.o \
./src/fmza6005019src/move.o \
./src/fmza6005019src/option.o \
./src/fmza6005019src/piece.o \
./src/fmza6005019src/piece_data.o \
./src/fmza6005019src/print.o \
./src/fmza6005019src/restore.o \
./src/fmza6005019src/sub.o \
./src/fmza6005019src/sub2.o \
./src/fmza6005019src/sub3.o 

C_DEPS += \
./src/fmza6005019src/analyze.d \
./src/fmza6005019src/arrange.d \
./src/fmza6005019src/d_check.d \
./src/fmza6005019src/d_escape.d \
./src/fmza6005019src/data.d \
./src/fmza6005019src/diff.d \
./src/fmza6005019src/fmza.d \
./src/fmza6005019src/h_check.d \
./src/fmza6005019src/h_escape.d \
./src/fmza6005019src/hist.d \
./src/fmza6005019src/hs_check.d \
./src/fmza6005019src/hs_escape.d \
./src/fmza6005019src/main.d \
./src/fmza6005019src/mate.d \
./src/fmza6005019src/move.d \
./src/fmza6005019src/option.d \
./src/fmza6005019src/piece.d \
./src/fmza6005019src/piece_data.d \
./src/fmza6005019src/print.d \
./src/fmza6005019src/restore.d \
./src/fmza6005019src/sub.d \
./src/fmza6005019src/sub2.d \
./src/fmza6005019src/sub3.d 


# Each subdirectory must supply rules for building sources it contributes
src/fmza6005019src/%.o: ../src/fmza6005019src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


