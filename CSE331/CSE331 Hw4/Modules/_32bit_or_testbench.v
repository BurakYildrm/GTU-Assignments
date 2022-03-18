`define DELAY 20
module _32bit_or_testbench(); 
reg [31:0] a, b;
wire [31:0] r;

_32bit_or _32bo (r, a, b);

initial begin
a = 32'hAAAA_AAAA; b = 32'h5555_5555;
#`DELAY;
a = 32'hFFFF_FFFF; b = 32'hFFFF_FFFF;
#`DELAY;
a = 32'h0000_0000; b = 32'h0000_0000;
#`DELAY;
a = 32'hFFFF_FFFF; b = 32'h0000_0000;
end
 
 
initial
begin
$monitor("time = %2d\na = %32b\nb = %32b\nr = %32b\n", $time, a, b, r);
end
 
endmodule