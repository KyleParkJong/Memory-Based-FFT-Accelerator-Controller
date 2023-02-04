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

Authors: Jooho Wang (joohowang@konkuk.ac.kr)

Revision History
2018.11.13: Started by Jooho Wang
2022.11.17: Edited by Uyong Lee
*******************************************************************************/


module axis_fft
(
   ///////////////////////////////////////////////////////////////////////////////
   // Port Declarations
   ///////////////////////////////////////////////////////////////////////////////
   // System Signals
   ///////////////////////////////////////////////////////////////////////////////
   input                           s_axis_aresetn,
   input                           m_axis_aresetn,

   // Slave side (FFT Input)
   input                           s_axis_aclk,
   input                           s_axis_tvalid, //in_vld
   output                          s_axis_tready, //in_rdy *
   input signed [32-1:0]   		  s_axis_tdata,
   input                           s_axis_tlast,
   input        [32/8-1:0] 		  s_axis_tkeep,

    // Master side (FFT Output)
   input                           m_axis_aclk,
   output                          m_axis_tvalid, //out_vld *
   input                           m_axis_tready, //out_rdy
   output signed [32-1:0]   		  m_axis_tdata,
   output                          m_axis_tlast,
   output        [32/8-1:0] 		  m_axis_tkeep
);

// Internal signals
wire in_rdy, out_vld;
reg [11:0] cnt_tlast;

///////////////////////////////////////////////////////
//FFT: start instantiation
///////////////////////////////////////////////////////

TopFFT  TopFFT(
   .nrst(s_axis_aresetn),
   .clk(s_axis_aclk),
   .in_vld(s_axis_tvalid),
   .out_rdy(m_axis_tready),
   .ext_data_input(s_axis_tdata),
   .in_rdy(in_rdy),
   .out_vld(out_vld),
   .ext_data_output(m_axis_tdata)
   );

///////////////////////////////////////////////////////
//FFT: end instantiation
///////////////////////////////////////////////////////

always @(posedge s_axis_aclk) begin
    if(!s_axis_aresetn) begin
        cnt_tlast <= 0;
    end
    else if(m_axis_tready && out_vld) begin     // m_axis_tready : DMA -> FFT, output 값을 받을 준비가 됐다. (out_rdy)
        if(cnt_tlast == 4095) cnt_tlast <= 0;
        else cnt_tlast <= cnt_tlast + 1;
    end
    else cnt_tlast <= cnt_tlast;  

end

assign m_axis_tlast = (cnt_tlast==4095) ? 1'b1 : 1'b0 ;

assign m_axis_tvalid = out_vld;
assign s_axis_tready = in_rdy;
assign m_axis_tkeep  = 4'b1111;

endmodule