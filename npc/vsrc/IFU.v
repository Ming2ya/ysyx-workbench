module IFU(
    input         clk,
    input         rst,
    input         Jump,
    input         Branch,
    input  [31:0] JumpAddr,
    output [31:0] pc
);
    wire [31:0] snpc, dnpc, pc_in;
    wire pc_sel;
    assign pc_sel = Jump | Branch;

    ALU_add u_ALU_add(
        .a  	( pc    ),
        .b  	( 32'd4 ),
        .ci 	( 1'b0  ),
        .s  	( snpc  ),
        .co 	(       )
    );

    assign dnpc = JumpAddr;
    assign pc_in = (pc_sel == 1'b1)? dnpc: snpc;

    Reg #(32, 32'h80000000)
    PC_reg(
        .clk  	(clk   ),
        .rst  	(rst   ),
        .din  	(pc_in ),
        .dout 	(pc    ),
        .wen  	(1'b1  )
    );
    
endmodule
