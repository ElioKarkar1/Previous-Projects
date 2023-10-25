`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/17/2023 03:21:52 PM
// Design Name: 
// Module Name: countUD4L
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


module countUD4L(
input clk, Up, Dw, LD,
input [3:0] Din,
output [3:0] Q,
output UTC, DTC

    );
wire enable;
assign enable = LD |(Up^Dw);
wire [3:0] X, D, Dor;
assign D[0] = ((Up)^Q[0])&~LD&~Dw;
assign D[1] = (((Up & Q[0])^Q[1]))&~LD&~Dw;
assign D[2] = ((Up & Q[1] &Q[0])^Q[2])&~LD;
assign D[3] = ((Up & Q[2] & Q[1] & Q[0])^Q[3])&~LD&~Dw;

assign X[0] = ((Dw)^Q[0])&~LD&~Up;
assign X[1] = (((Dw & ~Q[0])^Q[1]))&~LD&~Up;
assign X[2] = ((Dw & ~Q[1] & ~Q[0])^Q[2])&~LD&~Up;
assign X[3] = ((Dw & ~Q[2] & ~Q[1] & ~Q[0])^Q[3])&~LD&~Up;
assign Dor[0] = (LD & Din[0])|(D[0]&Up)|(X[0]&Dw);
assign Dor[1] = (LD & Din[1])|(D[1]&Up)|(X[1]&Dw);
assign Dor[2] = (LD & Din[2])|(D[2]&Up)|(X[2]&Dw);
assign Dor[3] = (LD & Din[3])|(D[3]&Up)|(X[3]&Dw);
assign UTC = &(Q);
assign DTC = ~(|(Q));
        FDRE #(.INIT(1'b0) ) A(.C(clk), .R(1'b0), .CE(enable), .D(Dor[0]), .Q(Q[0]));
        FDRE #(.INIT(1'b0) ) B (.C(clk), .R(1'b0), .CE(enable), .D(Dor[1]), .Q(Q[1]));
        FDRE #(.INIT(1'b0) ) C (.C(clk), .R(1'b0), .CE(enable), .D(Dor[2]), .Q(Q[2]));
        FDRE #(.INIT(1'b0) ) E (.C(clk), .R(1'b0), .CE(enable), .D(Dor[3]), .Q(Q[3]));

endmodule

