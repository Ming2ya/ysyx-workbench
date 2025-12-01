package npc.dpic

import chisel3._

class UpdateReg extends BlackBox {
    val io = IO(new Bundle{
        val clk = Input(Clock())
        val reset = Input(Bool())
        val rdAddr = Input(UInt(5.W))
        val rdData = Input(UInt(32.W))
        val regWrite = Input(UInt(1.W))
    })
}