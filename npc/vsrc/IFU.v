module IFU(
    input clk,
    input rst,
    output [31:0] pc
);
    wire [31:0] snpc;
    assign snpc = pc + 32'd4;

    Reg #(32, 32'h80000000)
    PC_reg(
        .clk  	(clk   ),
        .rst  	(rst   ),
        .din  	(snpc   ),
        .dout 	(pc    ),
        .wen  	(1'b1  )
    );
    
endmodule
