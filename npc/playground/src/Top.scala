package npc

import chisel3._
import npc.core._
import npc.dpi._

class Top extends Module{
    val io = IO(new Bundle {
        val pc      = Output(UInt(32.W))
        val inst    = Output(UInt(32.W))
        val ebreak  = Output(UInt(1.W))
        val valid   = Output(UInt(1.W))
    })

    val ifu = Module(new IFU)
    val idu = Module(new IDU)
    val exu = Module(new EXU)
    val rf  = Module(new RegisterFile)
    val mem = Module(new MEM)

    // IFU connections
    ifu.io.jump := idu.io.jump
    ifu.io.branch := idu.io.branch
    ifu.io.jumpAddr := idu.io.jumpAddr
    
    // IDU connections
    idu.io.inst    := ifu.io.inst
    idu.io.pc      := ifu.io.pc
    idu.io.rs1Data := rf.io.rs1Data
    idu.io.rs2Data := rf.io.rs2Data

    // EXU connections
    exu.io.imm := idu.io.imm
    exu.io.pc := ifu.io.pc
    exu.io.aluSrcA := idu.io.aluSrcA
    exu.io.aluSrcB := idu.io.aluSrcB
    exu.io.aluCode := idu.io.aluCode

    // MEM connections
    mem.io.valid := idu.io.memToReg
    mem.io.raddr := exu.io.aluResult
    mem.io.wen   := idu.io.writeMem
    mem.io.waddr := exu.io.aluResult
    mem.io.wdata := rf.io.rs2Data
    mem.io.memType := idu.io.memType

    // RF  connections
    rf.io.rs1Addr := idu.io.rs1Addr
    rf.io.rs2Addr := idu.io.rs2Addr
    rf.io.rdAddr := idu.io.rdAddr
    rf.io.regWrite := idu.io.regWrite
    rf.io.rdData := Mux(idu.io.memToReg === 1.U, mem.io.rdata, exu.io.aluResult)
    
    // Output signal
    io.pc := ifu.io.pc
    io.inst := ifu.io.inst
    io.ebreak := idu.io.ebreak
    io.valid := idu.io.valid
}

class RegisterFile extends Module {
    val io = IO(new Bundle {
        val rs1Addr = Input(UInt(5.W))
        val rs2Addr = Input(UInt(5.W))
        val rdAddr  = Input(UInt(5.W))
        val regWrite= Input(UInt(1.W))
        val rdData  = Input(UInt(32.W))
        val rs1Data = Output(UInt(32.W))
        val rs2Data = Output(UInt(32.W))
    })

    val regFile = RegInit(VecInit(Seq.fill(32)(0.U(32.W))))

    when(io.regWrite===1.U && io.rdAddr =/= 0.U) {
        regFile(io.rdAddr) := io.rdData
    }

    io.rs1Data := Mux(io.rs1Addr === 0.U, 0.U, regFile(io.rs1Addr))
    io.rs2Data := Mux(io.rs2Addr === 0.U, 0.U, regFile(io.rs2Addr))

    val updataReg = Module(new UpdateReg)
    updataReg.io.clk := clock
    updataReg.io.reset := reset
    updataReg.io.rdAddr := io.rdAddr
    updataReg.io.rdData := io.rdData
    updataReg.io.regWrite := io.regWrite
}