`define DELAY 20
module data_memory_testbench();
reg MemWrite, clk;
reg [31:0] WriteData, Address;
wire [31:0] ReadData;
  
data_memory dm(ReadData, Address, WriteData, MemWrite, clk);

initial begin
$readmemb("data.mem",dm.data);
clk = 1;
MemWrite = 0;
Address = 32'd0;
WriteData = 32'd20;
end
  
always begin 
#10 clk= ~clk;
end

initial begin
#`DELAY;
Address = 32'd1;
#`DELAY;
Address = 32'd2;
#`DELAY;
Address = 32'd3;
#`DELAY;
Address = 32'd4;
#`DELAY;
Address = 32'd5;
#`DELAY;
Address = 32'd6;
#`DELAY;
Address = 32'd7;
#`DELAY;
MemWrite = 1;
Address = 32'd8;
#`DELAY;
$writememb("data_updated.mem",dm.data);
$stop;
end
 
initial begin
$monitor("time = %2d\nAddress = %32b\nReadData = %32b\nMemWrite = %1b\nWriteData = %32b\n", $time, Address, ReadData, MemWrite, WriteData);
end
endmodule
