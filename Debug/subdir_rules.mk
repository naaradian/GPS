################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
bufferspi.obj: ../bufferspi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="bufferspi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

config.obj: ../config.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="config.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_SPI.obj: ../hal_SPI.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="hal_SPI.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mmc.obj: ../mmc.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="mmc.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

spi.obj: ../spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccs700/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/naa/workspace_v9/GPS/MMC_lib" --include_path="C:/Users/naa/workspace_v9/GPS/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs700/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" -g --define=DEPRECATED --define=__CC430F6137__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="spi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


