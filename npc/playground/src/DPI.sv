module UpdateReg(
    input clk,
    input reset,
    input [4:0] rdAddr,
    input [31:0] rdData,
    input regWrite
);
    import "DPI-C" context function void npc_update_reg(input int unsigned addr, input int unsigned data);
    always @(posedge clk) begin
        if (!reset && regWrite && |rdAddr) begin
            npc_update_reg({27'b0, rdAddr}, rdData);
        end
    end

endmodule

module InstFetch(
    input reset,
    input logic [31:0] pc,
    output logic [31:0] inst
);
    import "DPI-C" pure function int unsigned npc_inst_fetch(input int unsigned addr);
    assign inst = npc_inst_fetch(reset? 32'h80000000: pc);

endmodule

module DecResult(
    input valid,
    input ebreak,
    input [31:0] pc
);
    export "DPI-C" function npc_inv;
    function int unsigned npc_inv();
        npc_inv = {31'b0, !valid};
    endfunction

    export "DPI-C" function npc_ebreak;
    function int unsigned npc_ebreak();
        npc_ebreak = {31'b0, ebreak};
    endfunction

    export "DPI-C" function npc_get_pc;
    function int unsigned npc_get_pc();
        npc_get_pc = pc;
    endfunction

endmodule

// module MEM(
//     input clk,
//     input MemtoReg,
//     input 
// );
// endmodule
