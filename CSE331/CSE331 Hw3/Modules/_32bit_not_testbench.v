`define DELAY 20
module _32bit_not_testbench(); 
reg [31:0] a;
wire [31:0] r;

_32bit_not _32bn (r, a);

initial begin
a = 32'hAAAA_AAAA;
#`DELAY;
a = 32'h5555_5555;
#`DELAY;
a = 32'hFFFF_FFFF;
#`DELAY;
a = 32'h0000_0000;
end
 
 
initial
begin
$monitor("time = %2d\na = %32b\nr = %32b", $time, a, r);
end
 
endmodule