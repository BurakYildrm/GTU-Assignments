`define DELAY 20
module _4bit_adder_testbench(); 
reg [3:0] a, b;
reg carry_in;
wire carry_out;
wire [3:0] sum;

_4bit_adder _4ba (sum, carry_out, a, b, carry_in);

initial begin
a = 4'b0000; b = 4'b0000; carry_in = 1'b0;
#`DELAY;
a = 4'b0000; b = 4'b0000; carry_in = 1'b1;
#`DELAY;
a = 4'b0100; b = 4'b0001; carry_in = 1'b0;
#`DELAY;
a = 4'b0100; b = 4'b001; carry_in = 1'b1;
#`DELAY;
a = 4'b0011; b = 4'b0011; carry_in = 1'b0;
#`DELAY;
a = 4'b0011; b = 4'b0011; carry_in = 1'b1;
#`DELAY;
a = 4'b0001; b = 4'b0001; carry_in = 1'b0;
#`DELAY;
a = 4'b0001; b = 4'b0001; carry_in = 1'b1;
end
 
 
initial
begin
$monitor("time = %2d, a = %4b, b = %4b, carry_in = %1b, sum = %4b, carry_out = %1b", $time, a, b, carry_in, sum, carry_out);
end
 
endmodule