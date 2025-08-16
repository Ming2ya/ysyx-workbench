module light(
	input clk,
	input rst,
	output reg [15:0] led
);
	reg [31:0] cnt;
	always @(posedge clk) begin
		if (rst) begin led <= 1;cnt <= 0; end
		else begin
			if (cnt == 0) led <= {led[14:0], led[15]};
			cnt <= (cnt >= 5000000) ? 32'b0 : cnt + 1;
		end
	end
endmodule
