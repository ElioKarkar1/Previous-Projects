`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/19/2023 05:42:40 PM
// Design Name: 
// Module Name: EdgeDetector
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


module EdgeDetector(
input clk, button,
output val
    );
    wire [1:0] Q;
    FDRE #(.INIT(1'b0) ) A(.C(clk), .R(1'b0), .CE(1'b1), .D(button), .Q(Q[0]));
    FDRE #(.INIT(1'b0) ) B(.C(clk), .R(1'b0), .CE(1'b1), .D(Q[0]), .Q(Q[1]));
assign val = ~Q[1] & Q[0];
endmodule
