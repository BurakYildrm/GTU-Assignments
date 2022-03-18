module _32bit_adder (OF,SLT,S,C,A,B,C0,OP);
input [31:0] A,B;
input C0, OP;
output C, SLT, OF; //in order slt to work properly, carry in and op must be 1. i.e., adder must perform a subtraction operation.
output [31:0] S;
wire C4,C8,C12,C16,C20,C24,C28,C29,C30,C31;
wire [31:0] BMUX, NB;

//Negate input B if OP is 1
_32bit_not not0(NB, B);
_32bit_mux_2x1 mux0(BMUX, OP, B, NB); //op decides if adder whether perfom an add operation or a subtraction operation.
//Instantiate the 4 bit adder
_4bit_adder _4BA0(S[3:0], C4, A[3:0], BMUX[3:0], C0);
_4bit_adder	_4BA1(S[7:4], C8, A[7:4], BMUX[7:4], C4);
_4bit_adder	_4BA2(S[11:8], C12, A[11:8], BMUX[11:8], C8);
_4bit_adder	_4BA3(S[15:12], C16, A[15:12], BMUX[15:12], C12);
_4bit_adder	_4BA4(S[19:16], C20, A[19:16], BMUX[19:16], C16);
_4bit_adder	_4BA5(S[23:20], C24, A[23:20], BMUX[23:20], C20);
_4bit_adder	_4BA6(S[27:24], C28, A[27:24], BMUX[27:24], C24);
//Instantiate the full adder
full_adder FA0(S[28],C29,A[28],BMUX[28],C28);
full_adder FA1(S[29],C30,A[29],BMUX[29],C29);
full_adder FA2(S[30],C31,A[30],BMUX[30],C30);
full_adder FA3(S[31],C,A[31],BMUX[31],C31);
//overflow
xor overflow(OF, C, C31);
//set less than
xor set_less_than(SLT, OF, S[31]);
endmodule
 