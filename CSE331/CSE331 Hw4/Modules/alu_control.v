module alu_control(control, ALUop, func);
input [2:0] ALUop, func;
output [2:0] control;
wire [2:0] ALUop_not, func_not;
wire control2_cond0, control2_cond1, control2_cond2, control2_cond3, control2_cond4,
	  control1_cond0, control1_cond1, control1_cond2, control1_cond3, control1_cond4,
	  control0_cond0, control0_cond1, control0_cond2;
	  
not not0(ALUop_not[2], ALUop[2]),
	 not1(ALUop_not[1], ALUop[1]),
	 not2(ALUop_not[0], ALUop[0]);

not not3(func_not[2], func[2]),
	 not4(func_not[1], func[1]),
	 not5(func_not[0], func[0]);
	
and and0(control2_cond0, ALUop_not[2], ALUop[1]), //beginning of control_2 conditions
	 and1(control2_cond1, ALUop_not[2], ALUop[0]),
	 and2(control2_cond2, ALUop[2], ALUop[0]),
	 and3(control2_cond3, ALUop[2], ALUop[1], func[2]),
	 and4(control2_cond4, ALUop[2], ALUop[1], func_not[1], func_not[0]),
	 and5(control1_cond0, ALUop_not[2], ALUop_not[1], ALUop[0]), //beginning of control_1 conditions
	 and6(control1_cond1, ALUop_not[2], ALUop[1], ALUop_not[0]),
	 and7(control1_cond2, ALUop[2], ALUop_not[1], ALUop_not[0]),
	 and8(control1_cond3, ALUop[2], ALUop[1], func_not[2], func_not[0]),
	 and9(control1_cond4, ALUop[2], ALUop[1], func[2], func[0]),
	 and10(control0_cond0, ALUop_not[2], ALUop[1]), //beginning of control_0 conditions
	 and11(control0_cond1, ALUop[2], ALUop[1], func[2]),
	 and12(control0_cond2, ALUop[2], ALUop[1], func[1], func[0]);
	 
or or0(control[2], control2_cond0, control2_cond1, control2_cond2, control2_cond3, control2_cond4), //control_2
	or1(control[1], control1_cond0, control1_cond1, control1_cond2, control1_cond3, control1_cond4), //control_1
	or2(control[0], control0_cond0, control0_cond1, control0_cond2); //control_0
endmodule
