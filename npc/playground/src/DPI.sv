module UpdateReg(
    input clk,
    input [4:0] rdAddr,
    input [31:0] rdData,
    input regWrite
);
    import "DPI-C" context function void npc_update_reg(input int unsigned addr, input int unsigned data);
    always @(posedge clk) begin
        if (regWrite) begin
            npc_update_reg({27'b0, rdAddr}, rdData);
        end
    end

endmodule

module InstFetch(
    input clk,
    input logic [31:0] pc,
    output logic [31:0] inst
);
    import "DPI-C" context function int unsigned npc_inst_fetch(input int unsigned addr);
    always @(posedge clk) begin
        inst <= npc_inst_fetch(pc);
    end

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
