include $(NPC_HOME)/tools/difftest.mk

override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt
override ARGS += $(ARGS_DIFF)

IMG ?=
NPC_EXEC = $(BINARY) $(ARGS) $(IMG)

INCLUDES = $(addprefix -I, $(INC_PATH))
CFLAGS  := -O2 -MMD  $(INCLUDES) $(CFLAGS) #-Wall -Werror
LDFLAGS := -O2 $(LDFLAGS)

OBJS = $(CSRCS:%.c=$(OBJ_DIR)/%.o)

# Compilation patterns
$(OBJ_DIR)/%.o: %.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)

$(BUILD_DIR)/$(TOPNAME).sv: $(SCALASRCS)
	$(call git_commit, "generate verilog")
	mkdir -p $(BUILD_DIR)
	mill -i $(PRJ).runMain Elaborate --target-dir $(BUILD_DIR)

$(BINARY): $(VSRCS) $(CXXSRCS) $(OBJS)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator --Mdir $(OBJ_DIR) -o $@ $^ $(VFLAGS) \
			$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))

sim: $(BINARY)
	$(NPC_EXEC)

gdb: $(BINARY)
	gdb -s $(BINARY) -args $(NPC_EXEC)