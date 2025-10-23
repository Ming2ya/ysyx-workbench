#include <common.h>
#include <elf.h>
#include <cpu/decode.h>

static char ringbuf[CONFIG_RINGTRACE_LENGTH][128];
static int ringhead = 0;

void ringtrace_add(char* log) {
    strcpy(ringbuf[ringhead], log);
    ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
}

void ringtrace_print() {
    Log("Instruction Ring Trace:");
    for (int i = 0; i < CONFIG_RINGTRACE_LENGTH; i ++) {
        puts(ringbuf[ringhead]);
        ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
    }
}

static FILE *elf_fp = NULL;
static Elf32_Sym func_tab[100];
static int func_num = 0;
static char func_name[1024];

void elf_parser(){
    Elf32_Ehdr *ehdr;
    ehdr = malloc(sizeof(Elf32_Ehdr));
    fread(ehdr, sizeof(Elf32_Ehdr), 1, elf_fp);
    Elf32_Shdr *shdr;
    shdr = malloc(sizeof(Elf32_Shdr) * ehdr->e_shnum);
    fseek(elf_fp, ehdr->e_shoff, SEEK_SET);
    fread(shdr, sizeof(Elf32_Shdr), ehdr->e_shnum, elf_fp);
    uint16_t shstrndx = ehdr->e_shstrndx;
    char* shstrtab;
    shstrtab = malloc(shdr[shstrndx].sh_size);
    fseek(elf_fp, shdr[shstrndx].sh_offset, SEEK_SET);
    fread(shstrtab, sizeof(char), shdr[shstrndx].sh_size / sizeof(char), elf_fp);

    Elf32_Sym *sym;
    int sym_len;
    for (int i = 0; i < ehdr->e_shnum; i++){
        if (shdr[i].sh_type == SHT_SYMTAB && !strcmp(shstrtab[shdr[i].sh_name], ".symtab")){
            sym = malloc(shdr[i].sh_size);
            fseek(elf_fp, shdr[i].sh_offset, SEEK_SET);
            sym_len = shdr[i].sh_size / sizeof(Elf32_Sym);
            fread(sym, sizeof(Elf32_Sym), sym_len, elf_fp);
        }
        if (shdr[i].sh_type == SHT_STRTAB && !strcmp(shstrtab[shdr[i].sh_name], ".strtab")){
            fseek(elf_fp, shdr[i].sh_offset, SEEK_SET);
            fread(func_name, sizeof(char), shdr[i].sh_size / sizeof(char), elf_fp);
        }
    }
    for (int i = 0; i < sym_len; i++){
        if ((sym[i].st_info & 0xf) == STT_FUNC){
            func_tab[func_num] = sym[i];
            func_num += 1;
        }
    }

    free(ehdr);
    free(shdr);
    free(sym);
}

void elf_init(const char *elf_file){
    if (elf_file == NULL){
        IFDEF(CONFIG_FTRACE, Assert(0, "No elf file given"));
    }
    else {
        FILE *fp = fopen(elf_file, "rb");
        Assert(fp, "can't open '%s'", elf_file);
        elf_fp = fp;
        elf_praser();
    }
}

void ftrace_main(Decode *s, vaddr_t npc){
    
}

void ftrace_print(){
    Log("Function Trace:");

}