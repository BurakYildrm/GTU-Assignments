module main_control(RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch, ALUop, op);
input [3:0] op;
output [2:0] ALUop;
output RegWrite, ALUSrc, RegDst, MemtoReg, MemRead, MemWrite, Branch;
wire [3:0] op_not;
wire R_type, addi, andi, ori, nori, beq, bne, slti, lw, sw;

not not0(op_not[3], op[3]),
	 not1(op_not[2], op[2]),
	 not2(op_not[1], op[1]),
	 not3(op_not[0], op[0]);
	 
and and0(R_type, op_not[3], op_not[2], op_not[1], op_not[0]),
	 and1(addi, op_not[3], op_not[2], op_not[1], op[0]),
	 and2(andi, op_not[3], op_not[2], op[1], op_not[0]),
	 and3(ori, op_not[3], op_not[2], op[1], op[0]),
	 and4(nori, op_not[3], op[2], op_not[1], op_not[0]),
	 and5(beq, op_not[3], op[2], op_not[1], op[0]),
	 and6(bne, op_not[3], op[2], op[1], op_not[0]),
	 and7(slti, op_not[3], op[2], op[1], op[0]),
	 and8(lw, op[3], op_not[2], op_not[1], op_not[0]),
	 and9(sw, op[3], op_not[2], op_not[1], op[0]);

or or0(RegDst, R_type, 1'b0),
	or1(ALUSrc, addi, andi, ori, nori, slti, lw, sw),
	or2(MemtoReg, lw, 1'b0),
	or3(RegWrite, R_type, addi, andi, ori, nori, slti, lw),
	or4(MemRead, lw, 1'b0),
	or5(MemWrite, sw, 1'b0),
	or6(Branch, beq, bne),
	or7(ALUop[2], R_type, beq, bne, slti),
	or8(ALUop[1], R_type, ori, nori),
	or9(ALUop[0], andi, nori, slti);
endmodule
