`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/19/2023 04:48:44 PM
// Design Name: 
// Module Name: RingCounter
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


module RingCounter(
input inc, clk,
output [3:0] S

    );
        wire [3:0] Q;
        assign S[3] = Q[3];
        assign S[2] = Q[2];
        assign S[1] = Q[1];
        assign S[0] = ~Q[0];
        FDRE #(.INIT(1'b0) ) A(.C(clk), .R(1'b0), .CE(inc), .D(~Q[3]), .Q(Q[0]));
        FDRE #(.INIT(1'b0) ) B (.C(clk), .R(1'b0), .CE(inc), .D(~Q[0]), .Q(Q[1]));
        FDRE #(.INIT(1'b0) ) C (.C(clk), .R(1'b0), .CE(inc), .D(Q[1]), .Q(Q[2]));
        FDRE #(.INIT(1'b0) ) F (.C(clk), .R(1'b0), .CE(inc), .D(Q[2]), .Q(Q[3]));
endmodule
