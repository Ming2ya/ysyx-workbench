CPPFLAGS += -I$(NVBOARD_HOME)/usr/include
LDFLAGS += -L$(NVBOARD_HOME)/build

DEBUG_FLAGS = -Og -ggdb3

include $(NVBOARD_HOME)/scripts/nvboard.mk
include ../Makefile
all:
	#@echo "Write this Makefile by your self."

name = top
TOPNAME = $(name)
NXDC_FILES = constr/$(TOPNAME).nxdc
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
SIM_BIN = $(BUILD_DIR)/sim_$(TOPNAME)
BOADR_BIN = $(BUILD_DIR)/board_$(TOPNAME)
$(shell mkdir -p $(BUILD_DIR))

SIM_SRCS = $(abspath ./csrc/*.cpp)
BOARD_SRCS = $(abspath ./csrc/board_$(TOPNAME).cpp)
VSRCS = $(abspath ./vsrc/*.v)
VFLAGS = --cc --MMD --build --exe --trace --top-module $(TOPNAME)

VFLAGS_DEBUG = -CFLAGS "-Og -ggdb3"

$(SIM_BIN): $(VSRCS) $(SIM_SRCS)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	#@echo "Write this Makefile by your self."
	verilator $(VFLAGS) $(VFLAGS_DEBUG) $(VSRCS) $(SIM_SRCS) \
		 --Mdir $(OBJ_DIR) -o $(abspath $(SIM_BIN))

sim: $(SIM_BIN)
	$(SIM_BIN)

gdb: $(SIM_BIN)
	gdb $(SIM_BIN)

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
board: $(BOARD_BIN)
	$(BOARD_BIN)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

$(BOARD_BIN): $(VSRCS) $(BOARD_SRCS) $(SRC_AUTO_BIND) $(NVBOARD_ARCHIVE)
	verilator $(VFLAGS) $^ \
		$(addprefix -CFLAGS , $(CPPFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))\
		--Mdir $(OBJ_DIR) -o $(abspath $(BOARD_BIN))

clean:
	rm -rf build *.vcd
