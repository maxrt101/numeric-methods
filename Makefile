# lab5

export TOPDIR    := $(shell pwd)
export BUILD_DIR := $(TOPDIR)/build

export CXX      := g++
export CXXFLAGS := -std=c++17 -I $(TOPDIR)
export TARGET   := lab5

ifeq ($(DEBUG),1)
CXXFLAGS += -g3 -D_DEBUG
endif

.PHONY: build

build: prepare
	$(info [+] Build $(TARGET))
	$(CXX) $(CXXFLAGS) main.cc -o $(BUILD_DIR)/bin/$(TARGET)

prepare:
	$(info [+] Prepare folders)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/bin

$(V).SILENT:
