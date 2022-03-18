module alu (R, A, B, OP);
input [31:0] A,B;
input [2:0] OP;
output [31:0] R;
wire NTOP0, NTOP1, NTOP2, SLT, CARRY_IN, C, OF;
wire [31:0] R_ADDER, R_XOR, R_SLT, R_NOR, R_AND, R_OR;

or is_sub(CARRY_IN, OP[1], OP[2]);
_32bit_adder adder(OF, SLT, R_ADDER, C, A, B, CARRY_IN, CARRY_IN);
_32bit_xor _xor(R_XOR, A, B);
_32bit_nor _nor(R_NOR, A, B);
_32bit_and _and(R_AND, A, B);
_32bit_or _or(R_OR, A, B);
_32bit_mux_2x1 slt_mux(R_SLT, SLT, 32'h0000_0000, 32'h0000_0001); //makes slt 32 bit
_32bit_mux_8x1 choice(R, OP[2], OP[1], OP[0], R_ADDER, R_XOR, R_ADDER, 32'd0, R_SLT, R_NOR, R_AND, R_OR);
endmodule 