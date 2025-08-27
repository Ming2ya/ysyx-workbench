CPPFLAGS += -I$(NVBOARD_HOME)/usr/include
CPPFLAGS += -I$(VERILATOR_INC_DIR)
LDFLAGS += -L$(NVBOARD_HOME)/build

include $(NVBOARD_HOME)/scripts/nvboard.mk

TOPNAME = $(name)

NXDC_FILES = ./$(TOPNAME).nxdc
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
$(shell mkdir -p $(BUILD_DIR))
SIM_SRCS = $(abspath ./sim_$(TOPNAME).cpp)
BOARD_SRCS = $(abspath ./board_$(TOPNAME).cpp)
VSRCS = $(abspath ./$(TOPNAME).v)
VFLAGS = -cc -MMD --build --exe --top-module $(TOPNAME)

env_var:
env_var:
	@echo "TOPNAME = $(TOPNAME)"
	@echo "VSRCS = $(VSRCS)"
	@echo "BOARD_SRCS = $(BOARD_SRCS)"
	@echo "SIM_SRCS = $(SIM_SRCS)"
	@echo "NXDC_FILES = $(NXDC_FILES)"
	@echo "OBJ_DIR = $(OBJ_DIR)"
	@echo "BIN = $(BIN)"
	@echo "SRC_AUTO_BIND = $(SRC_AUTO_BIND)"

sim:
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

.PHONY: env_var clean board sim
