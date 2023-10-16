`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2023 03:32:53 PM
// Design Name: 
// Module Name: top_mux
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


module top_mux(
    input [15:0] sw,
    input btnU, btnR, clkin,
    output [6:0] seg, 
    output dp, 
    output [3:0] an, 
    output [15:0] led
    );
    
    wire dig_sel;
    wire [7:0] res;
    wire[6:0] x;
    wire[6:0] y;
    mux2to1 mux(.i0(y), .i1(x),.s(dig_sel),.y(seg));
    SignChanger signchanger(.a(sw[7:0]),.sign(btnU),.d(res),.ovfl(dp));
    hex7seg upperbits(.n(res[7:4]), .seg(y));
    hex7seg lowerbits(.n(res[3:0]),.seg(x));
    lab2_digsel clock (.clkin(clkin), .greset(btnR), .digsel(dig_sel));
    assign led = sw;
    assign an[0] = ~dig_sel;
    assign an[1] = dig_sel;
    assign an[2] = 1'b1;
    assign an[3] = 1'b1;
endmodule
