module mux_4x1 (R, S1, S0, I0, I1, I2, I3);
input S1, S0, I0, I1, I2, I3;
output R;
wire M0, M1;

mux_2x1 mux0(M0, S0, I0, I1);
mux_2x1 mux1(M1, S0, I2, I3);
mux_2x1 mux2(R, S1, M0, M1);
endmodule
