`define DELAY 20
module register_block_testbench();
reg RegWrite, clk;
reg [2:0] ReadReg1, ReadReg2, WriteReg;
reg [31:0] WriteData;
wire [31:0] ReadData1, ReadData2;
  
register_block rb(ReadData1, ReadData2, ReadReg1, ReadReg2, WriteReg, WriteData, RegWrite, clk);

initial begin
$readmemb("registers.mem",rb.registers);
clk = 1;
RegWrite = 0;
ReadReg1 = 3'd0;
ReadReg2 = 3'd1;
WriteReg = 3'd0;
WriteData = 32'd30;
end
  
always begin 
#10 clk= ~clk;
end

initial begin
#`DELAY;
ReadReg1 = 3'd2;
ReadReg2 = 3'd3;
#`DELAY;
ReadReg1 = 3'd4;
ReadReg2 = 3'd5;
#`DELAY;
RegWrite = 1;
ReadReg1 = 3'd6;
ReadReg2 = 3'd7;
#`DELAY;
WriteReg = 3'd1;
#`DELAY;
$writememb("registers_updated.mem",rb.registers);
$stop;
end
 
initial begin
$monitor("time = %2d\nReadReg1 = %3b\nReadReg2 = %3b\nReadData1 = %32b\nReadData2 = %32b\nRegWrite = %1b\nWriteReg = %3b\nWriteData = %32b\n", $time, ReadReg1, ReadReg2, ReadData1, ReadData2, RegWrite, WriteReg, WriteData);
end
endmodule
