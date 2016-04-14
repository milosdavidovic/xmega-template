################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../1wire.c \
../ADC.c \
../calendar.c \
../clock.c \
../ds18b20.c \
../gsm.c \
../main.c \
../peripherals.c \
../power.c \
../RTC.c \
../system.c \
../telit_gl865.c \
../timers.c \
../trash_sonar_mb7137.c \
../twi.c \
../Usart.c \
../watchdog.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
1wire.o \
ADC.o \
calendar.o \
clock.o \
ds18b20.o \
gsm.o \
main.o \
peripherals.o \
power.o \
RTC.o \
system.o \
telit_gl865.o \
timers.o \
trash_sonar_mb7137.o \
twi.o \
Usart.o \
watchdog.o

OBJS_AS_ARGS +=  \
1wire.o \
ADC.o \
calendar.o \
clock.o \
ds18b20.o \
gsm.o \
main.o \
peripherals.o \
power.o \
RTC.o \
system.o \
telit_gl865.o \
timers.o \
trash_sonar_mb7137.o \
twi.o \
Usart.o \
watchdog.o

C_DEPS +=  \
1wire.d \
ADC.d \
calendar.d \
clock.d \
ds18b20.d \
gsm.d \
main.d \
peripherals.d \
power.d \
RTC.d \
system.d \
telit_gl865.d \
timers.d \
trash_sonar_mb7137.d \
twi.d \
Usart.d \
watchdog.d

C_DEPS_AS_ARGS +=  \
1wire.d \
ADC.d \
calendar.d \
clock.d \
ds18b20.d \
gsm.d \
main.d \
peripherals.d \
power.d \
RTC.d \
system.d \
telit_gl865.d \
timers.d \
trash_sonar_mb7137.d \
twi.d \
Usart.d \
watchdog.d

OUTPUT_FILE_PATH +=xmega-template.elf

OUTPUT_FILE_PATH_AS_ARGS +=xmega-template.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a1u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\gcc\dev\atxmega128a1u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="xmega-template.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega128a1u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\gcc\dev\atxmega128a1u"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "xmega-template.elf" "xmega-template.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "xmega-template.elf" "xmega-template.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "xmega-template.elf" > "xmega-template.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "xmega-template.elf" "xmega-template.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "xmega-template.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "xmega-template.elf" "xmega-template.a" "xmega-template.hex" "xmega-template.lss" "xmega-template.eep" "xmega-template.map" "xmega-template.srec" "xmega-template.usersignatures"
	