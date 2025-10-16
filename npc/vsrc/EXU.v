module EXU(
    input [31:0] src1, src2,
    input [31:0] imm,
    input [31:0] pc,
    input [1:0] ALUSrcA,
    input [1:0] ALUSrcB,
    output [31:0] ALUResult
    );

    wire [31:0] ALU_A, ALU_B;

    MuxKey #(3, 2, 32) Mux_ALU_A(
        .out 	( ALU_A   ),
        .key 	( ALUSrcA ),
        .lut 	( {2'b00, src1, 2'b01, pc, 2'b10, 32'd0} )
    );

    MuxKey #(3, 2, 32) Mux_ALU_B(
        .out 	( ALU_B   ),
        .key 	( ALUSrcB ),
        .lut 	( {2'b00, src2, 2'b01, imm, 2'b10, 32'd4} )
    );

    ALU_add u_ALU_add(
        .a  	( ALU_A  ),
        .b  	( ALU_B  ),
        .ci 	( 1'b0   ),
        .s  	( ALUResult ),
        .co 	(        )
    );

endmodule
