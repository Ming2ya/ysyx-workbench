module top(
    input clk,
    input reset,
    input [31:0] inst,
    input [31:0] MemDout,
    output [31:0] pc,
    output MemRead,
    output MemWrite
);
    wire        Jump;
    wire        Branch;
    wire [31:0] JumpAddr;
    wire [31:0] src1;
    wire [31:0] src2;
    wire [31:0] imm;
    wire [31:0] ALUResult;
    wire        ALUSrcA;
    wire [1:0]  ALUSrcB;

    IFU u_IFU(
        .clk 	    ( clk       ),
        .rst 	    ( reset     ),
        .Jump       (Jump       ),
        .Branch     (Branch     ),
        .JumpAddr   (JumpAddr   ),
        .pc  	    ( pc        )
    );

    IDU u_IDU(
        .clk       	( clk       ),
        .pc         ( pc        ),
        .inst      	( inst      ),
        .ALUResult 	( ALUResult ),
        .MemDout   	( MemDout   ),
        .src1      	( src1      ),
        .src2      	( src2      ),
        .imm       	( imm       ),
        .JumpAddr   ( JumpAddr  ),
        .MemRead   	( MemRead   ),
        .MemWrite  	( MemWrite  ),
        .Jump       ( Jump      ),
        .Branch     ( Branch    ),
        .ALUSrcA   	( ALUSrcA   ),
        .ALUSrcB   	( ALUSrcB   )
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
