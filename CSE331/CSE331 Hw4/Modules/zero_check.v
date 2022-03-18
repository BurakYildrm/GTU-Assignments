module zero_check(R, I);
input [31:0] I;
output R;
wire or_res0, or_res1, or_res2, or_res3, or_res4, or_res5, or_res6, or_res7, or_res8, or_res9, or_res10, or_res11, or_res12, or_res13, or_res14,
     or_res15, or_res16, or_res17, or_res18, or_res19, or_res20, or_res21, or_res22, or_res23, or_res24, or_res25, or_res26, or_res27, or_res28, 
	  or_res29, or_res30;

or or30(or_res30, I[31], I[30]),
   or29(or_res29, I[29], I[28]),
   or28(or_res28, I[27], I[26]),
   or27(or_res27, I[25], I[24]),
   or26(or_res26, I[23], I[22]),
   or25(or_res25, I[21], I[20]),
   or24(or_res24, I[19], I[18]),
   or23(or_res23, I[17], I[16]),
   or22(or_res22, I[15], I[14]),
   or21(or_res21, I[13], I[12]),
   or20(or_res20, I[11], I[10]),
   or19(or_res19, I[9], I[8]),
   or18(or_res18, I[7], I[6]),
   or17(or_res17, I[5], I[4]),
   or16(or_res16, I[3], I[2]),
   or15(or_res15, I[1], I[0]),
   or14(or_res14, or_res30, or_res29),
   or13(or_res13, or_res28, or_res27),
   or12(or_res12, or_res26, or_res25),
   or11(or_res11, or_res24, or_res23),
   or10(or_res10, or_res22, or_res21),
   or9(or_res9, or_res20, or_res19),
   or8(or_res8, or_res18, or_res17),
   or7(or_res7, or_res16, or_res15),
   or6(or_res6, or_res14, or_res13),
   or5(or_res5, or_res12, or_res11),
   or4(or_res4, or_res10, or_res9),
   or3(or_res3, or_res8, or_res7),
   or2(or_res2, or_res6, or_res5),
   or1(or_res1, or_res4, or_res3),
   or0(or_res0, or_res2, or_res1);

not not0(R, or_res0);	  
endmodule
