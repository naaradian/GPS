################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
RF1A.obj: ../RF1A.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="RF1A.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

RfRegSettings.obj: ../RfRegSettings.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="RfRegSettings.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

bufferspi.obj: ../bufferspi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="bufferspi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

config.obj: ../config.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="config.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

events.obj: ../events.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="events.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

gps.obj: ../gps.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="gps.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_SPI.obj: ../hal_SPI.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_SPI.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_pmm.obj: ../hal_pmm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_pmm.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

i2c.obj: ../i2c.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="i2c.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mmc.obj: ../mmc.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mmc.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

radio.obj: ../radio.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="radio.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

spi.obj: ../spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="spi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timers.obj: ../timers.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="timers.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O2 --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="uart.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


