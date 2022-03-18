`define DELAY 20
module _4bit_not_testbench(); 
reg [3:0] a;
wire [3:0] r;

_4bit_not _4bn (r, a);

initial begin
a = 4'b1010;
#`DELAY;
a = 4'b0101;
#`DELAY;
a = 4'b1111;
#`DELAY;
a = 4'b0000;
end
 
 
initial
begin
$monitor("time = %2d, a = %4b, r = %4b", $time, a, r);
end
 
endmodule