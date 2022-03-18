module data_memory(ReadData, Address, WriteData, MemWrite, clk);
input MemWrite, clk;
input [31:0] WriteData, Address;
output reg [31:0] ReadData;
reg [31:0] data [255:0];

always @(*) begin
	ReadData <= data[Address];
end

always @(posedge clk) begin
	if(MemWrite)
		data[Address] <= WriteData;
end
endmodule
