module _3bit_mux_2x1 (R, S, I0, I1);
input S;
input [2:0] I0, I1;
output [2:0] R;

mux_2x1  mux0(R[0], S, I0[0], I1[0]),
			mux1(R[1], S, I0[1], I1[1]),
			mux2(R[2], S, I0[2], I1[2]);
endmodule
