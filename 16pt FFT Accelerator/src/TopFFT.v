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
module TopFFT(
    input clk, nrst, in_vld ,out_rdy,
    input [31:0] ext_data_input,
    output in_rdy, out_vld, 
    output [31:0] ext_data_output
);

//Top_FFT wire Declaration//
wire we_AMEM;
wire we_BMEM;
wire we_OMEM;
wire [31:0] data_rd_AMEM;
wire [31:0] data_rd_BMEM;
wire [31:0] data_rd_CROM;
wire [31:0] data_rd_OMEM;
wire [31:0] out_FFTcore;
wire [3:0] addr_AMEM;
wire [3:0] addr_BMEM;
wire [3:0] addr_OMEM;
wire [2:0] addr_CROM;
wire sel_input;

wire [31:0] output_mux_input;

assign output_mux_input = (sel_input) ? ext_data_input : out_FFTcore;
assign ext_data_output = (out_vld) ? data_rd_OMEM : 0 ;


//FFTCore Instantiation//
FFTcore FFTcore(
    .clk(clk),
    .nrst(nrst),
    .in_vld(in_vld),
    .out_rdy(out_rdy),
    .data_rd_AMEM(data_rd_AMEM),
    .data_rd_BMEM(data_rd_BMEM),
    .data_rd_CROM(data_rd_CROM),
    .we_AMEM(we_AMEM),
    .we_BMEM(we_BMEM),
    .we_OMEM(we_OMEM),
    .out_FFT(out_FFTcore),
    .addr_AMEM(addr_AMEM),
    .addr_BMEM(addr_BMEM),
    .addr_OMEM(addr_OMEM),
    .addr_CROM(addr_CROM),
    .sel_input(sel_input),
    .out_vld(out_vld),
    .in_rdy(in_rdy)
);

//Instantiate with the name of the BRAM module you created
blk_mem_gen_0  AMEM(
	.clka(clk),	
	.wea(~we_AMEM),	
	.addra(addr_AMEM),
	.dina(output_mux_input),	
	.douta(data_rd_AMEM)
);

//Instantiate with the name of the BRAM module you created
blk_mem_gen_0  BMEM(	
	.clka(clk),
	.wea(~we_BMEM),
	.addra(addr_BMEM),
	.dina(out_FFTcore),
	.douta(data_rd_BMEM)
);

//Instantiate with the name of the BRAM module you created
blk_mem_gen_0  OMEM(
	.clka(clk),
	.wea(~we_OMEM),
	.addra(addr_OMEM),
	.dina(out_FFTcore),
	.douta(data_rd_OMEM)
);

//Instantiate with the name of the BROM module you created
blk_mem_gen_1 CROM(
    .clka(clk),
	.addra(addr_CROM),
	.douta(data_rd_CROM)
);


endmodule
