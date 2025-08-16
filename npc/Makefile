CPPFLAGS += -I$(NVBOARD_HOME)/usr/include
LDFLAGS += -L$(NVBOARD_HOME)/build

include $(NVBOARD_HOME)/scripts/nvboard.mk
include ../Makefile
all:
	#@echo "Write this Makefile by your self."

TOPNAME = $(name)
NXDC_FILES = constr/$(TOPNAME).nxdc
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
$(shell mkdir -p $(BUILD_DIR))

CSRCS = $(abspath ./csrc/sim_$(TOPNAME).cpp)
VSRCS = $(abspath ./vsrc/$(TOPNAME).v)
VFLAGS = -cc -MMD --build --exe --trace-fst -Wall

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	#@echo "Write this Makefile by your self."
	verilator $(VFLAGS) $(VSRCS) $(CSRCS) \
		 --Mdir $(OBJ_DIR) -o $(abspath $(BIN))
	$(BIN)

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
board: $(BIN)
	$(BIN)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

$(BIN): $(VSRCS) $(CSRCS) $(SRC_AUTO_BIND) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	verilator $(VFLAGS) $^ \
		$(addprefix -CFLAGS , $(CPPFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))\
		--Mdir $(OBJ_DIR) -o $(abspath $(BIN))

clean:
	rm -rf build *.vdc *.fst
