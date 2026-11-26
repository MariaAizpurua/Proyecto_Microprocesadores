################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1200/ccs/ccs_base/msp430/include" --include_path="{PROJECT_ROOT}/drivers" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/hal" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/own_libraries" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/_ti_grlib_3.21.00.00" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/_ti_grlib_3.21.00.00/fonts" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/_ti_driverlib_2.91.13.01" --include_path="C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --include_path="C:/Tecnun/4. Cuarto/Microprocesadores/Proyecto_Microprocesadores/edumkiilib_empty/include" --advice:hw_config=all --define=__MSP430FR5994__ --printf_support=minimal --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


