#include "common.h"

void cpu_exec(uint64_t n);
void init_monitor(int argc, char *argv[]);
void close_sim();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
    init_monitor(argc, argv);

    cpu_exec(-1);

    close_sim();
    return is_exit_status_bad();
}
