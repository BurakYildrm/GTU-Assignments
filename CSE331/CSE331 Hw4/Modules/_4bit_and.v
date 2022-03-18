module _4bit_and (R, A, B);
input [3:0] A,B;
output [3:0] R;
and and0(R[0], A[0], B[0]);
and and1(R[1], A[1], B[1]);
and and2(R[2], A[2], B[2]);
and and3(R[3], A[3], B[3]);

endmodule
