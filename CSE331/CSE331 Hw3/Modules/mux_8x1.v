module mux_8x1 (R, S2, S1, S0, I0, I1, I2, I3, I4, I5, I6, I7);
input S2, S1, S0, I0, I1, I2, I3, I4, I5, I6, I7;
output R;
wire M0, M1;

mux_4x1 mux0(M0, S1, S0, I0, I1, I2, I3);
mux_4x1 mux1(M1, S1, S0, I4, I5, I6, I7);
mux_2x1 mux2(R, S2, M0, M1);
endmodule
