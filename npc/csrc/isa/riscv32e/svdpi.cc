#include "svdpi.h"
#include <isa.h>
#include "local-include/reg.h"

extern "C"{
void update_reg(int addr, const svLogicVecVal* data){
    uint32_t data_val = data[0].aval;
    gpr(addr) = data_val;
}}