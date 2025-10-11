module IDU(
    input clk,
    //input rst,
    input [31:0] inst,
    input [31:0] ALUResult,
    input [31:0] MemDout,
    output [31:0] src1,
    output [31:0] src2,
    output [31:0] imm,
    output MemRead,
    output MemWrite,
    output ALUSrcA,
    output [1:0] ALUSrcB
    );

    wire [6:0] op;
    assign op = inst[6:0];
    wire R_type, I_type, B_type, LW, JALR, SW, LUI, AUIPC, JAL;
    assign R_type = (op == 7'h33);
    assign I_type = (op == 7'h13);
    assign B_type = (op == 7'h63);
    assign LW = (op == 7'h03);
    assign JALR = (op == 7'h67);
    assign SW = (op == 7'h23);
    assign LUI = (op == 7'h37);
    assign AUIPC = (op == 7'h17);
    assign JAL = (op == 7'h6F);

    wire [8:0] inst_type;
    assign inst_type = {R_type, I_type, B_type, LW, JALR, SW, LUI, AUIPC, JAL};

    wire [31:0] immI, immU, immJ, immS, immB;
    assign immI = {{20{inst[31]}}, inst[31:20]};
    assign immU = {inst[31:12], 12'd0};
    assign immJ = {{11{inst[31]}}, inst[31], inst[19:12], inst[20], inst[30:21], 1'b0};
    assign immS = {{20{inst[31]}}, inst[31:25], inst[11:7]};
    assign immB = {{19{inst[31]}}, inst[31], inst[7], inst[30:25], inst[11:8], 1'b0};
    
    MuxKey #(8, 9, 32) u_Mux_imm(
        .out 	( imm       ),
        .key 	( inst_type ),
        .lut 	( {9'b010000000, immI,
                   9'b001000000, immB,
                   9'b000100000, immI,
                   9'b000010000, immI,
                   9'b000001000, immS,
                   9'b000000100, immU,
                   9'b000000010, immU,
                   9'b000000001, immJ
                } )
    );

    wire MemtoReg, RegWrite, Jump, Branch;
    assign MemtoReg = LW;
    assign RegWrite = R_type | I_type | LW | JALR | LUI | AUIPC | JAL;
    assign MemRead = LW;
    assign MemWrite = SW;
    // 跳转未实现

    wire [4:0] rd, rs1, rs2;
    assign rd = inst[11:7];
    assign rs1 = inst[19:15];
    assign rs2 = inst[24:20];
    wire [31:0] wdata;
    assign wdata = (MemtoReg == 1'b1)? MemDout: ALUResult;

    RegisterFile gpr(
        .clk     	( clk      ),
        .wdata   	( wdata    ),
        .waddr   	( rd       ),
        .wen     	( RegWrite ),
        .rs1addr 	( rs1      ),
        .rs2addr 	( rs2      ),
        .rs1data 	( src1     ),
        .rs2data 	( src2     )
    );

    assign ALUSrcA = JAL | JALR | AUIPC;
    assign ALUSrcB[0] = ~(R_type | JAL | JALR);
    assign ALUSrcB[1] = JAL | AUIPC;

endmodule

module RegisterFile #(ADDR_WIDTH = 5, DATA_WIDTH = 32) (
  input clk,
  input [DATA_WIDTH-1:0] wdata,
  input [ADDR_WIDTH-1:0] waddr,
  input wen,
  input [ADDR_WIDTH-1:0] rs1addr, rs2addr,
  output [DATA_WIDTH-1:0] rs1data, rs2data
);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];
  always @(posedge clk) begin
    if (wen) rf[waddr] <= wdata;
  end
  assign rs1data = (rs1addr == 5'd0)? 32'd0: rf[rs1addr];
  assign rs2data = (rs2addr == 5'd0)? 32'd0: rf[rs2addr];

endmodule
