PROJECT_NAME     := Blackbird
TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := _build

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

SDK_ROOT := e:\Nordic\nRF5_SDK_13.1.0_7ca7556
FREE_RTOS := $(SDK_ROOT)/external/freertos
PROJ_DIR := .

TEMPLATE_PATH = $(SDK_ROOT)/components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

# Override toolchain commands
CC      := arm-none-eabi-gcc
CXX     := arm-none-eabi-c++
AS      := arm-none-eabi-as
AR      := arm-none-eabi-ar -r
LD      := arm-none-eabi-ld
NM      := arm-none-eabi-nm
OBJDUMP := arm-none-eabi-objdump
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/common/ble_conn_state.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/common/ble_advdata.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/common/ble_conn_params.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/common/ble_srv_common.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/peer_manager.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/peer_id.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/pm_mutex.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/id_manager.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/security_manager.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/peer_manager/peer_database.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/ble/ble_services/ble_dis/ble_dis.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/boards/boards.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/bsp/bsp.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/timer/app_timer_freertos.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/fds/fds.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/util/app_error.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/util/app_util_platform.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/util/sdk_mapped_flags.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/libraries/fstorage/fstorage.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/toolchain/system_nrf52.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c)
C_SOURCE_FILES += $(abspath $(SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/croutine.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/event_groups.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/list.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/portable/GCC/nrf52/port.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/portable/CMSIS/nrf52/port_cmsis.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/portable/CMSIS/nrf52/port_cmsis_systick.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/queue.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/tasks.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/timers.c)
C_SOURCE_FILES += $(abspath $(FREE_RTOS)/source/portable/MemMang/heap_4.c)
C_SOURCE_FILES += $(abspath $(PROJ_DIR)/main.c)
C_SOURCE_FILES += $(abspath $(PROJ_DIR)/ble.c)
C_SOURCE_FILES += $(abspath $(PROJ_DIR)/services/obd_service.c)
C_SOURCE_FILES += $(abspath $(PROJ_DIR)/drivers/kline_drv.c)

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath $(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52.S)

#assembly files common to all targets
LIBS  = 

#includes common to all targets
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/boards)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/ble/common)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/ble/nrf_ble_gatt)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/ble/ble_advertising)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/ble/ble_services/ble_dis)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/bsp)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/button)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/util)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/fds)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/log)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/log/src)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/strerror)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/fstorage)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/timer)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/experimental_section_vars)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/device)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/common)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/uart)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/hal)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/clock)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/toolchain)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/toolchain/cmsis/include)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/softdevice/s132/headers)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/softdevice/s132/headers/nrf52)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/softdevice/common/softdevice_handler)
INC_PATHS += -I$(abspath $(FREE_RTOS)/portable/GCC/nrf52)
INC_PATHS += -I$(abspath $(FREE_RTOS)/source/include)
INC_PATHS += -I$(abspath $(FREE_RTOS)/portable/CMSIS/nrf52)
INC_PATHS += -I$(abspath $(PROJ_DIR))
INC_PATHS += -I$(abspath $(PROJ_DIR)/include)
INC_PATHS += -I$(abspath $(PROJ_DIR)/config)
INC_PATHS += -I$(abspath $(PROJ_DIR)/tasks)
INC_PATHS += -I$(abspath $(PROJ_DIR)/drivers)
INC_PATHS += -I$(abspath $(PROJ_DIR)/services)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

HEAP_SIZE=0
STACK_SIZE=2048

#flags common to all targets
CFLAGS += -D__HEAP_SIZE=$(HEAP_SIZE)
CFLAGS += -D__STACK_SIZE=$(STACK_SIZE)
#CFLAGS += -DUSE_APP_CONFIG
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DUART_TX_BUF_SIZE=64
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DS132
CFLAGS += -DNRF52
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DENABLE_DEBUG_LOG_SUPPORT
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DNRF_SD_BLE_API_VERSION=4
CFLAGS += -DNRF_TLS_MAX_INSTANCE_COUNT=1
CFLAGS += -DBOARD_PCA10040
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -w -O0 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DUART_TX_BUF_SIZE=64
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -Ds1xx
ASMFLAGS += -DNRF52
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DENABLE_DEBUG_LOG_SUPPORT
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DNRF_TLS_MAX_INSTANCE_COUNT=1
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -D__HEAP_SIZE=$(HEAP_SIZE)
ASMFLAGS += -D__STACK_SIZE=$(STACK_SIZE)

#default target - first one defined
default: all nrf52832_xxaa_s1xx_iot

#building all targets
all: 
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf52832_xxaa

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.S=.o) )

vpath %.c $(C_PATHS)
vpath %.S $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf52832_xxaa: OUTPUT_FILENAME := nrf52832_xxaa
nrf52832_xxaa: LINKER_SCRIPT=$(PROJ_DIR)/config/armgcc_s132_nrf52832_xxaa.ld

nrf52832_xxaa: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@
    
# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
	
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
	
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
	
flash: all $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/nrf52832_xxaa.hex
	nrfjprog --program ./$(OUTPUT_BINARY_DIRECTORY)/nrf52832_xxaa.hex -f nrf52 --sectorerase	
	nrfjprog --reset -f nrf52

flash_softdevice:
	@echo Flashing: s1xx-iot-prototype3_nrf52_softdevice.hex
	nrfjprog --program $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_4.0.2_softdevice.hex -f nrf52 --chiperase
	nrfjprog --reset -f nrf52
