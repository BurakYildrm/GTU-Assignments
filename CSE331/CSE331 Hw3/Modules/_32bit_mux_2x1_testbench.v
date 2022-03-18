`define DELAY 20
module _32bit_mux_2x1_testbench();
reg [31:0] a, b;
reg s;
wire [31:0] r;

_32bit_mux_2x1 mux0(r, s, a, b);

initial begin
a = 32'h0000_0000; b = 32'h0000_0001; s = 1'b0;
#`DELAY;
a = 32'h0000_0000; b = 32'h0000_0001; s = 1'b1;
end

initial 
begin
$monitor("time = %2d\na = %32b\nb = %32b\ns = %1b\nr = %32b", $time, a, b, s, r);
end
endmodule
