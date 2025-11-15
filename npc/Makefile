ISA ?= riscv32e
GUEST_ISA = $(ISA)
NAME = $(GUEST_ISA)-npc
TOPNAME = top

CSRCS = $(abspath ./csrc/*.c) $(abspath ./csrc/*.cpp)
VSRCS = $(abspath ./vsrc/*.v)

CFLAGS += -Og -ggdb3
VFLAGS += --cc --MMD --build --exe --trace --top-module $(TOPNAME)

BUILD_DIR = $(NPC_HOME)/build
OBJ_DIR = $(BUILD_DIR)/obj-$(NAME)
BINARY = $(BUILD_DIR)/$(NAME)

$(shell mkdir -p $(BUILD_DIR))

TARGET ?= sim

ifdef CONFIG_TARGET_AM
include $(AM_HOME)/Makefile
LINKAGE += $(ARCHIVES)
else
include $(NPC_HOME)/scripts/sim.mk
endif

include ../Makefile

clean:
	rm -rf build
