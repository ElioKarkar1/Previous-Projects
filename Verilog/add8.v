`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2023 03:45:53 PM
// Design Name: 
// Module Name: add8
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


module add8(
    input [7:0] A, [7:0] B,
    input Cin,
    output [7:0] S, 
    output Cout, ovfl
    );
    wire [7:0] carryout;
    FA Fa0 (.x(A[0]),.y(B[0]),.Cin(1'b0),.S(S[0]),.Cout(carryout[0]));
    FA Fa1 (.x(A[1]),.y(B[1]),.Cin(carryout[0]),.S(S[1]),.Cout(carryout[1]));
    FA Fa2 (.x(A[2]),.y(B[2]),.Cin(carryout[1]),.S(S[2]),.Cout(carryout[2]));
    FA Fa3 (.x(A[3]),.y(B[3]),.Cin(carryout[2]),.S(S[3]),.Cout(carryout[3]));
    FA Fa4 (.x(A[4]),.y(B[4]),.Cin(carryout[3]),.S(S[4]),.Cout(carryout[4]));
    FA Fa5 (.x(A[5]),.y(B[5]),.Cin(carryout[4]),.S(S[5]),.Cout(carryout[5]));
    FA Fa6 (.x(A[6]),.y(B[6]),.Cin(carryout[5]),.S(S[6]),.Cout(carryout[6]));
    FA Fa7 (.x(A[7]),.y(B[7]),.Cin(carryout[6]),.S(S[7]),.Cout(Cout));
    //assign S = {S[7], S[6], S[5], S[4], S[3], S[2], S[1], S[0]};
    assign ovfl = ((~A[7]&~B[7]&S[7])|(A[7]&B[7]&~S[7]));
endmodule
