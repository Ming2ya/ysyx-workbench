module ALU_add(
    input [31:0] a, b,
    input ci,
    output [31:0] s,
    output co
);
    wire [8:0] carry;

    assign carry[0] = ci;

    genvar i;
    generate
        for (i = 0; i < 8; i = i + 1) begin : adder_block
            adder_4bits u_adder(
                .a  	(a[4*i+3 : 4*i] ),
                .b  	(b[4*i+3 : 4*i] ),
                .ci 	(carry[i]       ),
                .s  	(s[4*i+3 : 4*i] ),
                .co 	(carry[i + 1]   )
            );
        end
    endgenerate
    assign co = carry[8];
endmodule

module adder_select_4bits(
    input [3:0] a, b,
    input ci,
    output [3:0] s,
    output co
);
    wire [3:0]  s_1, s_0;
    wire        c_1, c_0;

    adder_4bits u_adder_4bits_1(
        .a  	( a    ),
        .b  	( b    ),
        .ci 	( 1'b1 ),
        .s  	( s_1  ),
        .co 	( c_1  )
    );

    adder_4bits u_adder_4bits_0(
        .a  	( a    ),
        .b  	( b    ),
        .ci 	( 1'b0 ),
        .s  	( s_0  ),
        .co 	( c_0  )
    );

    assign s = (ci == 1'b1)? s_1: s_0;
    assign co = (ci == 1'b1)? c_1: c_0;

endmodule

module adder_4bits(
    input [3:0] a, b,
    input ci,
    output [3:0] s,
    output co
);
    wire [3:0] p, g, c;
    assign p = a | b;
    assign g = a & b;
    assign c[0] = ci;
    assign c[1] = g[0] | p[0] & ci;
    assign c[2] = g[1] | p[1] & g[0] | p[1] & p[0] & ci;
    assign c[3] = g[2] | p[2] & g[1] | p[2] & p[1] & g[0] | p[2] & p[1] & p[0] & ci;
    assign co = g[3] | p[3] & g[2] | p[3] & p[2] & g[1] | p[3] & p[2] & p[1] & g[0] | p[3] & p[2] & p[1] & p[0] & ci;
    assign s = p & ~g ^ c;
endmodule
