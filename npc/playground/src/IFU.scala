package npc.core

import chisel3._
import npc.dpi._

class IFU extends Module{
    val io = IO(new Bundle{
        val branch = Input(UInt(1.W))
        val jump = Input(UInt(1.W))
        val jumpAddr = Input(UInt(32.W))
        val pc = Output(UInt(32.W))
        val inst = Output(UInt(32.W))
    })

    val pcReg = RegInit(0x80000000L.U(32.W))
    val jump = io.branch | io.jump

    pcReg := Mux(jump === 1.U, io.jumpAddr, pcReg + 4.U)
    io.pc := pcReg

    val instFetch = Module(new InstFetch)
    instFetch.io.clk := clock
    instFetch.io.pc := pcReg
    io.inst := instFetch.io.inst

}