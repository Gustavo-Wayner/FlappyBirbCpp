# ===========================
# Project
# ===========================
TARGET_NAME := game

SRC_DIR   := src
BUILD_DIR := build

ASSETS_SRC := assets
ASSETS_DST := $(BUILD_DIR)/assets

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CXX ?= g++

# Raylib
RAYLIB_DIR     := third_party/raylib
RAYLIB_INC     := $(RAYLIB_DIR)/src
RAYLIB_EXT     := $(RAYLIB_DIR)/src/external
RAYLIB_LIB_DIR := $(RAYLIB_DIR)/src

# ===========================
# Compiler flags
# ===========================
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 \
	-I$(RAYLIB_INC) -I$(RAYLIB_EXT) -Ithird_party\
	-Wno-missing-field-initializers -Wno-unused-parameter

# ===========================
# Platform / commands
# ===========================
ifeq ($(OS),Windows_NT)
    ifneq ($(MSYSTEM),)
        PLATFORM := WINDOWS_MSYS
        MKDIR  = mkdir -p "$(dir $@)"
        RMDIR  = rm -rf
    else
        PLATFORM := WINDOWS
        SHELL = pwsh.exe -NoProfile -Command
        MKDIR  = if not exist "$(dir $@)" mkdir "$(dir $@)"
        RMDIR  = rmdir /S /Q
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM := LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := MACOS
    endif
    MKDIR  = mkdir -p "$(dir $@)"
    RMDIR  = rm -rf
endif

EXE_EXT :=
ifeq ($(PLATFORM),WINDOWS)
    EXE_EXT := .exe
endif
ifeq ($(PLATFORM),WINDOWS_MSYS)
    EXE_EXT := .exe
endif

TARGET := $(BUILD_DIR)/$(TARGET_NAME)$(EXE_EXT)

# ===========================
# Linking
# ===========================
LDFLAGS := -L$(RAYLIB_LIB_DIR)

ifeq ($(PLATFORM),WINDOWS)
    LDLIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
    SUBSYSTEM := -Wl,-subsystem,windows
endif
ifeq ($(PLATFORM),WINDOWS_MSYS)
    LDLIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
    SUBSYSTEM := -Wl,-subsystem,windows
endif

ifeq ($(PLATFORM),LINUX)
    LDLIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(PLATFORM),MACOS)
    LDLIBS := -lraylib \
        -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

# ===========================
# Rules
# ===========================
.PHONY: all run clean assets

all: assets $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS) $(SUBSYSTEM)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
ifeq ($(PLATFORM),WINDOWS_MSYS)
	@cmd //c start "" /D "$(BUILD_DIR)" "$(TARGET_NAME)$(EXE_EXT)"
else ifeq ($(PLATFORM),WINDOWS)
	@cd "$(BUILD_DIR)" && $(TARGET_NAME)$(EXE_EXT)
else
	@cd "$(BUILD_DIR)" && ./$(TARGET_NAME)$(EXE_EXT)
endif

clean:
	-@$(RMDIR) "$(BUILD_DIR)"

# ===========================
# Assets
# ===========================
ifeq ($(PLATFORM),WINDOWS)
assets:
	-@if exist "$(ASSETS_DST)" rmdir /S /Q "$(ASSETS_DST)"
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(ASSETS_DST)" mkdir "$(ASSETS_DST)"
	@xcopy /E /I /Y "$(ASSETS_SRC)\*" "$(ASSETS_DST)\"
else
assets:
	-@$(RMDIR) "$(ASSETS_DST)"
	@mkdir -p "$(ASSETS_DST)"
	@cp -r "$(ASSETS_SRC)/." "$(ASSETS_DST)/"
endif
