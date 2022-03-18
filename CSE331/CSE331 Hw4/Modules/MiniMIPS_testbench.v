`define DELAY 10
module MiniMIPS_testbench();
reg clk;
reg [31:0] Counter;
wire [31:0] NewCounter;
  
MiniMIPS mm(NewCounter, Counter, clk);

initial begin
	$readmemb("registers.mem",mm.rb.registers);
	$readmemb("data.mem",mm.dm.data);
	$readmemb("instructions.mem",mm.im.instructions);
	clk = 1;
	Counter = 32'd0;
end
  
always begin 
	#5 clk= ~clk;
end

always begin
	#`DELAY;
	Counter = NewCounter;
	if(Counter > 32'd29) begin
		#`DELAY;
		$writememb("registers_updated.mem",mm.rb.registers);
		$writememb("data_updated.mem",mm.dm.data);
		$stop;
	end
end
 
initial begin
	$monitor("time = %2d\nCounter = %2d\nInstruction = %16b\nReadData1 = %32b\nReadData2 = %32b\nResult = %32b\nNewCounter = %2d\n", $time, Counter, mm.Instruction, mm.ReadData1, mm.ReadData2, mm.ALURes, NewCounter);
end
endmodule
