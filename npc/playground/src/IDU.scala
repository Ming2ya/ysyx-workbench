package npc.core

import chisel3._
import chisel3.util._
import npc.defines.decode._
import npc.dpi.MEM_DPI

class IDU extends Module{
    val io = IO(new Bundle {
        val inst    = Input(UInt(32.W))
        val pc      = Input(UInt(32.W))
        val rs1Data = Input(UInt(32.W))
        val rs2Data = Input(UInt(32.W))
        val rs1Addr = Output(UInt(5.W))
        val rs2Addr = Output(UInt(5.W))
        val rdAddr  = Output(UInt(5.W))
        val regWrite= Output(UInt(1.W))
        val memToReg= Output(UInt(1.W))
        val writeMem= Output(UInt(1.W))
        val memType = Output(UInt(3.W))
        val aluSrcA = Output(UInt(32.W))
        val aluSrcB = Output(UInt(32.W))
        val aluCode = Output(UInt(4.W))
        val imm     = Output(UInt(32.W))    //可能不需要了，暂留
        val jump    = Output(UInt(1.W))
        val branch  = Output(UInt(1.W))
        val jumpAddr= Output(UInt(32.W))
        val ebreak  = Output(UInt(1.W))
        val valid   = Output(UInt(1.W))
    })
    val funct7  = io.inst(31, 25)
    val funct3  = io.inst(14, 12)
    val opcode  = io.inst(6, 0)

    io.rs1Addr := io.inst(19, 15)
    io.rs2Addr := io.inst(24, 20)
    io.rdAddr  := io.inst(11, 7)

