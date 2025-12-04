IMG ?=
NPC_EXEC = $(BINARY) $(IMG)

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

$(VSRCS): verilog

$(BINARY): $(VSRCS) $(CXXSRCS) $(OBJS)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator --Mdir $(OBJ_DIR) -o $@ $^ $(VFLAGS) \
			$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))

sim: $(BINARY)
	$(NPC_EXEC)

gdb: $(BINARY)
	gdb -s $(BINARY) -args $(NPC_EXEC)