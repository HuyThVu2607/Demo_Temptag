################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS

GEN_OPTS__FLAG := @"./device.opt" 
GEN_CMDS__FLAG := -Wl,-l"./device_linker.cmd" 

ORDERED_OBJS += \
"./controller.o" \
"./ti_msp_dl_config.o" \
"./startup_mspm0c110x_ticlang.o" \
"./Driver/SEGGER_RTT.o" \
"./Driver/SEGGER_RTT_printf.o" \
"./Driver/acc_sc7a20.o" \
"./Driver/fm13dt160.o" \
"./Driver/hdc302x.o" \
"./Driver/mspm0_i2c_polling_safe.o" \
"./NFC_ISO/fm13_t4t.o" \
$(GEN_CMDS__FLAG) \
-Wl,-ldevice.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Driver/subdir_vars.mk
-include NFC_ISO/subdir_vars.mk
-include subdir_rules.mk
-include Driver/subdir_rules.mk
-include NFC_ISO/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
io_expander_i2c_host.out 

EXE_OUTPUTS__QUOTED += \
"io_expander_i2c_host.out" 

BIN_OUTPUTS += \
io_expander_i2c_host.hex 

BIN_OUTPUTS__QUOTED += \
"io_expander_i2c_host.hex" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "io_expander_i2c_host.out" secondary-outputs

# Tool invocations
io_expander_i2c_host.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -gdwarf-3 -Wl,-m"io_expander_i2c_host.map" -Wl,-i"C:/ti/mspm0_sdk_2_09_00_01/source" -Wl,-i"C:/Users/huyvu/workspace_ccstheia/io_expander_i2c_host" -Wl,-i"C:/Users/huyvu/workspace_ccstheia/io_expander_i2c_host/Debug/syscfg" -Wl,-i"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="io_expander_i2c_host_linkInfo.xml" -Wl,--rom_model -o "io_expander_i2c_host.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

io_expander_i2c_host.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmhex.exe" --diag_wrap=off --intel -o "io_expander_i2c_host.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "controller.o" "ti_msp_dl_config.o" "startup_mspm0c110x_ticlang.o" "Driver\SEGGER_RTT.o" "Driver\SEGGER_RTT_printf.o" "Driver\acc_sc7a20.o" "Driver\fm13dt160.o" "Driver\hdc302x.o" "Driver\mspm0_i2c_polling_safe.o" "NFC_ISO\fm13_t4t.o" 
	-$(RM) "controller.d" "ti_msp_dl_config.d" "startup_mspm0c110x_ticlang.d" "Driver\SEGGER_RTT.d" "Driver\SEGGER_RTT_printf.d" "Driver\acc_sc7a20.d" "Driver\fm13dt160.d" "Driver\hdc302x.d" "Driver\mspm0_i2c_polling_safe.d" "NFC_ISO\fm13_t4t.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

