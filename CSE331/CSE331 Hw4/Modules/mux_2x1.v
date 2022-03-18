module mux_2x1 (R, S, I0, I1);
input S, I0, I1;
output R;
wire A0, A1, NT;

not not0(NT, S);
and and0(A0, NT, I0);
and and1(A1, S, I1);
or or0(R, A0, A1);
endmodule
