#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "xaxidma.h"
#include "fft.h"
#include "input.h"
#include "rtlout_ref.h"
#include <xtime_l.h>


#include "parameter.h"
#include "benchmarking.h"
unsigned int initializor_dummy(my_complex* uiParam0, my_complex* uiParam1, my_complex* uiParam2) { return 1; }
unsigned int validator_dummy(my_complex* uiParam0, my_complex* uiParam1, my_complex* uiParam2) { return 1; }


#define INPUT_BASE 			0x11000000
#define OUTPUT_BASE 		0x14000000
#define LENGTH		N * FFT_ROUNDS
#define NUM_FRAC_BITS 15 // output fixed point (1,15)


///////////////////////////////////////////////////////
//////////// Edit code below if necessary  ////////////
XAxiDma DMA0;


//////////// Edit code above if necessary  ////////////
///////////////////////////////////////////////////////
 
#include "PL_DMA.h"

void fft_hw(int input_addr, int output_addr, int len)
{
	/////////////////////////////////////
	/////////// Edit code below!!////////
	Xil_DCacheFlushRange(input_addr, 4 * len);
	DMA_ADDR_setup(&DMA0, input_addr, output_addr);

	DMA_Go(&DMA0, len * 4);

	while ((XAxiDma_Busy(&DMA0, XAXIDMA_DEVICE_TO_DMA)));
	while ((XAxiDma_Busy(&DMA0, XAXIDMA_DMA_TO_DEVICE)));

	Xil_DCacheInvalidateRange(output_addr, 4 * len);
	//////////Edit code above!!/////////
	////////////////////////////////////
}


void make_input(my_complex *input, int hw_input_addr)
{
	int i;
	short num_rand;
	int tmp0, tmp1;

	srand(0);
	Xil_Out32(hw_input_addr, 0x7FFFFFFF);

	//fixedpoint(1,15)
	//input range (-1 ~ 0.999)
	for (i = 0; i < N*FFT_ROUNDS; i++)
	{
		//real ref
		input[i].real = in_ref[i*2];
		//imag ref
		input[i].img = in_ref[(i*2)+1];
		//real & imag hw
		tmp0 = in_hw[i];

		Xil_Out32(hw_input_addr+ 4*(i) , tmp0);
	}
}

