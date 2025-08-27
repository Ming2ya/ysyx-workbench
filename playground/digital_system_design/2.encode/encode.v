module encode(
    input [7:0] x,
    input en,
    output reg [7:0] y
    );
    reg [2:0] bin;
    integer i;
    always @(x or en) begin
        if (en) begin
            bin = 3'd0;
            for (i = 0; i < 8; i = i + 1) begin
                if (x[i] == 1'b1)
                    bin = i[2:0];
            end
        end
        else bin = 3'd0;
    end
    bin7seg i0(bin, y);
endmodule

module bin7seg(     //低电平有效
    input [2:0] x,
    output reg [7:0] y
    );
    always @(x) begin
        case(x)
            3'd0: y = ~8'b11111100;
            3'd1: y = ~8'b01100000;
            3'd2: y = ~8'b11011010;
            3'd3: y = ~8'b11110010;
            3'd4: y = ~8'b01100110;
            3'd5: y = ~8'b10110110;
            3'd6: y = ~8'b10111110;
            3'd7: y = ~8'b11100000;
            default: y = ~8'b00000000;
        endcase
    end
endmodule
