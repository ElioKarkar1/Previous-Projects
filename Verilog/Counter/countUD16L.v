`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/17/2023 05:21:30 PM
// Design Name: 
// Module Name: countUD16L
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


module countUD16L(
input Up,Dw,clk,LD,
input [15:0] Din,
output [15:0] Q,
output [3:0] UTC, DTC


    );

countUD4L count1(.Up(Up),.Dw(Dw),.clk(clk),.LD(LD),.Din(Din[3:0]),.Q(Q[3:0]),.UTC(UTC[0]),.DTC(DTC[0]));
countUD4L count2(.Up(Up&UTC[0]),.Dw(Dw&DTC[0]),.clk(clk),.LD(LD),.Din(Din[7:4]),.Q(Q[7:4]),.UTC(UTC[1]),.DTC(DTC[1]));
countUD4L count3(.Up(Up&UTC[1]&UTC[0]),.Dw(Dw&DTC[1]&DTC[0]),.clk(clk),.LD(LD),.Din(Din[11:8]),.Q(Q[11:8]),.UTC(UTC[2]),.DTC(DTC[2]));
countUD4L count4(.Up(Up&UTC[2]&UTC[1]&UTC[0]),.Dw(Dw&DTC[2]&DTC[1]&DTC[0]),.clk(clk),.LD(LD),.Din(Din[15:12]),.Q(Q[15:12]),.UTC(UTC[3]),.DTC(DTC[3]));


endmodule
