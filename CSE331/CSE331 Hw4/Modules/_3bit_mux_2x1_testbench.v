`define DELAY 20
module _3bit_mux_2x1_testbench();
reg [2:0] a, b;
reg s;
wire [2:0] r;

_3bit_mux_2x1 mux0(r, s, a, b);

initial begin
a = 3'b000; b = 3'b001; s = 1'b0;
#`DELAY;
a = 3'b000; b = 3'b001; s = 1'b1;
end

initial 
begin
$monitor("time = %2d\na = %3b\nb = %3b\ns = %1b\nr = %3b", $time, a, b, s, r);
end
endmodule
