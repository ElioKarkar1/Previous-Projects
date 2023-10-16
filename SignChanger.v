`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/11/2023 12:08:53 AM
// Design Name: 
// Module Name: SignChanger
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


module SignChanger(
    input [7:0] a,
    input sign,
    output [7:0] d, 
    output ovfl
    );
    wire [7:0] ovfltest; 
    wire [7:0] inv;
    wire [7:0] r;
    wire [7:0] res;
    assign ovfltest = {8'b11111111};
    assign r = {8'b00000001};
    assign inv = (ovfltest & ~a) | (~ovfltest & a);
    assign ovfl = ~((a[7] & ~|(a[6:0])&sign));
    //HA ha(.a(inv), .b(r),.s(sign), .C(1'b0));
    add8 add(.A(a ^ {8{sign}}), .B({7'b0,sign}),.S(res),.Cin(1'b0), .Cout());
    mux2to1 v (.i0(a), .i1(res),.s(sign),.y(d));
    
endmodule
