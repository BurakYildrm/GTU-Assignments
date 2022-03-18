`define DELAY 20
module _32bit_adder_testbench(); 
reg [31:0] a, b; 
reg carry_in, op;
wire [31:0] sum;
wire carry_out, slt, of;

_32bit_adder _32batb (of, slt, sum, carry_out, a, b, carry_in, op);

initial begin
a = 32'd25;
b = 32'd81;
carry_in = 1'b0;
op = 0;
#`DELAY;
carry_in = 1'b1;
op = 0;
#`DELAY;
carry_in = 1'b1;
op = 1;
#`DELAY;
a = 32'd90;
b = 32'd10;
carry_in = 1'b0;
op = 0;
#`DELAY;
carry_in = 1'b1;
op = 0;
#`DELAY;
carry_in = 1'b1;
op = 1;
#`DELAY;
a = 32'h7FFF_FFFF;
b = 32'd0;
carry_in = 1'b1;
op = 0;
#`DELAY;
a = 32'h8000_0000;
b = 32'd1;
carry_in = 1'b0;
op = 1;
end
 
 
initial
begin
$monitor("time = %2d\na = %32b\nb = %32b\ncarry_in = %1b\nop = %1b\nsum = %32b\ncarry_out = %1b\nslt = %1b\noverflow = %1b\n", 
			$time, a, b, carry_in, op, sum, carry_out, slt, of);
end
 
endmodule
