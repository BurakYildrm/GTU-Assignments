module _32bit_nor (R, A, B);
input [31:0] A,B;
output [31:0] R;
wire [31:0] O;

_32bit_or or0(O, A, B);
_32bit_not not0(R, O);

endmodule
