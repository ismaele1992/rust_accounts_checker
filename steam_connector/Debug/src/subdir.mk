################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Players.cpp \
../src/Servers.cpp \
../src/SteamAPI.cpp \
../src/steam_connector.cpp 

OBJS += \
./src/Players.o \
./src/Servers.o \
./src/SteamAPI.o \
./src/steam_connector.o 

CPP_DEPS += \
./src/Players.d \
./src/Servers.d \
./src/SteamAPI.d \
./src/steam_connector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


