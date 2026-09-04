#include <common.h>
#include <elf.h>
#include <cpu/decode.h>

#ifdef CONFIG_IRINGTRACE
static char ringbuf[CONFIG_RINGTRACE_LENGTH][128];
static int ringhead = 0;

void ringtrace_add(char* log) {
    strcpy(ringbuf[ringhead], log);
    ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
}

void ringtrace_print() {
    Log("Instruction Ring Trace:");
    for (int i = 0; i < CONFIG_RINGTRACE_LENGTH; i ++) {
        if (ringbuf[ringhead][0] != '\0') puts(ringbuf[ringhead]);
        ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
    }
}
#endif

#ifdef CONFIG_MTRACE
vaddr_t read_addr, write_addr;
word_t read_data, write_data, reg_data;
const char* reg_name_buf;
bool read_flag = false, write_flag = false, reg_flag = false;

void mtrace_print(bool print_step){
    if (read_flag) {
        log_write("    read from " FMT_PADDR ": " FMT_WORD "\n", read_addr, read_data);
        if (print_step) printf("    read from " FMT_PADDR ": " FMT_WORD "\n", read_addr, read_data);
    }
    if (write_flag) {
        log_write("    write to " FMT_PADDR ": " FMT_WORD "\n", write_addr, write_data);
        if (print_step) printf("    write to " FMT_PADDR ": " FMT_WORD "\n", write_addr, write_data);
    }
    if (reg_flag) {
        log_write("    reg write %-3s: " FMT_WORD "\n", reg_name_buf, reg_data);
        if (print_step) printf("    reg write %-3s: " FMT_WORD "\n", reg_name_buf, reg_data);
    }
    read_flag = false;
    write_flag = false;
    reg_flag = false;
}

void mtrace_read(vaddr_t addr, int len, word_t data){
    read_addr = addr;
    read_data = data;
    read_flag = true;
}

void mtrace_write(vaddr_t addr, int len, word_t data){
    write_addr = addr;
    write_data = data;
    write_flag = true;
}

void mtrace_reg(const char* name, uint32_t data){
    reg_name_buf = name;
    reg_data = data;
    reg_flag = true;
}

#endif

#ifdef CONFIG_FTRACE
static FILE *elf_fp = NULL;
static Elf32_Sym *func_tab;
static int func_num = 0;
static char *func_name;

static void elf_parser(){
    Elf32_Ehdr *ehdr;
    ehdr = malloc(sizeof(Elf32_Ehdr));
    Assert(fread(ehdr, sizeof(Elf32_Ehdr), 1, elf_fp) == 1, "Read elf header failed!");
    Elf32_Shdr *shdr;
    shdr = malloc(sizeof(Elf32_Shdr) * ehdr->e_shnum);
    fseek(elf_fp, ehdr->e_shoff, SEEK_SET);
    Assert(fread(shdr, sizeof(Elf32_Shdr), ehdr->e_shnum, elf_fp) == ehdr->e_shnum, "Read elf section header failed!");
    uint16_t shstrndx = ehdr->e_shstrndx;
    char* shstrtab;
    shstrtab = malloc(shdr[shstrndx].sh_size);
    fseek(elf_fp, shdr[shstrndx].sh_offset, SEEK_SET);
    Assert(fread(shstrtab, sizeof(char), shdr[shstrndx].sh_size / sizeof(char), elf_fp) == shdr[shstrndx].sh_size / sizeof(char), "Read elf shstrlab failed!");

    Elf32_Sym *sym = NULL;
    int sym_len = 0;
    for (int i = 0; i < ehdr->e_shnum; i++){
        if (shdr[i].sh_type == SHT_SYMTAB && !strcmp(shstrtab + shdr[i].sh_name, ".symtab")){
            sym = malloc(shdr[i].sh_size);
            func_tab = malloc(shdr[i].sh_size);
            fseek(elf_fp, shdr[i].sh_offset, SEEK_SET);
            sym_len = shdr[i].sh_size / sizeof(Elf32_Sym);
            Assert(fread(sym, sizeof(Elf32_Sym), sym_len, elf_fp) == sym_len, "Read elf symtab failed!");
        }
        if (shdr[i].sh_type == SHT_STRTAB && !strcmp(shstrtab + shdr[i].sh_name, ".strtab")){
            func_name = malloc(shdr[i].sh_size);
            fseek(elf_fp, shdr[i].sh_offset, SEEK_SET);
            Assert(fread(func_name, sizeof(char), shdr[i].sh_size / sizeof(char), elf_fp) == shdr[i].sh_size / sizeof(char), "Read elf strtab failed!");
        }
    }
    Assert(sym, "Read elf symtab failed!");
    for (int i = 0; i < sym_len; i++){
        if ((sym[i].st_info & 0xf) == STT_FUNC){
            func_tab[func_num] = sym[i];
            func_num += 1;
        }
    }

    free(ehdr);
    free(shdr);
    free(shstrtab);
    free(sym);
}

