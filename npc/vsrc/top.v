module top(
    input clk,
    input reset,
    input [31:0] inst,
    input [31:0] MemDout,
    output [31:0] pc,
    output MemRead,
    output MemWrite,
    output [2:0] state
);
    wire        Jump;
    wire        Branch;
    wire        Ebreak;
    wire [31:0] JumpAddr;
    wire [31:0] src1;
    wire [31:0] src2;
    wire [31:0] imm;
    wire [31:0] ALUResult;
    wire [1:0]  ALUSrcA;
    wire [1:0]  ALUSrcB;

localparam NEMU_RUNNING = 3'd0;
localparam NEMU_STOP    = 3'd1;
localparam NEMU_END     = 3'd2;
localparam NEMU_ABORT   = 3'd3;
localparam NEMU_QUIT    = 3'd4;

    wire [2:0] cur_state, next_state;

    assign next_state = (Ebreak) ? NEMU_END: NEMU_RUNNING;
    
    Reg #(3, 3'd0)
    cur_state_Reg (
        .clk  	(clk        ),
        .rst  	(reset      ),
        .din  	(next_state ),
        .dout 	(cur_state  ),
        .wen  	(1'b1       )
    );
    assign state = cur_state;

    wire halt = (cur_state != NEMU_RUNNING);


    IFU u_IFU(
        .clk 	    ( clk       ),
        .rst 	    ( reset     ),
        .halt       ( halt      ),
        .Jump       ( Jump      ),
        .Branch     ( Branch    ),
        .JumpAddr   ( JumpAddr  ),
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
        .Ebreak     ( Ebreak    ),
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
