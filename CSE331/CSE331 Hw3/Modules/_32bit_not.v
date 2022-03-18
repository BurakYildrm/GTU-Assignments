module _32bit_not (R, A);
input [31:0] A;
output [31:0] R;

_4bit_not 	not0(R[3:0], A[3:0]);
_4bit_not	not1(R[7:4], A[7:4]);
_4bit_not	not2(R[11:8], A[11:8]);
_4bit_not	not3(R[15:12], A[15:12]);
_4bit_not	not4(R[19:16], A[19:16]);
_4bit_not	not5(R[23:20], A[23:20]);
_4bit_not	not6(R[27:24], A[27:24]);
_4bit_not	not7(R[31:28], A[31:28]);

endmodule
