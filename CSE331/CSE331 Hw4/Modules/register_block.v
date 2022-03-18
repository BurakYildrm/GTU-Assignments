module register_block(ReadData1, ReadData2, ReadReg1, ReadReg2, WriteReg, WriteData, RegWrite, clk);
input RegWrite, clk;
input [2:0] ReadReg1, ReadReg2, WriteReg;
input [31:0] WriteData;
output reg [31:0] ReadData1, ReadData2;
reg [31:0] registers [7:0];

always @(*) begin
	ReadData1 <= registers[ReadReg1];
	ReadData2 <= registers[ReadReg2];
end

always @(posedge clk) begin
	if(RegWrite) begin
		if(WriteReg != 32'd0)
			registers[WriteReg] <= WriteData;
	end
end
endmodule
