package npc.dpi

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

class InstFetch extends BlackBox {
    val io = IO(new Bundle {
        val reset = Input(Bool())
        val pc = Input(UInt(32.W))
        val inst = Output(UInt(32.W))
    })
}

class DecResult extends BlackBox {
    val io = IO(new Bundle {
        val valid = Input(UInt(1.W))
        val ebreak = Input(UInt(1.W))
        val pc = Input(UInt(32.W))
    })
}

class MEM extends BlackBox {
    val io = IO(new Bundle {
        val clk = Input(Clock())
        val MemtoReg = Input(UInt(1.W))
        val raddr = Input(UInt(32.W))
        val writeMem = Input(UInt(3.W))
        val waddr = Input(UInt(32.W))
    })
}