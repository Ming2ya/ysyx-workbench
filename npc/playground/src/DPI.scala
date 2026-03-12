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

// class DecResult extends BlackBox {
//     val io = IO(new Bundle {
//         val valid = Input(UInt(1.W))
//         val ebreak = Input(UInt(1.W))
//         val pc = Input(UInt(32.W))
//     })
// }

class MEM_DPI extends BlackBox {
    val io = IO(new Bundle {
        val valid = Input(UInt(1.W))
        val raddr = Input(UInt(32.W))
        val wen   = Input(UInt(1.W))
        val waddr = Input(UInt(32.W))
        val wdata = Input(UInt(32.W))
        val wmask = Input(UInt(8.W))
        val rdata = Output(UInt(32.W))
    })
}