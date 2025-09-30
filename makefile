################################################################################
# Makefile CI/CD pour STM32G031 Serre CO - Compile ELF et BIN uniquement
# Compatible Linux / GitHub Actions
################################################################################

# Nom du firmware
ARTIFACT := serre_co

# Répertoire de build (Debug par défaut)
BUILD_DIR ?= ./Debug

# Auto-détection récursive de tous les répertoires (Windows/Linux compatible)
# Trouve récursivement tous les dossiers sous certains répertoires racines
ROOT_SEARCH_DIRS := Core Drivers serre
SRC_DIRS := $(shell find $(ROOT_SEARCH_DIRS) -type d 2>/dev/null || echo "$(ROOT_SEARCH_DIRS)")

VERSION ?= "dev"

# Toolchain
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
OBJCOPY := arm-none-eabi-objcopy

# MCU & flags pour STM32G031
MCU_FLAGS := -mcpu=cortex-m0plus -mthumb

# Recherche automatique du linker script
LD_SCRIPT := $(firstword $(wildcard STM32G031*.ld *.ld))

# Auto-détection des répertoires d'includes (utilise les mêmes dossiers que les sources)
INCLUDES := $(foreach dir,$(SRC_DIRS),-I$(dir))

# Auto-détection de tous les fichiers sources dans les répertoires trouvés
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cc) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.s))

OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

# Création automatique de tous les dossiers de build nécessaires
$(shell mkdir -p $(foreach dir,$(SRC_DIRS),$(BUILD_DIR)/$(dir)))

# Définition du MCU
MCU_DEFINE := STM32G031xx

# Flags de compilation et linking (Debug par défaut)
CFLAGS ?= $(MCU_FLAGS) -D$(MCU_DEFINE) -DUSE_HAL_DRIVER -O0 -g3 -Wall -fdata-sections -ffunction-sections $(INCLUDES)
CXXFLAGS ?= $(CFLAGS) -std=c++11 -fno-exceptions -fno-rtti
LDFLAGS ?= $(MCU_FLAGS) -T$(LD_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(ARTIFACT).map,--gc-sections -lc -lm -lnosys --specs=nano.specs
CFLAGS += -DFW_VERSION=\"$(VERSION)\"
CXXFLAGS += -DFW_VERSION=\"$(VERSION)\"

# Vérifications
check-deps:
	@echo "Checking project structure..."
	@echo "Source directories found: $(SRC_DIRS)"
	@echo "Linker script: $(LD_SCRIPT)"
	@echo "MCU definition: $(MCU_DEFINE)"
	@echo "Sources found: $(words $(SRCS)) files"
	@$(if $(LD_SCRIPT),,echo "No linker script found!" && exit 1)
	@$(if $(SRCS),,echo "No source files found!" && exit 1)
	@echo "✅ Project structure OK"

# Règle par défaut
all: check-deps $(BUILD_DIR)/$(ARTIFACT).elf $(BUILD_DIR)/$(ARTIFACT).bin

# Mode debug : build dans ./build/debug
debug: check-deps
	@$(MAKE) BUILD_DIR=./build/debug all

# Mode release : optimisation -Os, build dans ./build/release
release: check-deps
	@$(MAKE) BUILD_DIR=./build/release CFLAGS='$(MCU_FLAGS) -D$(MCU_DEFINE) -DUSE_HAL_DRIVER -Os -DNDEBUG -Wall -fdata-sections -ffunction-sections $(INCLUDES) -DFW_VERSION=\"$(VERSION)\"' CXXFLAGS='$(MCU_FLAGS) -D$(MCU_DEFINE) -DUSE_HAL_DRIVER -Os -DNDEBUG -Wall -fdata-sections -ffunction-sections $(INCLUDES) -std=c++11 -fno-exceptions -fno-rtti -DFW_VERSION=\"$(VERSION)\"' LDFLAGS='$(MCU_FLAGS) -T$(LD_SCRIPT) -Wl,-Map=./build/release/$(ARTIFACT).map,--gc-sections -lc -lm -lnosys --specs=nano.specs' all

# Compilation des objets
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	@echo "Compiling C++ $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling C++ $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link ELF
$(BUILD_DIR)/$(ARTIFACT).elf: $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Génération BIN
$(BUILD_DIR)/$(ARTIFACT).bin: $(BUILD_DIR)/$(ARTIFACT).elf
	@echo "Generating binary $@"
	$(OBJCOPY) -O binary $< $@

# Génération HEX
$(BUILD_DIR)/$(ARTIFACT).hex: $(BUILD_DIR)/$(ARTIFACT).elf
	@echo "Generating hex $@"
	$(OBJCOPY) -O ihex $< $@

# Target avec HEX inclus
hex: $(BUILD_DIR)/$(ARTIFACT).elf $(BUILD_DIR)/$(ARTIFACT).bin $(BUILD_DIR)/$(ARTIFACT).hex

# Flash targets (compatible Windows/Linux)
flash-debug: debug
	@echo "Flashing debug build..."
	@st-flash write ./build/debug/$(ARTIFACT).bin 0x8000000 || echo "st-flash not found or failed. Please install stlink tools."

flash-release: release
	@echo "Flashing release build..."
	@st-flash write ./build/release/$(ARTIFACT).bin 0x8000000 || echo "st-flash not found or failed. Please install stlink tools."

flash: flash-debug

# Nettoyage
clean:
	@echo "Cleaning build directories..."
	rm -rf ./Debug ./Release ./build

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build debug version (default)"
	@echo "  debug        - Build debug version in ./build/debug"
	@echo "  release      - Build release version in ./build/release"
	@echo "  hex          - Build with hex file included"
	@echo "  check-deps   - Check project structure"
	@echo "  flash        - Flash debug version to MCU"
	@echo "  flash-debug  - Flash debug version to MCU"
	@echo "  flash-release- Flash release version to MCU"
	@echo "  clean        - Remove all build files"
	@echo "  help         - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  VERSION      - Set version string (default: dev)"
	@echo "  BUILD_DIR    - Override build directory"
	@echo ""
	@echo "Examples:"
	@echo "  make debug VERSION=v1.0.0"
	@echo "  make release VERSION=v1.0.0"

.PHONY: all debug release hex flash flash-debug flash-release clean help check-deps