module LFSR(
    input [7:0] din,
    input op,
    input clk,
    output [7:0] hex1, hex0
    );

    reg [7:0] digit;
    hex7seg i1(.x(digit[7:4]), .y(hex1));
    hex7seg i0(.x(digit[3:0]), .y(hex0));
    always @(posedge clk) begin
        if (op) begin
            digit <= din;
        end
        else begin
            digit <= {digit[4] ^ digit[3] ^ digit[2] ^ digit[0], digit[7:1]};
        end
    end

endmodule

module hex7seg(     //低电平有效
    input [3:0] x,
    output reg [7:0] y
    );
    always @(x) begin
        case(x)
            4'd0: y = ~8'b11111100;
            4'd1: y = ~8'b01100000;
            4'd2: y = ~8'b11011010;
            4'd3: y = ~8'b11110010;
            4'd4: y = ~8'b01100110;
            4'd5: y = ~8'b10110110;
            4'd6: y = ~8'b10111110;
            4'd7: y = ~8'b11100000;
            4'd8: y = ~8'b11111110;
            4'd9: y = ~8'b11110110;
            4'd10: y = ~8'b11101110;
            4'd11: y = ~8'b00111110;
            4'd12: y = ~8'b10011100;
            4'd13: y = ~8'b01111010;
            4'd14: y = ~8'b10011110;
            4'd15: y = ~8'b10001110;
        endcase
    end
endmodule
