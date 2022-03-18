`define DELAY 20
module alu_testbench();
reg [31:0] a, b;
reg [2:0] op;
wire [31:0] r;

alu _alu(r, a, b, op);

initial begin
a = 32'd25; b = 32'd15; op = 3'b000;
#`DELAY;
op = 3'b001;
#`DELAY;
op = 3'b010;
#`DELAY;
op = 3'b011;
#`DELAY;
op = 3'b100;
#`DELAY;
op = 3'b101;
#`DELAY;
op = 3'b110;
#`DELAY;
op = 3'b111;
#`DELAY;
a = 32'd30; b = 32'd45; op = 3'b000;
#`DELAY;
op = 3'b001;
#`DELAY;
op = 3'b010;
#`DELAY;
op = 3'b011;
#`DELAY;
op = 3'b100;
#`DELAY;
op = 3'b101;
#`DELAY;
op = 3'b110;
#`DELAY;
op = 3'b111;
#`DELAY;
end

initial 
begin
$monitor("time = %2d\na = %32b\nb = %32b\nop code = %3b\nresult = %32b\n", $time, a, b, op, r);
end
endmodule
