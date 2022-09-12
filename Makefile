# lab1

export TOPDIR    := $(shell pwd)
export BUILD_DIR := $(TOPDIR)/build

export CXX      := g++
export CXXFLAGS := -std=c++17 -I $(TOPDIR)
export TARGET   := lab4

ifeq ($(DEBUG),1)
CXXFLAGS += -g3 -D_DEBUG
endif

.PHONY: build

build: prepare
	$(info [+] Build $(TARGET))
	$(CXX) $(CXXFLAGS) main.cc -o $(BUILD_DIR)/bin/$(TARGET)
	for file in $(TOPDIR)/tests/*; do \
		echo "[+] Build $$(basename $${file%.*}).so"; \
		$(CXX) -c $(CXXFLAGS) $$file -o "$(BUILD_DIR)/obj/$$(basename $${file%.*}).o"; \
		$(CXX) -shared -o "$(BUILD_DIR)/tests/$$(basename $${file%.*}).so" "$(BUILD_DIR)/obj/$$(basename $${file%.*}).o"; \
	done

prepare:
	$(info [+] Prepare folders)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/obj
	mkdir -p $(BUILD_DIR)/tests

$(V).SILENT:
