`define DELAY 20
module alu_control_testbench();
reg [2:0] ALUop, func;
wire [2:0] control;

alu_control ac(control, ALUop, func);

initial begin
ALUop = 3'b000;
func = 3'b000;
#`DELAY;
ALUop = 3'b001;
#`DELAY;
ALUop = 3'b010;
#`DELAY;
ALUop = 3'b011;
#`DELAY;
ALUop = 3'b100;
#`DELAY;
ALUop = 3'b101;
#`DELAY;
ALUop = 3'b110;
func = 3'b000;
#`DELAY;
ALUop = 3'b110;
func = 3'b001;
#`DELAY;
ALUop = 3'b110;
func = 3'b010;
#`DELAY;
ALUop = 3'b110;
func = 3'b011;
#`DELAY;
ALUop = 3'b110;
func = 3'b100;
#`DELAY;
ALUop = 3'b110;
func = 3'b101;
end

initial 
begin
$monitor("time = %2d\nALUop = %3b\nfunc = %3b\ncontrol = %3b\n", $time, ALUop, func, control);
end
endmodule
