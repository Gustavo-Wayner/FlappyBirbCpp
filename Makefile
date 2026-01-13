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

# Raylib (inside the repo)
RAYLIB_INC     := raylib/src
RAYLIB_EXT     := raylib/src/external
RAYLIB_LIB_DIR := raylib/src

CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -I$(RAYLIB_INC) -I$(RAYLIB_EXT) -Wno-missing-field-initializers -Wno-unused-parameter

# ===========================
# Platform / commands
# ===========================
ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
    SHELL := cmd.exe
    MKDIR  = if not exist "$(dir $@)" mkdir "$(dir $@)"
    RMDIR  = rmdir /S /Q
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

TARGET := $(BUILD_DIR)/$(TARGET_NAME)$(EXE_EXT)

ifeq ($(PLATFORM),WINDOWS)
    ASSETS_CMD = \
        -@$(RMDIR) "$(ASSETS_DST)" && \
        if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)" && \
        if not exist "$(ASSETS_DST)" mkdir "$(ASSETS_DST)" && \
        xcopy /E /I /Y "$(ASSETS_SRC)\*" "$(ASSETS_DST)\"
else
    ASSETS_CMD = \
        -@$(RMDIR) "$(ASSETS_DST)" && \
        mkdir -p "$(ASSETS_DST)" && \
        cp -r "$(ASSETS_SRC)/." "$(ASSETS_DST)/"
endif

# ===========================
# Linking
# ===========================
LDFLAGS := -L$(RAYLIB_LIB_DIR)

ifeq ($(PLATFORM),WINDOWS)
    LDLIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
    SUBSYSTEM=-Wl,-subsystem,windows
endif

ifeq ($(PLATFORM),LINUX)
    LDLIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(PLATFORM),MACOS)
    LDLIBS := -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

# ===========================
# Rules
# ===========================
.PHONY: all run clean assets


all: assets $(TARGET)

# Links in build/game(.exe)
$(TARGET): $(OBJS)
	@$(MKDIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS) $(SUBSYSTEM)

# Compiles any .cpp from src/ to build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: assets $(TARGET)
	@cd "$(BUILD_DIR)" && "$(TARGET_NAME)$(EXE_EXT)"

clean:
	-@$(RMDIR) "$(BUILD_DIR)"


ifeq ($(PLATFORM),WINDOWS)
assets:
	-@$(RMDIR) "$(ASSETS_DST)"
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(ASSETS_DST)" mkdir "$(ASSETS_DST)"
	@xcopy /E /I /Y "$(ASSETS_SRC)\*" "$(ASSETS_DST)\"
else
assets:
	-@$(RMDIR) "$(ASSETS_DST)"
	@mkdir -p "$(ASSETS_DST)"
	@cp -r "$(ASSETS_SRC)/." "$(ASSETS_DST)/"
endif