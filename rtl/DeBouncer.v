`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/09/2025 03:02:01 PM
// Design Name: 
// Module Name: DeBouncer
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


module DeBouncer(
input clk,
input rstn,

input BTN,
output INTout
    );
reg [25:0] Count;
always @(posedge clk or negedge rstn)
    if (!rstn) Count <= 26'h3FFFFFF;
     else if (Count != 26'h3FFFFFF) Count <= Count + 1;
     else if (BTN == 1'b1) Count <= 26'h0000000;  
assign INTout = (Count == 26'h0000001) ? 1'b1 : 1'b0;     
endmodule
