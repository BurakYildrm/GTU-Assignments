`define DELAY 20
module main_control_testbench();
reg [3:0] opcode;
wire [2:0] ALUop;
wire RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch;

main_control mc(RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch, ALUop, opcode);

initial begin
opcode = 4'b0000;
#`DELAY;
opcode = 4'b0001;
#`DELAY;
opcode = 4'b0010;
#`DELAY;
opcode = 4'b0011;
#`DELAY;
opcode = 4'b0100;
#`DELAY;
opcode = 4'b0101;
#`DELAY;
opcode = 4'b0110;
#`DELAY;
opcode = 4'b0111;
#`DELAY;
opcode = 4'b1000;
#`DELAY;
opcode = 4'b1001;
end

initial 
begin
$monitor("time = %2d\nopcode = %4b\nRegWrite = %1b\nALUSrc = %1b\nRegDst = %1b\nMemtoReg = %1b\nMemRead = %1b\nMemWrite = %1b\nBranch = %1b\nALUop = %3b\n", $time, opcode, RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch, ALUop);
end
endmodule