void init_elf(const char *elf_file){
    if (elf_file == NULL){
        IFDEF(CONFIG_FTRACE, Assert(0, "No elf file given"));
    }
    else {
        FILE *fp = fopen(elf_file, "rb");
        Assert(fp, "can't open '%s'", elf_file);
        elf_fp = fp;
        elf_parser();
    }
}

static int f_depth = 0;
static int f_len = 0;
#define F_LEN 512
static char f_log[F_LEN][80];

static void ftrace_call(vaddr_t pc, vaddr_t npc){
    char *name;
    Assert(f_len < F_LEN, "ftrace log buf overflow!");
    for (int i = 0; i < func_num; i++){
        if (func_tab[i].st_value == npc){
            name = func_name + func_tab[i].st_name;
            snprintf(f_log[f_len], 80, "0x%08x: %*scall [%s@0x%08x]\n", pc, 2*f_depth, "", name, npc);
            f_depth += 1;
            f_len += 1;
            return;
        }
    }
    snprintf(f_log[f_len], 80, "0x%08x: %*scall [???@0x%08x]\n", pc,2*f_depth, "", npc);
    f_depth += 1;
    f_len += 1;
}

static void ftrace_ret(vaddr_t pc, vaddr_t npc){
    char *name;
    Assert(f_len < F_LEN, "ftrace log buf overflow!");
    for (int i = 0; i < func_num; i ++){
        if (func_tab[i].st_value <= pc && func_tab[i].st_value + func_tab[i].st_size >= pc){
            name = func_name + func_tab[i].st_name;
            f_depth -= 1;
            snprintf(f_log[f_len], 80, "0x%08x: %*sret  [%s]\n", pc,2*f_depth, "", name);
            f_len += 1;
            return;
        }
    }
    f_depth -= 1;
    snprintf(f_log[f_len], 80, "0x%08x: %*sret  [???]\n", pc, 2*f_depth, "");
    f_len += 1;
}

void ftrace_main(Decode *s, vaddr_t npc){
    if (s->snpc == npc)
        return;
    uint32_t i = s->isa.inst;
    int rs1 = BITS(i, 19, 15);
    int rd  = BITS(i, 11, 7);
    int op  = BITS(i, 6, 0);
    if (op == 0x6f){        //jal
        if (rd == 1){
            ftrace_call(s->pc, npc);
        }
    }
    else if (op == 0x67){   //jalr
        if (rd == 1){
            ftrace_call(s->pc, npc);
        }
        else if (rd == 0 && rs1 == 1){
            ftrace_ret(s->pc, npc);
        }
    }
}

void ftrace_print(){
    free(func_name);
    free(func_tab);
    Log("Function Trace:");
    for (int i = 0; i < f_len; i ++){
        printf("%s", f_log[i]);
        log_write("%s", f_log[i]);
    }
}
#endif

#ifdef CONFIG_DTRACE
void dtrace_read(vaddr_t addr, int len, word_t data, char *name){
    Log("Read from %s at " FMT_PADDR ": " FMT_WORD, name, addr, data);
}

void dtrace_write(vaddr_t addr, int len, word_t data, char *name){
    Log("Write to %s at " FMT_PADDR ": " FMT_WORD, name, addr, data);
}

#endif