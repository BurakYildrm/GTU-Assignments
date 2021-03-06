module _32bit_shift_left(R, I, shamt);
input [4:0] shamt;
input [31:0] I;
output [31:0] R;
wire [31:0] layer0, layer1, layer2, layer3, temp0, temp1, temp2, temp3, temp4;

or  or0(temp0[31], I[30], 1'b0),
    or1(temp0[30], I[29], 1'b0),
    or2(temp0[29], I[28], 1'b0),
    or3(temp0[28], I[27], 1'b0),
    or4(temp0[27], I[26], 1'b0),
    or5(temp0[26], I[25], 1'b0),
    or6(temp0[25], I[24], 1'b0),
    or7(temp0[24], I[23], 1'b0),
    or8(temp0[23], I[22], 1'b0),
    or9(temp0[22], I[21], 1'b0),
    or10(temp0[21], I[20], 1'b0),
    or11(temp0[20], I[19], 1'b0),
    or12(temp0[19], I[18], 1'b0),
    or13(temp0[18], I[17], 1'b0),
    or14(temp0[17], I[16], 1'b0),
    or15(temp0[16], I[15], 1'b0),
    or16(temp0[15], I[14], 1'b0),
    or17(temp0[14], I[13], 1'b0),
    or18(temp0[13], I[12], 1'b0),
    or19(temp0[12], I[11], 1'b0),
    or20(temp0[11], I[10], 1'b0),
    or21(temp0[10], I[9], 1'b0),
    or22(temp0[9], I[8], 1'b0),
    or23(temp0[8], I[7], 1'b0),
    or24(temp0[7], I[6], 1'b0),
    or25(temp0[6], I[5], 1'b0),
    or26(temp0[5], I[4], 1'b0),
    or27(temp0[4], I[3], 1'b0),
    or28(temp0[3], I[2], 1'b0),
    or29(temp0[2], I[1], 1'b0),
    or30(temp0[1], I[0], 1'b0),
	 zero0(temp0[0], 1'b0, 1'b0);

_32bit_mux_2x1 mux0(layer0, shamt[0], I, temp0);

or  or31(temp1[31], layer0[29], 1'b0),
    or32(temp1[30], layer0[28], 1'b0),
    or33(temp1[29], layer0[27], 1'b0),
    or34(temp1[28], layer0[26], 1'b0),
    or35(temp1[27], layer0[25], 1'b0),
    or36(temp1[26], layer0[24], 1'b0),
    or37(temp1[25], layer0[23], 1'b0),
    or38(temp1[24], layer0[22], 1'b0),
    or39(temp1[23], layer0[21], 1'b0),
    or40(temp1[22], layer0[20], 1'b0),
    or41(temp1[21], layer0[19], 1'b0),
    or42(temp1[20], layer0[18], 1'b0),
    or43(temp1[19], layer0[17], 1'b0),
    or44(temp1[18], layer0[16], 1'b0),
    or45(temp1[17], layer0[15], 1'b0),
    or46(temp1[16], layer0[14], 1'b0),
    or47(temp1[15], layer0[13], 1'b0),
    or48(temp1[14], layer0[12], 1'b0),
    or49(temp1[13], layer0[11], 1'b0),
    or50(temp1[12], layer0[10], 1'b0),
    or51(temp1[11], layer0[9], 1'b0),
    or52(temp1[10], layer0[8], 1'b0),
    or53(temp1[9], layer0[7], 1'b0),
    or54(temp1[8], layer0[6], 1'b0),
    or55(temp1[7], layer0[5], 1'b0),
    or56(temp1[6], layer0[4], 1'b0),
    or57(temp1[5], layer0[3], 1'b0),
    or58(temp1[4], layer0[2], 1'b0),
    or59(temp1[3], layer0[1], 1'b0),
    or60(temp1[2], layer0[0], 1'b0),
	 zero1(temp1[1], 1'b0, 1'b0),
	 zero2(temp1[0], 1'b0, 1'b0);
	 
_32bit_mux_2x1 mux1(layer1, shamt[1], layer0, temp1);

or or61(temp2[31], layer1[27], 1'b0),
    or62(temp2[30], layer1[26], 1'b0),
    or63(temp2[29], layer1[25], 1'b0),
    or64(temp2[28], layer1[24], 1'b0),
    or65(temp2[27], layer1[23], 1'b0),
    or66(temp2[26], layer1[22], 1'b0),
    or67(temp2[25], layer1[21], 1'b0),
    or68(temp2[24], layer1[20], 1'b0),
    or69(temp2[23], layer1[19], 1'b0),
    or70(temp2[22], layer1[18], 1'b0),
    or71(temp2[21], layer1[17], 1'b0),
    or72(temp2[20], layer1[16], 1'b0),
    or73(temp2[19], layer1[15], 1'b0),
    or74(temp2[18], layer1[14], 1'b0),
    or75(temp2[17], layer1[13], 1'b0),
    or76(temp2[16], layer1[12], 1'b0),
    or77(temp2[15], layer1[11], 1'b0),
    or78(temp2[14], layer1[10], 1'b0),
    or79(temp2[13], layer1[9], 1'b0),
    or80(temp2[12], layer1[8], 1'b0),
    or81(temp2[11], layer1[7], 1'b0),
    or82(temp2[10], layer1[6], 1'b0),
    or83(temp2[9], layer1[5], 1'b0),
    or84(temp2[8], layer1[4], 1'b0),
    or85(temp2[7], layer1[3], 1'b0),
    or86(temp2[6], layer1[2], 1'b0),
    or87(temp2[5], layer1[1], 1'b0),
    or88(temp2[4], layer1[0], 1'b0),
	 zero3(temp2[3], 1'b0, 1'b0),
	 zero4(temp2[2], 1'b0, 1'b0),
	 zero5(temp2[1], 1'b0, 1'b0),
	 zero6(temp2[0], 1'b0, 1'b0);
	 
_32bit_mux_2x1 mux2(layer2, shamt[2], layer1, temp2);

or or89(temp3[31], layer2[23], 1'b0),
    or90(temp3[30], layer2[22], 1'b0),
    or91(temp3[29], layer2[21], 1'b0),
    or92(temp3[28], layer2[20], 1'b0),
    or93(temp3[27], layer2[19], 1'b0),
    or94(temp3[26], layer2[18], 1'b0),
    or95(temp3[25], layer2[17], 1'b0),
    or96(temp3[24], layer2[16], 1'b0),
    or97(temp3[23], layer2[15], 1'b0),
    or98(temp3[22], layer2[14], 1'b0),
    or99(temp3[21], layer2[13], 1'b0),
    or100(temp3[20], layer2[12], 1'b0),
    or101(temp3[19], layer2[11], 1'b0),
    or102(temp3[18], layer2[10], 1'b0),
    or103(temp3[17], layer2[9], 1'b0),
    or104(temp3[16], layer2[8], 1'b0),
    or105(temp3[15], layer2[7], 1'b0),
    or106(temp3[14], layer2[6], 1'b0),
    or107(temp3[13], layer2[5], 1'b0),
    or108(temp3[12], layer2[4], 1'b0),
    or109(temp3[11], layer2[3], 1'b0),
    or110(temp3[10], layer2[2], 1'b0),
    or111(temp3[9], layer2[1], 1'b0),
    or112(temp3[8], layer2[0], 1'b0),
	 zero7(temp3[7], 1'b0, 1'b0),
	 zero8(temp3[6], 1'b0, 1'b0),
	 zero9(temp3[5], 1'b0, 1'b0),
	 zero10(temp3[4], 1'b0, 1'b0),
	 zero11(temp3[3], 1'b0, 1'b0),
	 zero12(temp3[2], 1'b0, 1'b0),
	 zero13(temp3[1], 1'b0, 1'b0),
	 zero14(temp3[0], 1'b0, 1'b0);
	 
_32bit_mux_2x1 mux3(layer3, shamt[3], layer2, temp3);

or or113(temp4[31], layer3[15], 1'b0),
    or114(temp4[30], layer3[14], 1'b0),
    or115(temp4[29], layer3[13], 1'b0),
    or116(temp4[28], layer3[12], 1'b0),
    or117(temp4[27], layer3[11], 1'b0),
    or118(temp4[26], layer3[10], 1'b0),
    or119(temp4[25], layer3[9], 1'b0),
    or120(temp4[24], layer3[8], 1'b0),
    or121(temp4[23], layer3[7], 1'b0),
    or122(temp4[22], layer3[6], 1'b0),
    or123(temp4[21], layer3[5], 1'b0),
    or124(temp4[20], layer3[4], 1'b0),
    or125(temp4[19], layer3[3], 1'b0),
    or126(temp4[18], layer3[2], 1'b0),
    or127(temp4[17], layer3[1], 1'b0),
    or128(temp4[16], layer3[0], 1'b0),
	 zero15(temp4[15], 1'b0, 1'b0),
	 zero16(temp4[14], 1'b0, 1'b0),
	 zero17(temp4[13], 1'b0, 1'b0),
	 zero18(temp4[12], 1'b0, 1'b0),
	 zero19(temp4[11], 1'b0, 1'b0),
	 zero20(temp4[10], 1'b0, 1'b0),
	 zero21(temp4[9], 1'b0, 1'b0),
	 zero22(temp4[8], 1'b0, 1'b0),
	 zero23(temp4[7], 1'b0, 1'b0),
	 zero24(temp4[6], 1'b0, 1'b0),
	 zero25(temp4[5], 1'b0, 1'b0),
	 zero26(temp4[4], 1'b0, 1'b0),
	 zero27(temp4[3], 1'b0, 1'b0),
	 zero28(temp4[2], 1'b0, 1'b0),
	 zero29(temp4[1], 1'b0, 1'b0),
	 zero30(temp4[0], 1'b0, 1'b0);
	 
_32bit_mux_2x1 mux4(R, shamt[4], layer3, temp4);
endmodule
