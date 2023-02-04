/******************************************************************************
Copyright (c) 2017 SoC Design Laboratory, Konkuk University, South Korea
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

Authors: Yungyu Gim (yunkyu933@gmail.com)

Revision History
2017.09.11: Started by Yungyu Gim

*******************************************************************************/

#ifndef PL_DMA_H_
#define PL_DMA_H_

// DMA PRESET


void DMA_preset(){
	XAxiDma_Config *CfgPtr;
	int Status;

	// Initialize the XAxiDma device.
	CfgPtr = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);

	Status = XAxiDma_CfgInitialize(&DMA0, CfgPtr);

	// Disable interrupts, we use polling mode
	XAxiDma_IntrDisable(&DMA0, XAXIDMA_IRQ_ALL_MASK,
			XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&DMA0, XAXIDMA_IRQ_ALL_MASK,
			XAXIDMA_DMA_TO_DEVICE);
}



// DMA ADDRESS SETTING
void DMA_ADDR_setup(XAxiDma *Set_DMA, int SrcAddr, int DestAddr){
	int RingIndex = 0;

	XAxiDma_WriteReg(Set_DMA->RxBdRing[RingIndex].ChanBase,
			XAXIDMA_DESTADDR_OFFSET, (u32) DestAddr);

//	XAxiDma_WriteReg(Set_DMA->RxBdRing[RingIndex].ChanBase,XAXIDMA_CR_OFFSET,65539);

	XAxiDma_WriteReg(Set_DMA->RxBdRing[RingIndex].ChanBase,
			XAXIDMA_CR_OFFSET,
			XAxiDma_ReadReg(Set_DMA->RxBdRing[RingIndex].ChanBase,
					XAXIDMA_CR_OFFSET)| XAXIDMA_CR_RUNSTOP_MASK);

	XAxiDma_WriteReg(Set_DMA->TxBdRing.ChanBase,
			XAXIDMA_SRCADDR_OFFSET, (u32) SrcAddr);

//	XAxiDma_WriteReg(Set_DMA->TxBdRing.ChanBase,XAXIDMA_CR_OFFSET,65539);

	XAxiDma_WriteReg(Set_DMA->TxBdRing.ChanBase,
			XAXIDMA_CR_OFFSET,
			XAxiDma_ReadReg(Set_DMA->TxBdRing.ChanBase,XAXIDMA_CR_OFFSET)| XAXIDMA_CR_RUNSTOP_MASK);

}

// DMA Go
void DMA_Go(XAxiDma *Set_DMA, int Len){
	int RingIndex = 0;

	XAxiDma_WriteReg(Set_DMA->RxBdRing[RingIndex].ChanBase,
			XAXIDMA_BUFFLEN_OFFSET, Len);

	XAxiDma_WriteReg(Set_DMA->TxBdRing.ChanBase,
			XAXIDMA_BUFFLEN_OFFSET, Len);
}

#endif /* PL_DMA_H_ */
