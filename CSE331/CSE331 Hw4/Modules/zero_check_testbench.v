`define DELAY 20
module zero_check_testbench();
reg [31:0] I;
wire R;
  
zero_check zc(R, I);

initial begin
I = 32'd0;
#`DELAY;
I = 32'd1;
#`DELAY;
I = 32'h8FFF_FFFF;
#`DELAY;
I = 32'hFFFF_FFFF;
end
 
initial begin
$monitor("time = %2d\nI = %32b\nIs zero = %1b\n", $time, I, R);
end
endmodule
