################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/%.obj: ../drivers/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.4.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=5 --use_hw_mpy=F5 --include_path="C:/ti/ccs1031/ccs/ccs_base/msp430/include" --include_path="C:/Temp/edumkiilib_empty" --include_path="C:/Temp/edumkiilib_empty/_ti_driverlib_2.91.13.01" --include_path="C:/Temp/edumkiilib_empty/_ti_grlib_3.21.00.00/fonts" --include_path="C:/Temp/edumkiilib_empty/_ti_grlib_3.21.00.00" --include_path="C:/Temp/edumkiilib_empty/drivers" --include_path="C:/Temp/edumkiilib_empty/hal" --include_path="C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.4.LTS/include" --advice:power="none" --advice:hw_config=all --define=__MSP430FR5994__ --define=_MPU_ENABLE -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="drivers/$(basename $(<F)).d_raw" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


