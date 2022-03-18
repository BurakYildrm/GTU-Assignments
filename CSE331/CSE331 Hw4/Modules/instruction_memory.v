module instruction_memory(instruction, address, clk);
input [31:0] address;
input clk;
output reg [15:0] instruction;
reg [15:0] instructions [63:0];

always @(posedge clk) begin
	instruction <= instructions[address];
end

endmodule
