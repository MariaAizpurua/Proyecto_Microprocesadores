COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = easylink_config.obj EasyLink_nortos.obj main_nortos.obj smartrf_settings.obj smartrf_settings_predefined.obj rfEasyLinkTx_nortos.obj CC1310_LAUNCHXL.obj CC1310_LAUNCHXL_fxns.obj ccfg.obj

NAME = rfEasyLinkTx

CFLAGS = -I.. \
    -DDeviceFamily_CC13X0 \
    -DCCFG_FORCE_VDDR_HH=0 \
    "-I$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source" \
    -DSUPPORT_PHY_CUSTOM \
    -DSUPPORT_PHY_50KBPS2GFSK \
    -DSUPPORT_PHY_625BPSLRM \
    -DSUPPORT_PHY_5KBPSSLLR \
    "-I$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/kernel/nortos" \
    "-I$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/kernel/nortos/posix" \
    --cpu=Cortex-M3 \
    --debug \
    --silent \
    -e \
    --aeabi \
    --thumb \
    --diag_suppress=Pa050

LFLAGS = "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/ti/display/lib/display.arm3" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/ti/grlib/lib/iar/m3/grlib.a" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/third_party/spiffs/lib/iar/m3/spiffs_cc26xx.a" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/ti/drivers/rf/lib/rf_multiMode_cc13x0.arm3" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/ti/drivers/lib/drivers_cc13x0.arm3" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/kernel/nortos/lib/nortos_cc13x0.arm3" \
    "$(COM_TI_SIMPLELINK_CC13X0_SDK_INSTALL_DIR)/source/ti/devices/cc13x0/driverlib/bin/iar/driverlib.lib" \
    --config ../iar/CC1310_LAUNCHXL_NoRTOS.icf \
    --map "$(NAME).map" \
    --silent \
    --cpu=Cortex-M3 \
    --semihosting=iar_breakpoint

all: $(NAME).out

easylink_config.obj: ../easylink/easylink_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

EasyLink_nortos.obj: ../easylink/EasyLink_nortos.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

main_nortos.obj: ../main_nortos.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

smartrf_settings.obj: ../smartrf_settings/smartrf_settings.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

smartrf_settings_predefined.obj: ../smartrf_settings/smartrf_settings_predefined.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

rfEasyLinkTx_nortos.obj: ../rfEasyLinkTx_nortos.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

CC1310_LAUNCHXL.obj: ../CC1310_LAUNCHXL.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

CC1310_LAUNCHXL_fxns.obj: ../CC1310_LAUNCHXL_fxns.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ccfg.obj: ../ccfg.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS) $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
