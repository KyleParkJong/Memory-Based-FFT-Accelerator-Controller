/******************************************************************************
Copyright (c) 2022 SoC Design Laboratory, Konkuk University, South Korea
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met: redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer;
redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution;
neither the name of the copyright holders nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Authors: Uyong Lee (uyonglee@konkuk.ac.kr)

Revision History
2022.11.17: Started by Uyong Lee
*******************************************************************************/
module FFT
(
    input clk,
    input nrst,
    input [31:0] data_rd_AMEM,
    input [31:0] data_rd_BMEM,
    input [31:0] data_rd_CROM,
    input sel_mem, //mux_mem select signal
    input sel_res, //mux_mem select signal
    input en_REG_A, //REG_A enable signal
    input en_REG_B, //REG_A enable signal
    input en_REG_C, //REG_A enable signal
    output [31:0] out_FFT
);
wire [31:0] out_mux_mem; 
wire [31:0] out_mux_res; 
wire [31:0] out_MULT;
wire [31:0] out_REG_A;
wire [31:0] out_REG_B;
wire [31:0] out_REG_C;
wire [31:0] out0_BF;
wire [31:0] out1_BF;

reg [31:0] REG_A;
reg [31:0] REG_B;
reg [31:0] REG_C;

//===========================Mux
assign out_mux_mem = sel_mem <= 0? data_rd_AMEM : data_rd_BMEM; //if 0 AMEM, else if 1 BMEM
assign out_mux_res = sel_res <= 0? out_REG_C : out1_BF; //if 0 out_REG_C, else if 1 out1_BF

//===========================Module
MULT MULT(
    .in_MULT_re(out_mux_mem[15:0]), .in_MULT_im(out_mux_mem[31:16]), 
    .tw_in_re(data_rd_CROM[15:0]), .tw_in_im(data_rd_CROM[31:16]),
    .out_MULT(out_MULT)
);

BF BF(
    .in_re0(out_REG_A[15:0]),.in_im0(out_REG_A[31:16]),
    .in_re1(out_REG_B[15:0]),.in_im1(out_REG_B[31:16]),
    .out0_BF(out0_BF),.out1_BF(out1_BF)
);


//===========================Register
always @(posedge clk)
begin
   if(!nrst) begin
    REG_A <= 0;
   end
   else if(en_REG_A)begin
    REG_A <= out_mux_mem;
   end
   else begin
    REG_A <= REG_A;
   end
end
always @(posedge clk)
begin
   if(!nrst) begin
    REG_B <= 0;
   end
   else if(en_REG_B)begin
    REG_B <= out_MULT;
   end
   else begin
    REG_B <= REG_B;
   end
end
always @(posedge clk)
begin
   if(!nrst) begin
    REG_C <= 0;
   end
   else if(en_REG_C)begin
    REG_C <= out0_BF;
   end
   else begin
    REG_C <= REG_C;
   end
end

assign out_REG_A = REG_A;
assign out_REG_B = REG_B;
assign out_REG_C = REG_C;

//===========================output
assign out_FFT = out_mux_res;

endmodule