`define DELAY 20
module sign_extend_testbench();
reg [5:0] I;
wire [31:0] R;
  
sign_extend se(R, I);

initial begin
I=6'b000001;
#`DELAY;
I=6'b101010;
#`DELAY;
I=6'b111111;
#`DELAY;
I=6'b000000;
#`DELAY;
I=6'b100000;
end
 
initial begin
$monitor("time = %2d\ninitial=%6b\nextended = %32b\n", $time, I, R);
end
endmodule
