`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2023 03:37:36 PM
// Design Name: 
// Module Name: mux2to1
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


module mux2to1(
    input s,
    input [7:0] i0,
    input [7:0] i1,
    output [7:0] y
    );
    wire [7:0] x;
    assign x = {8{s}};
    assign y  = (~x & i0) | (x & i1);
endmodule
