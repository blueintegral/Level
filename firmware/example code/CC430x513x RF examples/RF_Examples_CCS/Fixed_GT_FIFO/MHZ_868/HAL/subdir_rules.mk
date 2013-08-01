################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
HAL/RF1A.obj: ../HAL/RF1A.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/hunter/Documents/GitHub/cc430-embedded/example code/SLAA465 - CC430 RF Examples/CC430x513x RF examples/RF_Examples_CCS/Fixed_GT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__CC430F5137__ --define=MHZ_868 --diag_warning=225 --display_error_number --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/RF1A.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/RfRegSettings.obj: ../HAL/RfRegSettings.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/hunter/Documents/GitHub/cc430-embedded/example code/SLAA465 - CC430 RF Examples/CC430x513x RF examples/RF_Examples_CCS/Fixed_GT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__CC430F5137__ --define=MHZ_868 --diag_warning=225 --display_error_number --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/RfRegSettings.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_UCS.obj: ../HAL/hal_UCS.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/hunter/Documents/GitHub/cc430-embedded/example code/SLAA465 - CC430 RF Examples/CC430x513x RF examples/RF_Examples_CCS/Fixed_GT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__CC430F5137__ --define=MHZ_868 --diag_warning=225 --display_error_number --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/hal_UCS.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_pmm.obj: ../HAL/hal_pmm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/hunter/Documents/GitHub/cc430-embedded/example code/SLAA465 - CC430 RF Examples/CC430x513x RF examples/RF_Examples_CCS/Fixed_GT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__CC430F5137__ --define=MHZ_868 --diag_warning=225 --display_error_number --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/hal_pmm.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


