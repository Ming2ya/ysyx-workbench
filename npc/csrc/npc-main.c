#include "common.h"

void exec_once();
void cpu_init();
void init_monitor(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    init_monitor(argc, argv);

    int i = 0;
    while (i < 30){
        i ++;
        exec_once();
    }

    return 0;
}