int main()
{

	xil_printf("\r\n");
	xil_printf("<%d-point Fourier Transform>\r\n", N);
	xil_printf("<Number of Iterations : %d>\r\n", FFT_ROUNDS);
	int i,  mode_sel, ii = 0;

	my_complex *in_FFT = 0;
	my_complex *out_FFT_ref = 0;
	my_complex *W = 0;
	in_FFT = (my_complex*)malloc(sizeof(my_complex)* N * FFT_ROUNDS); 
	if (in_FFT == NULL) { xil_printf("Memory allocation error\r\n"); };
	out_FFT_ref = (my_complex*)malloc(sizeof(my_complex) * N * FFT_ROUNDS); 
	if (out_FFT_ref == NULL) { xil_printf("Memory allocation error\r\n"); };
	W = (my_complex*)malloc(sizeof(my_complex) * N); 
	if (W == NULL) { xil_printf("Memory allocation error\r\n"); };

	for(i=0;i<N*FFT_ROUNDS;i++) //memory initialization
		Xil_Out32(OUTPUT_BASE+i*sizeof(int), 0X70007000);
	Xil_DCacheFlushRange(OUTPUT_BASE, 4 * N*FFT_ROUNDS);

	make_twiddle(W);
	make_input(in_FFT, INPUT_BASE);

	DMA_preset();
	for (mode_sel = 0; mode_sel < _mode; mode_sel++)
	{
		if (mode_sel == 0)
		{
			fft_ref(out_FFT_ref, in_FFT, W);
		}
		else
		{
			fft_hw(INPUT_BASE, OUTPUT_BASE, LENGTH);
		}
	}

	double error = 0;
	double error_ref = 0;
	double signal = 0;
	double NSRdB = 0;
	double NSRdB_ref = 0;
	double opt_time = 0;
	double ref_time = 0;
	float ref_float = 0;
	short tmp_short;
	short ref_short;
	int Nsam_it=0;
	my_complex out_FFT_hw;
	for (ii = 0; ii < N*FFT_ROUNDS; ii+=1) {

		tmp_short = Xil_In16(OUTPUT_BASE + sizeof(int)*ii + sizeof(short));
		ref_short = (short)rtlout_ref[(ii*2)+1];
		ref_float = 16 * ((float)ref_short/ pow(2,NUM_FRAC_BITS));

		out_FFT_hw.img = 16 * ((float)tmp_short /pow(2,NUM_FRAC_BITS));

		Nsam_it++;
		error = error + (((out_FFT_hw.img - out_FFT_ref[ii].img)*(out_FFT_hw.img - out_FFT_ref[ii].img)) - error) / Nsam_it;
		signal = signal + ((out_FFT_ref[ii].img * out_FFT_ref[ii].img) - signal) / Nsam_it;

		error_ref = error_ref + (((out_FFT_hw.img - ref_float)*(out_FFT_hw.img - ref_float)) - error_ref) / Nsam_it;

		tmp_short = Xil_In16(OUTPUT_BASE + sizeof(int)*ii);
		ref_short = (short)rtlout_ref[ii*2];
		ref_float = 16 * ((float)ref_short/ pow(2,NUM_FRAC_BITS));

		out_FFT_hw.real = 16 * (((float)tmp_short) /pow(2,NUM_FRAC_BITS));

		Nsam_it++;
		error = error + (((out_FFT_hw.real - out_FFT_ref[ii].real)*(out_FFT_hw.real - out_FFT_ref[ii].real)) - error) / Nsam_it;
		signal = signal + ((out_FFT_ref[ii].real * out_FFT_ref[ii].real) - signal) / Nsam_it;

		error_ref = error_ref + (((out_FFT_hw.real - ref_float)*(out_FFT_hw.real - ref_float)) - error_ref) / Nsam_it;
	}

	NSRdB = 10 * log10(error / signal);
	NSRdB_ref = 10 * log10(error_ref / signal);
	printf("\nMeasured Accuracy (vs Floating Point): NSR(dB) = %0.3f \n", NSRdB);
	printf("\nMeasured Accuracy (vs RTL): NSR(dB)            = %0.3f \n", NSRdB_ref);

	BENCHMARK_CASE *pBenchmarkCase;
	BENCHMARK_STATISTICS *pStat;
	BENCHMARK_CASE BenchmarkCases[TEST_CASE] = {
		{"FFT Reference", TEST_ROUNDS, initializor_dummy, fft_ref,
			{out_FFT_ref, in_FFT, W},
			0, validator_dummy
		},
		{"FFT Optimization", TEST_ROUNDS, initializor_dummy, fft_hw,
				{INPUT_BASE, OUTPUT_BASE, LENGTH},
			0, validator_dummy
		}
	};
	xil_printf("\r\n");
	xil_printf("-----Benchmarking Start-----\r\n");
	for (i = 0; i < TEST_CASE; i++)
	{
		pBenchmarkCase = &BenchmarkCases[i];
		pStat = &(pBenchmarkCase->stat);
		printf("Case %d: %s\r\n", i, pBenchmarkCase->pName);
		run_benchmark_single(pBenchmarkCase);
		statistics_print(pStat);
		if (i == 0)
			ref_time = pStat->ullMax;
		else
			opt_time = pStat->ullMax;
	}

	xil_printf("----Benchmarking Complete----\r\n");
	xil_printf("\r\n");
	printf("HW FFT is x%.2f faster than Reference\r\n", (double)(ref_time / opt_time));
	xil_printf("\r\n");

	free(in_FFT);
	free(out_FFT_ref);
	free(W);


	return 0;
}

