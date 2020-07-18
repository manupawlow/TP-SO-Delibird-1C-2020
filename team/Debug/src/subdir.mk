################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/funciones_auxiliares.c \
../src/hilos.c \
../src/team.c 

OBJS += \
./src/funciones_auxiliares.o \
./src/hilos.o \
./src/team.o 

C_DEPS += \
./src/funciones_auxiliares.d \
./src/hilos.d \
./src/team.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-NN/bibliotec/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/hilos.o: ../src/hilos.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-NN/bibliotec/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/hilos.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


