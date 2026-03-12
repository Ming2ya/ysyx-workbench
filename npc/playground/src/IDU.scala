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
        val memMask = Output(UInt(8.W))
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
        List(ALU_X, OP1_X, OP2_X, MEM_X, REN_X, WB_X, IMM_X, JUMP_X, BRANCH_X, BASE_X, EBREAK_X, VALID_X),
        Array(
            ADD   -> List(ALU_ADD , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SUB   -> List(ALU_SUB , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SLT   -> List(ALU_SLT , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SLTU  -> List(ALU_SLTU, OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            XOR   -> List(ALU_XOR , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            OR    -> List(ALU_OR  , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            AND   -> List(ALU_AND , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SLL   -> List(ALU_SLL , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SRL   -> List(ALU_SRL , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SRA   -> List(ALU_SRA , OP1_RS1, OP2_RS2 , MEM_X , REN_Y, WB_ALU, IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),

            ADDI  -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SLTI  -> List(ALU_SLT , OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SLTUI -> List(ALU_SLTU, OP1_RS1, OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),

            LB    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_B , REN_Y, WB_MEM, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            LH    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_H , REN_Y, WB_MEM, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            LW    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_W , REN_Y, WB_MEM, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            LBU   -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_BU, REN_Y, WB_MEM, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            LHU   -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_HU, REN_Y, WB_MEM, IMM_I, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),

            JALR  -> List(ALU_ADD , OP1_PC , OP2_FOUR, MEM_X , REN_Y, WB_ALU, IMM_I, JUMP_Y, BRANCH_X, BASE_RS1, EBREAK_X, VALID_Y),

            SB    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_B , REN_X, WB_ALU, IMM_S, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SH    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_H , REN_X, WB_ALU, IMM_S, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            SW    -> List(ALU_ADD , OP1_RS1, OP2_IMM , MEM_W , REN_X, WB_ALU, IMM_S, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),

            LUI   -> List(ALU_LUI , OP1_X  , OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_U, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),
            AUIPC -> List(ALU_ADD , OP1_PC , OP2_IMM , MEM_X , REN_Y, WB_ALU, IMM_U, JUMP_X, BRANCH_X, BASE_X  , EBREAK_X, VALID_Y),

            JAL   -> List(ALU_ADD , OP1_PC , OP2_FOUR, MEM_X , REN_Y, WB_ALU, IMM_J, JUMP_Y, BRANCH_X, BASE_PC , EBREAK_X, VALID_Y),

            EBREAK-> List(ALU_X   , OP1_X  , OP2_X   , MEM_X , REN_X, WB_X  , IMM_X, JUMP_X, BRANCH_X, BASE_X  , EBREAK_Y, VALID_Y)
        )
    )
    val alu_code::op1_sel::op2_sel::mem_mask::rf_wen::wb_sel::imm_sel::jump::branch::base_sel::ebreak::valid::Nil = ctrl
    io.aluCode := alu_code
    io.regWrite:= rf_wen
    io.memToReg:= (wb_sel === WB_MEM).asUInt
    io.memMask := mem_mask
    io.writeMem:= (opcode === "b0100011".U).asUInt
    io.jump    := jump
    io.branch  := branch    // 分支测试未完成
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
    val base = Mux(base_sel === BASE_PC, io.pc, io.rs1Data)
    io.jumpAddr := base + imm

}

class MEM extends Module {              // 假设访存指令都是4字节对齐的
    val io = IO(new Bundle {
        val valid = Input(UInt(1.W))
        val raddr = Input(UInt(32.W))
        val wen   = Input(UInt(1.W))
        val waddr = Input(UInt(32.W))
        val wdata = Input(UInt(32.W))
        val wmask = Input(UInt(8.W))
        val rdata = Output(UInt(32.W))
    })

    val MEM_DPI = Module(new MEM_DPI)
    MEM_DPI.io.valid := io.valid
    MEM_DPI.io.raddr := io.raddr
    MEM_DPI.io.wen   := io.wen
    MEM_DPI.io.waddr := io.waddr
    MEM_DPI.io.wdata := io.wdata
    MEM_DPI.io.wmask := io.wmask

    io.rdata := MuxLookup(
        io.wmask,
        0.U(32.W),
        )(Seq(
            MEM_X  -> 0.U(32.W),
            MEM_B  -> MEM_DPI.io.rdata(7, 0).asSInt.asTypeOf(SInt(32.W)).asUInt,
            MEM_H  -> MEM_DPI.io.rdata(15, 0).asSInt.asTypeOf(SInt(32.W)).asUInt,
            MEM_W  -> MEM_DPI.io.rdata,
            MEM_BU -> MEM_DPI.io.rdata(7, 0).asTypeOf(UInt(32.W)),
            MEM_HU -> MEM_DPI.io.rdata(15, 0).asTypeOf(UInt(32.W))
        )
    )
}