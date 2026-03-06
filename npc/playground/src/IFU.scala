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
    val snpc = pcReg + 4.U
    val dnpc = Mux(jump === 1.U, io.jumpAddr, snpc)

    pcReg := dnpc
    io.pc := pcReg

    val instFetch = Module(new InstFetch)
    instFetch.io.reset := reset
    instFetch.io.pc := pcReg
    io.inst := instFetch.io.inst

}