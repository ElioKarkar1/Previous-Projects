`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2023 03:53:05 PM
// Design Name: 
// Module Name: FA
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

module FA(
    input x,y,Cin,
    output Cout, S
);
wire w1, w2, w3;

HA HA1 (.a(x),.b(y),.C(w1),.s(w2));
HA HA2 (.a(w2),.b(Cin),.C(w3),.s(S));
assign Cout = w1|w3;    
endmodule
