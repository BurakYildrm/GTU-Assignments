module _4bit_xor (R, A, B);
input [3:0] A,B;
output [3:0] R;
xor xor0(R[0], A[0], B[0]);
xor xor1(R[1], A[1], B[1]);
xor xor2(R[2], A[2], B[2]);
xor xor3(R[3], A[3], B[3]);

endmodule
