package npc

import chisel3._
import npc.core._
import npc.dpi._

class Top extends Module{
    val io = IO(new Bundle {
        val memDout = Input(UInt(32.W))
        val memWrite= Output(UInt(3.W))
        val memAddr = Output(UInt(32.W))
        val memData = Output(UInt(32.W))
        val pc      = Output(UInt(32.W))
    })

    val ifu = Module(new IFU)
    val idu = Module(new IDU)
    val exu = Module(new EXU)
    val rf  = Module(new RegisterFile)

    // IFU connections
    io.pc := ifu.io.pc
    ifu.io.jump := idu.io.jump
    ifu.io.branch := idu.io.branch
    ifu.io.jumpAddr := idu.io.jumpAddr
    
    // IDU connections
    idu.io.inst    := ifu.io.inst
    idu.io.pc      := ifu.io.pc
    idu.io.rs1Data := rf.io.rs1Data
    idu.io.rs2Data := rf.io.rs2Data
    rf.io.rs1Addr := idu.io.rs1Addr
    rf.io.rs2Addr := idu.io.rs2Addr
    exu.io.imm := idu.io.imm
    exu.io.pc := ifu.io.pc
    exu.io.aluSrcA := idu.io.aluSrcA
    exu.io.aluSrcB := idu.io.aluSrcB
    exu.io.aluCode := idu.io.aluCode
    
    // Write back connections
    rf.io.rdAddr := idu.io.rdAddr
    rf.io.regWrite := idu.io.regWrite
    rf.io.rdData := Mux(idu.io.memToReg === 1.U, io.memDout, exu.io.aluResult)
    
    // Output signal
    io.memWrite := idu.io.writeMem
    io.memAddr := exu.io.aluResult
    io.memData := rf.io.rs2Data
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

    val regFile = Reg(Vec(32, UInt(32.W)))

    when(io.regWrite===1.U && io.rdAddr =/= 0.U) {
        regFile(io.rdAddr) := io.rdData
    }

    io.rs1Data := Mux(io.rs1Addr === 0.U, 0.U, regFile(io.rs1Addr))
    io.rs2Data := Mux(io.rs2Addr === 0.U, 0.U, regFile(io.rs2Addr))

    val updataReg = Module(new UpdateReg)
    updataReg.io.clk := clock
    updataReg.io.rdAddr := io.rdAddr
    updataReg.io.rdData := io.rdData
    updataReg.io.regWrite := io.regWrite
}