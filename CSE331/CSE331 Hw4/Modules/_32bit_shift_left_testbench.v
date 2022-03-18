`define DELAY 20
module _32bit_shift_left_testbench();
reg [31:0] i;
reg [4:0] shamt;
wire [31:0] r;

_32bit_shift_left sl(r, i, shamt);

initial begin
i = 32'hFFFF_FFFF; shamt = 5'd5;
#`DELAY;
i = 32'hFFFF_FFFF; shamt = 5'd1;
#`DELAY;
i = 32'hFFFF_FFFF; shamt = 5'd10;
#`DELAY;
i = 32'hFFFF_FFFF; shamt = 5'd31;
#`DELAY;
i = 32'hFFFF_FFFF; shamt = 5'd16;
end

initial 
begin
$monitor("time = %2d\ni = %32b\nshamt = %5b\nresult = %32b\n", $time, i, shamt, r);
end
endmodule
