#include <common.h>

void cpu_exec(uint64_t n);
void init_monitor(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    init_monitor(argc, argv);
    cpu_exec(10);
    printf("Hello World");
    return 0;
}