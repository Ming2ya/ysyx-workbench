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

CSRCS := $(shell find csrc -name '*.c')
CXXSRCS := $(shell find $(abspath csrc) -name '*.cc')
VSRCS := $(BUILD_DIR)/$(TOPNAME).sv /home/mingya/ysyx-workbench/npc/playground/src/DPIC.sv
LIBS += -lreadline
INC_PATH += $(VERILATOR_ROOT)/include $(VERILATOR_ROOT)/include/vltstd $(OBJ_DIR)
INC_PATH += $(NPC_HOME)/include $(NPC_HOME)/csrc/isa/riscv32e/include

CFLAGS += -Og -ggdb3
CFLAGS += $(addprefix -I, $(INC_PATH))
CFLAGS += -D__GUEST_ISA__=$(GUEST_ISA)
LDFLAGS += -Wl,--no-relax
LDFLAGS += $(LIBS)
VFLAGS += --cc --MMD --build --exe --trace --top-module $(TOPNAME) -sv
	
include $(NPC_HOME)/scripts/sim.mk

-include ../Makefile
