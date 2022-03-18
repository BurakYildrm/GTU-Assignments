`define DELAY 20
module instruction_memory_testbench();
reg [31:0] address;
reg clk;
wire [15:0] instruction;
  
instruction_memory im(instruction, address, clk);

initial begin
$readmemb("instructions.mem",im.instructions);
clk = 1;
address = 32'd0;
end
  
always begin 
#10 clk = ~clk;
end

initial begin
#`DELAY;
address = 32'd1;
#`DELAY;
address = 32'd2;
#`DELAY;
address = 32'd3;
#`DELAY;
address = 32'd4;
#`DELAY;
address = 32'd5;
#`DELAY;
$stop;
end
 
initial begin
$monitor("time = %2d\ninstruction = %16b\n", $time, instruction);
end
endmodule
