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
module FFTcore(
    input clk,nrst,in_vld, out_rdy,
    input [31:0] data_rd_AMEM,
    input [31:0] data_rd_BMEM,
    input [31:0] data_rd_CROM,
    output we_AMEM,
    output we_BMEM,
    output we_OMEM,
    output [31:0] out_FFT,
    output [3:0] addr_AMEM,
    output [3:0] addr_BMEM,
    output [3:0] addr_OMEM,
    output [2:0] addr_CROM,
    output sel_input,
    output out_vld,
    output in_rdy
    );
    
//wire Declaration//
wire en_REG_A;
wire en_REG_B;
wire en_REG_C;
wire sel_res;
wire sel_mem;
wire flag_out_vld;


//FFT Initiation//
FFT FFT(
	.clk(clk),
	.nrst(nrst),
	.data_rd_AMEM(data_rd_AMEM),
	.data_rd_BMEM(data_rd_BMEM),
	.data_rd_CROM(data_rd_CROM),
	.sel_mem(sel_mem),
	.sel_res(sel_res),
	.en_REG_A(en_REG_A),
	.en_REG_B(en_REG_B),
	.en_REG_C(en_REG_C),
	.out_FFT(out_FFT)
);


//Controller Initiation//
controller controller(
    .clk(clk),
    .nrst(nrst),
    .in_vld(in_vld),
    .out_rdy(out_rdy),
    .in_rdy(in_rdy),
    .out_vld(out_vld),
    .sel_input(sel_input),
    .sel_res(sel_res),
    .sel_mem(sel_mem),	
    .we_AMEM(we_AMEM),
    .we_BMEM(we_BMEM),
    .we_OMEM(we_OMEM),
    .addr_AMEM(addr_AMEM),
    .addr_BMEM(addr_BMEM),
    .addr_OMEM(addr_OMEM),
    .addr_CROM(addr_CROM),
    .en_REG_A(en_REG_A),
    .en_REG_B(en_REG_B),
    .en_REG_C(en_REG_C)
);

endmodule