    val ctrl = ListLookup( io.inst,
        List(ALU_X, OP1_X, OP2_X, MEM_X, REN_X, WB_X, IMM_X, JUMP_X, BR_X, BASE_X, EBREAK_X, VALID_X),
        Array(
            ADD   -> List(ALU_ADD , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SUB   -> List(ALU_SUB , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLT   -> List(ALU_SLT , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLTU  -> List(ALU_SLTU, OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            XOR   -> List(ALU_XOR , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            OR    -> List(ALU_OR  , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            AND   -> List(ALU_AND , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLL   -> List(ALU_SLL , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SRL   -> List(ALU_SRL , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SRA   -> List(ALU_SRA , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),

            ADDI  -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLTI  -> List(ALU_SLT , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLTUI -> List(ALU_SLTU, OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            XORI  -> List(ALU_XOR , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            ANDI  -> List(ALU_AND , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SLLI  -> List(ALU_SLL , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SRLI  -> List(ALU_SRL , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SRAI  -> List(ALU_SRA , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),

            LB    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_B , REN_Y, WB_MEM, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            LH    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_H , REN_Y, WB_MEM, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            LW    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_W , REN_Y, WB_MEM, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            LBU   -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_BU, REN_Y, WB_MEM, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            LHU   -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_HU, REN_Y, WB_MEM, IMM_I, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),

            JALR  -> List(ALU_ADD , OP1_PC , OP2_FOUR, MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_Y, BR_X   , BASE_RS1, EBREAK_X, VALID_Y),

            SB    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_B , REN_X, WB_ALU, IMM_S, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SH    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_H , REN_X, WB_ALU, IMM_S, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            SW    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_W , REN_X, WB_ALU, IMM_S, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),

            LUI   -> List(ALU_LUI , OP1_X  , OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_U, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),
            AUIPC -> List(ALU_ADD , OP1_PC , OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_U, JUMP_X, BR_X   , BASE_X  , EBREAK_X, VALID_Y),

            JAL   -> List(ALU_ADD , OP1_PC , OP2_FOUR, MEM_X , REN_Y, WB_ALU, IMM_J, JUMP_Y, BR_X   , BASE_PC , EBREAK_X, VALID_Y),

            BEQ   -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BEQ , BASE_PC , EBREAK_X, VALID_Y),
            BNE   -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BNE , BASE_PC , EBREAK_X, VALID_Y),
            BLT   -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BLT , BASE_PC , EBREAK_X, VALID_Y),
            BGE   -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BGE , BASE_PC , EBREAK_X, VALID_Y),
            BLTU  -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BLTU, BASE_PC , EBREAK_X, VALID_Y),
            BGEU  -> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_ALU, IMM_B, JUMP_X, BR_BGEU, BASE_PC , EBREAK_X, VALID_Y),

            EBREAK-> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_X  , IMM_X, JUMP_X, BR_X   , BASE_X  , EBREAK_Y, VALID_Y)
        )
    )
    val alu_code::op1_sel::op2_sel::mem_type::rf_wen::wb_sel::imm_sel::jump::bru_code::base_sel::ebreak::valid::Nil = ctrl
    io.aluCode := alu_code
    io.regWrite:= rf_wen
    io.memToReg:= (wb_sel === WB_MEM).asUInt
    io.memType := mem_type
    io.writeMem:= (opcode === "b0100011".U).asUInt
    io.jump    := jump
    io.ebreak  := ebreak
    io.valid   := valid

    // val decResult = Module(new DecResult)
    // decResult.io.ebreak  := ebreak
    // decResult.io.valid   := valid
    // decResult.io.pc      := io.pc

    io.aluSrcA := MuxLookup(
        op1_sel,
        0.U(32.W)
        )(Seq(
            OP1_RS1 -> io.rs1Data,
            OP1_PC  -> io.pc
        )
    )

    io.aluSrcB := MuxLookup(
        op2_sel,
        0.U(32.W)
        )(Seq(
            OP2_RS2 -> io.rs2Data,
            OP2_IMM -> io.imm,
            OP2_FOUR-> 4.U(32.W)
        )
    )

    // ImmGen
    val immI = Cat(Fill(20, io.inst(31)), io.inst(31, 20))
    val immU = Cat(io.inst(31, 12), 0.U(12.W))
    val immS = Cat(Fill(20, io.inst(31)), io.inst(31, 25), io.inst(11, 7))
    val immJ = Cat(Fill(11, io.inst(31)), io.inst(31), io.inst(19, 12), io.inst(20), io.inst(30, 21), 0.U(1.W))
    val immB = Cat(Fill(19, io.inst(31)), io.inst(31), io.inst(7), io.inst(30, 25), io.inst(11, 8), 0.U(1.W))

    val imm = MuxLookup(imm_sel, 0.U(32.W))(
        Seq(
        IMM_I -> immI,
        IMM_U -> immU,
        IMM_S -> immS,
        IMM_J -> immJ,
        IMM_B -> immB
        )
    )
    io.imm := imm

    // jumpAddr
    io.jumpAddr := Mux(base_sel === BASE_RS1, (io.rs1Data + imm) & ~1.U(32.W), io.pc + imm)

    // branch test
    val bru = Module(new BRU)
    bru.io.bruCode := bru_code
    bru.io.bruSrcA := io.rs1Data
    bru.io.bruSrcB := io.rs2Data
    io.branch := bru.io.bruOutput

}

class MEM extends Module {
    val io = IO(new Bundle {
        val valid   = Input(UInt(1.W))
        val raddr   = Input(UInt(32.W))
        val wen     = Input(UInt(1.W))
        val waddr   = Input(UInt(32.W))
        val wdata   = Input(UInt(32.W))
        val memType = Input(UInt(3.W))
        val rdata   = Output(UInt(32.W))
    })

    val MEM_DPI = Module(new MEM_DPI)
    MEM_DPI.io.clk   := clock
    MEM_DPI.io.reset := reset
    MEM_DPI.io.valid := io.valid
    MEM_DPI.io.wen   := io.wen

    // lh/lhu：地址必须是 2 字节对齐
    when (
        (io.valid === 1.U) &&
        ((io.memType === MEM_H) || (io.memType === MEM_HU))
    ) {
        assert(!io.raddr(0), "misaligned half-word load")
    }

    // lw：地址必须是 4 字节对齐
    when ((io.valid === 1.U) && (io.memType === MEM_W)) {
        assert(io.raddr(1, 0) === 0.U, "misaligned word load")
    }

    // sh：地址必须是 2 字节对齐
    when ((io.wen === 1.U) && (io.memType === MEM_H)) {
        assert(!io.waddr(0), "misaligned half-word store")
    }

    // sw：地址必须是 4 字节对齐
    when ((io.wen === 1.U) && (io.memType === MEM_W)) {
        assert(io.waddr(1, 0) === 0.U, "misaligned word store")
    }

    val wByteOffset = io.waddr(1, 0)
    val wHalfOffset = io.waddr(1)        // 暂时不支持0x1,0x3 offset的lh/sh指令
    MEM_DPI.io.waddr := io.waddr & ~(0x3.U(32.W))
    MEM_DPI.io.wdata := MuxLookup(
        io.memType,
        0.U(32.W)
        )(Seq(
            MEM_X  -> 0.U(32.W),
            MEM_B  -> (io.wdata(7, 0) << Cat(wByteOffset, 0.U(3.W))),
            MEM_H  -> (io.wdata(15, 0) << Cat(wHalfOffset, 0.U(4.W))),
            MEM_W  -> io.wdata
        )
    )
    MEM_DPI.io.wmask := MuxLookup(
        io.memType,
        0.U(8.W)
        )(Seq(
            MEM_X  -> 0.U(8.W),
            MEM_B  -> (1.U << wByteOffset),
            MEM_H  -> (3.U << (wHalfOffset << 1)),
            MEM_W  -> 15.U(8.W)
        )
    )

    val rByteOffset = io.raddr(1, 0)
    val rHalfOffset = io.raddr(1)        // 暂时不支持0x1,0x3 offset的lh/sh指令
    MEM_DPI.io.raddr := io.raddr & ~(0x3.U(32.W))
    val byteData = MuxLookup(
        rByteOffset,
        0.U(8.W)
        )(Seq(
            0.U -> MEM_DPI.io.rdata(7, 0),
            1.U -> MEM_DPI.io.rdata(15, 8),
            2.U -> MEM_DPI.io.rdata(23, 16),
            3.U -> MEM_DPI.io.rdata(31, 24)
        )
    )
    val halfData = MuxLookup(
        rHalfOffset,
        0.U(16.W)
        )(Seq(
            0.U -> MEM_DPI.io.rdata(15, 0),
            1.U -> MEM_DPI.io.rdata(31, 16)
        )
    )
    io.rdata := MuxLookup(
        io.memType,
        0.U(32.W),
        )(Seq(
            MEM_X  -> 0.U(32.W),
            MEM_B  -> Cat(Fill(24, byteData(7)), byteData),
            MEM_H  -> Cat(Fill(16, halfData(15)), halfData),
            MEM_W  -> MEM_DPI.io.rdata,
            MEM_BU -> Cat(0.U(24.W), byteData),
            MEM_HU -> Cat(0.U(16.W), halfData)
        )
    )
}

class BRU extends Module {
    val io = IO(new Bundle {
        val bruCode = Input(UInt(3.W))
        val bruSrcA = Input(UInt(32.W))
        val bruSrcB = Input(UInt(32.W))
        val bruOutput = Output(UInt(1.W))
    })
    io.bruOutput := MuxLookup(io.bruCode, false.B)(Seq(
        BR_BEQ  -> (io.bruSrcA === io.bruSrcB),
        BR_BNE  -> (io.bruSrcA =/= io.bruSrcB),
        BR_BLT  -> (io.bruSrcA.asSInt <  io.bruSrcB.asSInt),
        BR_BGE  -> (io.bruSrcA.asSInt >= io.bruSrcB.asSInt),
        BR_BLTU -> (io.bruSrcA <  io.bruSrcB),
        BR_BGEU -> (io.bruSrcA >= io.bruSrcB)
  ))
}
