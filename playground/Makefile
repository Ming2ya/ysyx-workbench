VFLAGS = -cc --exe --build
CSRCS = ./csrc/*.c
VSRCS = ./vsrc/*.v
sim: VFLAGS += --trace-fst -Wall
all:
	#@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	#@echo "Write this Makefile by your self."
	verilator $(VFLAGS) $(CSRCS) $(VSRCS)
	./obj_dir/Vtop

include ../Makefile
