CPPFLAGS += -I$(NVBOARD_HOME)/usr/include
LDFLAGS += -L$(NVBOARD_HOME)/build

include $(NVBOARD_HOME)/scripts/nvboard.mk
include ../Makefile
all:
	#@echo "Write this Makefile by your self."

name = top
TOPNAME = $(name)
NXDC_FILES = constr/$(TOPNAME).nxdc
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
$(shell mkdir -p $(BUILD_DIR))

SIM_SRCS = $(abspath ./csrc/*.cpp)
BOARD_SRCS = $(abspath ./csrc/board_$(TOPNAME).cpp)
VSRCS = $(abspath ./vsrc/*.v)
VFLAGS = --cc --MMD --build --exe --top-module $(TOPNAME)

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	#@echo "Write this Makefile by your self."
	verilator $(VFLAGS) $(VSRCS) $(SIM_SRCS) --trace-fst\
		 --Mdir $(OBJ_DIR) -o $(abspath $(BIN))
	$(BIN)

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
board: $(BIN)
	$(BIN)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

$(BIN): $(VSRCS) $(BOARD_SRCS) $(SRC_AUTO_BIND) $(NVBOARD_ARCHIVE)
	verilator $(VFLAGS) $^ \
		$(addprefix -CFLAGS , $(CPPFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))\
		--Mdir $(OBJ_DIR) -o $(abspath $(BIN))

clean:
	rm -rf build *.fst
