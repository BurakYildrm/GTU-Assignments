`define DELAY 20
module _32bit_mux_8x1_testbench();
reg [31:0] i0, i1, i2, i3, i4, i5, i6, i7;
reg s2, s1, s0;
wire [31:0] r;

_32bit_mux_8x1 mux0(r, s2, s1, s0, i0, i1, i2, i3, i4, i5, i6, i7);

initial begin
i0 = 32'h0000_0000;
i1 = 32'h0000_0001;
i2 = 32'h0000_0002;
i3 = 32'h0000_0003;
i4 = 32'h0000_0004;
i5 = 32'h0000_0005;
i6 = 32'h0000_0006;
i7 = 32'h0000_0007;
s2 = 1'b0;
s1 = 1'b0;
s0 = 1'b0;
#`DELAY;
s2 = 1'b0;
s1 = 1'b0;
s0 = 1'b1;
#`DELAY;
s2 = 1'b0;
s1 = 1'b1;
s0 = 1'b0;
#`DELAY;
s2 = 1'b0;
s1 = 1'b1;
s0 = 1'b1;
#`DELAY;
s2 = 1'b1;
s1 = 1'b0;
s0 = 1'b0;
#`DELAY;
s2 = 1'b1;
s1 = 1'b0;
s0 = 1'b1;
#`DELAY;
s2 = 1'b1;
s1 = 1'b1;
s0 = 1'b0;
#`DELAY;
s2 = 1'b1;
s1 = 1'b1;
s0 = 1'b1;
end

initial 
begin
$monitor("time = %2d\ni0 = %32b\ni1 = %32b\ni2 = %32b\ni3 = %32b\ni4 = %32b\ni5 = %32b\ni6 = %32b\ni7 = %32b\ns2 = %1b\ns1 = %1b\ns0 = %1b\nr = %32b\n",
			$time, i0, i1, i2, i3, i4, i5, i6, i7, s2, s1, s0, r);
end
endmodule
