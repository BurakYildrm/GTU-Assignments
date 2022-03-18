`define DELAY 20
module _4bit_xor_testbench(); 
reg [3:0] a, b;
wire [3:0] r;

_4bit_xor _4bx (r, a, b);

initial begin
a = 4'b1010; b = 4'b0101;
#`DELAY;
a = 4'b1111; b = 4'b1111;
#`DELAY;
a = 4'b0000; b = 4'b0000;
#`DELAY;
a = 4'b1111; b = 4'b0000;
end
 
 
initial
begin
$monitor("time = %2d, a = %4b, b = %4b, r = %4b", $time, a, b, r);
end
 
endmodule