module MiniMIPS(NewCounter, Counter, clk);
input [31:0] Counter;
input clk;
output [31:0] NewCounter;
wire [2:0] ALUop, WriteReg, Control;
wire [15:0] Instruction;
wire [31:0] ReadData1, ReadData2, WriteData, Extended, ALUArg2, Shifted, ALURes, TempCounter, BranchCounter, ReadData;
wire RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch, IsZero, SLT0, SLT1, OF0, OF1, COut0, COut1, IsBranch, BranchCond, BranchSelector;

instruction_memory im(Instruction, Counter, clk);
_32bit_adder adder0(OF0, SLT0, TempCounter, COut0, Counter, 32'd1, 0, 0);
main_control mc(RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch, ALUop, Instruction[15:12]);
_3bit_mux_2x1 mux0(WriteReg, RegDst, Instruction[8:6], Instruction[5:3]);
register_block rb(ReadData1, ReadData2, Instruction[11:9], Instruction[8:6], WriteReg, WriteData, RegWrite, clk);
sign_extend se(Extended, Instruction[5:0]);
_32bit_mux_2x1 mux1(ALUArg2, ALUSrc, ReadData2, Extended);
_32bit_shift_left sl(Shifted, Extended, 5'd0); //no shift needed because instructions are in a queue based on their line number but this can be changed in the future depending on the implementation
_32bit_adder adder1(OF1, SLT1, BranchCounter, COut1, TempCounter, Shifted, 0, 0);
alu_control ac(Control, ALUop, Instruction[2:0]);
alu _alu(ALURes, ReadData1, ALUArg2, Control);
zero_check zc(IsZero, ALURes);
xor (BranchCond, IsZero, Instruction[13]);
and (BranchSelector, Branch, BranchCond);
_32bit_mux_2x1 mux2(NewCounter, BranchSelector, TempCounter, BranchCounter);
data_memory dm(ReadData, ALURes, ReadData2, MemWrite, clk);
_32bit_mux_2x1 mux3(WriteData, MemtoReg, ALURes, ReadData);
endmodule
