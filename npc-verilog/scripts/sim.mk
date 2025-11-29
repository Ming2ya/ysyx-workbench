IMG ?=
NPC_EXEC = $(BINARY) $(IMG)

$(BINARY): $(VSRCS) $(CSRCS)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator $(VFLAGS) $(addprefix -CFLAGS , $(CFLAGS)) $^ \
		 --Mdir $(OBJ_DIR) -o $@

sim: $(BINARY)
	$(NPC_EXEC)

gdb: $(BINARY)
	gdb -s $(BINARY) -args $(NPC_EXEC)