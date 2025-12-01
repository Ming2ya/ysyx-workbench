BUILD_DIR = $(NPC_HOME)/build

PRJ = playground

test:
	mill -i $(PRJ).test

verilog:
	$(call git_commit, "generate verilog")
	mkdir -p $(BUILD_DIR)
	mill -i $(PRJ).runMain Elaborate --target-dir $(BUILD_DIR)

help:
	mill -i $(PRJ).runMain Elaborate --help

reformat:
	mill -i __.reformat

checkformat:
	mill -i __.checkFormat

bsp:
	mill -i mill.bsp.BSP/install

idea:
	mill -i mill.idea.GenIdea/idea

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: test verilog help reformat checkformat clean

ISA ?= riscv32e
GUEST_ISA = $(ISA)
NAME = $(GUEST_ISA)-npc
TOPNAME = Top

OBJ_DIR = $(BUILD_DIR)/obj-$(NAME)
BINARY = $(BUILD_DIR)/$(NAME)

CSRCS := $(shell find $(abspath ./csrc) -name '*.c' -or -name '*.cc')
VSRCS := $(BUILD_DIR)/$(TOPNAME).sv /home/mingya/ysyx-workbench/npc/playground/src/DPIC.sv
INC_PATH := $(NPC_HOME)/include $(VERILATOR_ROOT)/include $(OBJ_DIR) $(INC_PATH)
INC_PATH += $(NPC_HOME)/csrc/isa/riscv32e/include

CFLAGS += -Og -ggdb3
CFLAGS += $(addprefix -I, $(INC_PATH))
CFLAGS += -D__GUEST_ISA__=$(GUEST_ISA)
VFLAGS += --cc --MMD --build --exe --trace --top-module $(TOPNAME) -sv --LDFLAGS -Wl,--no-relax
	
include $(NPC_HOME)/scripts/sim.mk

-include ../Makefile
