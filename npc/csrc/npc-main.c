#include "common.h"

void exec_once();
void cpu_init();
void init_monitor(int argc, char *argv[]);
void close_sim();

int main(int argc, char *argv[]) {
    init_monitor(argc, argv);

    int i = 0;
    while (i < 300){
        i ++;
        exec_once();
    }

    close_sim();
    return 0;
}
