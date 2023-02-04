#include<stdio.h>
#include <math.h>

#include "fft.h"




void make_twiddle(my_complex *W)
{
	double delta = 2 * PI / N;
	int i;

	for (i = 0; i < N; i++)
	{
		W[i].real = cos(i * delta);
		W[i].img = -sin(i * delta);
	}
}


int bit_revese(int num, int no_of_bits)
{
    int reverse_num = 0;
    int i;
    for (i = 0; i < no_of_bits; i++)
    {
        if((num & (1 << i)))
           reverse_num |= 1 << ((no_of_bits - 1) - i);
   }
    return reverse_num;
}

my_complex mult(my_complex x, my_complex y) // multiply function of real and imaginary number
{
	my_complex result;
	result.real = x.real*y.real - x.img*y.img;
	result.img = x.real*y.img + x.img*y.real;
	return result;
}
my_complex add(my_complex x, my_complex y) // add function of real and imaginary number
{
	my_complex result;
	result.real = x.real + y.real;
	result.img = x.img + y.img;
	return result;
}
my_complex sub(my_complex x, my_complex y) // subtract function of real and imaginary number
{
	my_complex result;
	result.real = x.real - y.real;
	result.img = x.img - y.img;
	return result;
}

void fft_ref(my_complex* out, my_complex* input, my_complex* W_in)
{
	my_complex stage[M + 1][N];
	my_complex output[N];
	my_complex W[N];

	int data;
	int i, m, n, k;
	int stride;
	int step;

	for (i = 0; i < FFT_ROUNDS; i++)
	{
		for (data = 0; data < N; data++)
		{
			stage[0][data].real = input[i*N + bit_revese(data,M)].real;
			stage[0][data].img = input[i*N + bit_revese(data,M)].img;
			W[data].real = W_in[data].real;
			W[data].img = W_in[data].img;
		}

		stride = 2;
		step = N / 2;

		for (m = 0; m < M; m++)
		{
			for (n = 0; n < stride / 2; n++)
			{
				for (k = 0; k < step; k++)
				{
					stage[m + 1][n + (stride * k)] = add(stage[m][n + (stride * k)], mult((stage[m][n + (stride * k) + stride / 2]),W[step*n]));
					stage[m + 1][n + (stride * k) + stride / 2] = sub(stage[m][n + (stride * k)], mult((stage[m][n + (stride * k) + stride / 2]), W[step * n]));
				}
			}
			stride = stride * 2;
			step = step / 2;
		}

		for (n = 0; n < N; n++)
		{
			output[n] = stage[M][n];
		}

		for (n = 0; n < N; n++)
		{
			out[i*N + n].real = output[n].real;
			out[i*N + n].img = output[n].img;
		}

	}
}



