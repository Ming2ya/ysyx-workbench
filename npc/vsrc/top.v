module top(
    input clk,
    input reset,
    input [31:0] inst,
    input [31:0] MemDout,
    output [31:0] pc,
    output MemRead,
    output MemWrite
);

    IFU u_IFU(
        .clk 	( clk   ),
        .rst 	( reset ),
        .pc  	( pc    )
    );

    wire [31:0] src1;
    wire [31:0] src2;
    wire [31:0] imm;
    wire [31:0] ALUResult;
    wire        ALUSrcA;
    wire [1:0]  ALUSrcB;

    IDU u_IDU(
        .clk       	( clk        ),
        .inst      	( inst       ),
        .ALUResult 	( ALUResult  ),
        .MemDout   	( MemDout    ),
        .src1      	( src1       ),
        .src2      	( src2       ),
        .imm       	( imm        ),
        .MemRead   	( MemRead    ),
        .MemWrite  	( MemWrite   ),
        .ALUSrcA   	( ALUSrcA    ),
        .ALUSrcB   	( ALUSrcB    )
    );

    EXU u_EXU(
        .src1      	( src1       ),
        .src2      	( src2       ),
        .imm       	( imm        ),
        .pc        	( pc         ),
        .ALUSrcA   	( ALUSrcA    ),
        .ALUSrcB   	( ALUSrcB    ),
        .ALUResult 	( ALUResult  )
    );

endmodule
