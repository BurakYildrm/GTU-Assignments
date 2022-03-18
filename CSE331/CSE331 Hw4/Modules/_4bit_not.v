module _4bit_not (R, A);
input [3:0] A;
output [3:0] R;
not not0(R[0], A[0]);
not not1(R[1], A[1]);
not not2(R[2], A[2]);
not not3(R[3], A[3]);

endmodule
