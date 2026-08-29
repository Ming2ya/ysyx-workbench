package npc.core

import chisel3._
import npc.defines.decode._
import chisel3.util.Mux1H
import chisel3.util.MuxLookup

class EXU extends Module{
    val io = IO(new Bundle {
        val imm     = Input(UInt(32.W))     //可能不需要了，暂留
        val pc      = Input(UInt(32.W))
        val aluSrcA = Input(UInt(32.W))
        val aluSrcB = Input(UInt(32.W))
        val aluCode = Input(UInt(4.W))
        val aluResult = Output(UInt(32.W))
        //val memWirteData = Output(UInt(32.W))
    })

    val alu = Module(new ALU)
    alu.io.aluSrcA := io.aluSrcA
    alu.io.aluSrcB := io.aluSrcB
    alu.io.aluCode := io.aluCode
    io.aluResult   := alu.io.aluResult
}

class ALU extends Module{
    val io = IO(new Bundle {
        val aluCode = Input(UInt(4.W))
        val aluSrcA = Input(UInt(32.W))
        val aluSrcB = Input(UInt(32.W))
        val aluResult = Output(UInt(32.W))
    })

    val add = io.aluSrcA + io.aluSrcB
    val sub = io.aluSrcA - io.aluSrcB
    val and = io.aluSrcA & io.aluSrcB
    val or  = io.aluSrcA | io.aluSrcB
    val xor = io.aluSrcA ^ io.aluSrcB
    val sll = io.aluSrcA << io.aluSrcB(4, 0)
    val srl = io.aluSrcA >> io.aluSrcB(4, 0)
    val sra = (io.aluSrcA.asSInt >> io.aluSrcB(4, 0)).asUInt
    val slt = (io.aluSrcA.asSInt < io.aluSrcB.asSInt).asUInt
    val sltu= (io.aluSrcA < io.aluSrcB).asUInt
    val lui = io.aluSrcB

    val aluResult = MuxLookup(
        io.aluCode,
        0.U(32.W)
        )(Seq(
            ALU_ADD  -> add,
            ALU_SUB  -> sub,
            ALU_AND  -> and,
            ALU_OR   -> or,
            ALU_XOR  -> xor,
            ALU_SLL  -> sll,
            ALU_SRL  -> srl,
            ALU_SRA  -> sra,
            ALU_SLT  -> slt,
            ALU_SLTU -> sltu,
            ALU_LUI  -> lui
        )
    )
    io.aluResult := aluResult
}