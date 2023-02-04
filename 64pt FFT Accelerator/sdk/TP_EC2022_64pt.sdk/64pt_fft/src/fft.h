#ifndef FFT_H_
#define FFT_H_

#include "parameter.h"

extern void make_twiddle(my_complex *W);
extern int bit_revese(int num, int no_of_bits); // reordering function
extern my_complex mult(my_complex x, my_complex y); // multiply function of real and imaginary number
extern my_complex add(my_complex x, my_complex y); // add function of real and imaginary number
extern my_complex sub(my_complex x, my_complex y); // subtract function of real and imaginary number
extern void fft_ref(my_complex* out, my_complex* input, my_complex* W_in);

#endif
