module alu(
    input [3:0] x, y,
    input [2:0] op,
    output reg [3:0] out,
    output cf, of, zf
    );
    reg [3:0] out_add;
    reg r_cf, r_of, r_zf;
    add add1(.x(x), .y(y), .op(op[0]), .out(out_add), 
            .cf(r_cf), .of(r_of), .zf(r_zf));

    always @(*) begin
        case(op)
            3'b000: out = out_add;
            3'b001: out = out_add;
            3'b010: out = ~x;
            3'b011: out = x & y;
            3'b100: out = x | y;
            3'b101: out = x ^ y;
            3'b110: out = x < y ? 4'd1: 4'd0;
            3'b111: out = x == y ? 1: 0;
        endcase
    end
    assign cf = (~| op[2:1]) ? r_cf: 0;
    assign of = (~| op[2:1]) ? r_of: 0;
    assign zf = (~| op[2:1]) ? r_zf: 0;

endmodule

module add(
    input [3:0] x, y,
    input op,
    output [3:0] out,
    output cf, of, zf
    );
    reg [3:0] z;
    always @(*) begin
        if (op == 0) z = y;
        else z = ~y + 4'b1000;
    end
    assign {cf, out} = x + z;
    assign of = (x[3] == z[3]) && (x[3] != out[3]);
    assign zf = ~(| out);
endmodule
