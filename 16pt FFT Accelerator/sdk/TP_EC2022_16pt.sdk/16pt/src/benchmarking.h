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

Authors: Jooho Wang (joohowang@konkuk.ac.kr)

Revision History
2017.11.11: Started by Jooho Wang
*******************************************************************************/

#ifndef BENCHMARKING_H_
#define BENCHMARKING_H_

#include "xparameters.h"
#include "xscutimer.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include <xtime_l.h>

#include "parameter.h"
/* definition and variable for time measurement */
#define CPU_FREQ_HZ			XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_LOAD_VALUE	0xFFFFFFFF
#define TIMER_PRE_SCALE	    0
#define TIME_PER_TICK		((float)2.0*(TIMER_PRE_SCALE+1)/CPU_FREQ_HZ)

typedef struct {
	unsigned int uiCount;
	unsigned int uiSuccess;
	u64          ullMax;
	u64          ullMin;
	u64          ullTotal;

}BENCHMARK_STATISTICS;

typedef struct {
	/* input */
	char         *pName;
	unsigned int uiTestRounds;
	unsigned int (*initializor)(my_complex* uiParam0, my_complex* uiParam1, my_complex* uiParam2);

	float        (*benchmarker)(my_complex* uiParam0, my_complex* uiParam1, my_complex* uiParam2);
	my_complex* uiParam[3];
	float uiRetCode;
	unsigned int (*validator)(int uiParam0, int uiParam1, int uiParam2);

	BENCHMARK_STATISTICS stat;
}BENCHMARK_CASE;

extern void statistics_print(BENCHMARK_STATISTICS *p);
extern int run_benchmark_single(BENCHMARK_CASE *pBenchmarkcase);

#endif /* BENCHMARKING_H_ */
