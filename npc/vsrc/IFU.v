module IFU(
    input clk,
    input rst,
    output [31:0] pc
);
    wire [31:0] snpc;

    ALU_add u_ALU_add(
        .a  	( pc    ),
        .b  	( 32'd4 ),
        .ci 	( 1'b0  ),
        .s  	( snpc  ),
        .co 	(       )
    );

    Reg #(32, 32'h80000000)
    PC_reg(
        .clk  	(clk   ),
        .rst  	(rst   ),
        .din  	(snpc  ),
        .dout 	(pc    ),
        .wen  	(1'b1  )
    );
    
endmodule
