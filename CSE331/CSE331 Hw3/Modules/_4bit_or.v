module _4bit_or (R, A, B);
input [3:0] A,B;
output [3:0] R;
or or0(R[0], A[0], B[0]);
or or1(R[1], A[1], B[1]);
or or2(R[2], A[2], B[2]);
or or3(R[3], A[3], B[3]);

endmodule
