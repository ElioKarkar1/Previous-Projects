`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/19/2023 04:01:24 PM
// Design Name: 
// Module Name: Top_Mod
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Top_Mod(
input clkin,btnR,btnU,btnD,btnC,btnL,
input [15:0] sw,
output [6:0] seg,
output dp,
output [3:0] an,
output [15:0] led
    );
    assign dp = 1'b1;
    assign led[14:1] = 1'b0;
    wire UTC, DTC,clk,UP,DW,digsel,max;
    wire [15:0] N;
    wire [3:0] Q,H;
    EdgeDetector A(.clk(clk),.button(btnU), .val(UP));
    EdgeDetector B(.clk(clk),.button(btnD), .val(DW));
    assign max = ((~&N[15:2])& btnC)|UP;
    
    countUD16L count (.Up(max),.Dw(DW),.clk(clk),.LD(btnL),.Din(sw),.Q(N),.UTC(),.DTC());
    
    RingCounter ring(.inc(digsel), .clk(clk), .S(Q));
    selector select(.N(N),.sel(Q),.H(H));
    labCnt_clks slowit (.clkin(clkin), .greset(btnR), .clk(clk), .digsel(digsel));
    assign led[15] = &N[15:0];
    assign led[0] = ~(|N[15:0]);
    assign an = ~Q;
    hex7seg controller(.n(H),.seg(seg));
endmodule
