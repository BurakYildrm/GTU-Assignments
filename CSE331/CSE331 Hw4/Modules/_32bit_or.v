module _32bit_or (R, A, B);
input [31:0] A,B;
output [31:0] R;

_4bit_or 	or0(R[3:0], A[3:0], B[3:0]);
_4bit_or		or1(R[7:4], A[7:4], B[7:4]);
_4bit_or		or2(R[11:8], A[11:8], B[11:8]);
_4bit_or		or3(R[15:12], A[15:12], B[15:12]);
_4bit_or		or4(R[19:16], A[19:16], B[19:16]);
_4bit_or		or5(R[23:20], A[23:20], B[23:20]);
_4bit_or		or6(R[27:24], A[27:24], B[27:24]);
_4bit_or		or7(R[31:28], A[31:28], B[31:28]);

endmodule
