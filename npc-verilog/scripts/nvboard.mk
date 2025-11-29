CPPFLAGS += -I$(NVBOARD_HOME)/usr/include
LDFLAGS += -L$(NVBOARD_HOME)/build
NXDC_FILES = constr/$(TOPNAME).nxdc

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
board: $(BOARD_BIN)
	$(BOARD_BIN)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

$(BOARD_BIN): $(VSRCS) $(BOARD_SRCS) $(SRC_AUTO_BIND) $(NVBOARD_ARCHIVE)
	verilator $(VFLAGS) $^ \
		$(addprefix -CFLAGS , $(CPPFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))\
		--Mdir $(OBJ_DIR) -o $(abspath $(BOARD_BIN))