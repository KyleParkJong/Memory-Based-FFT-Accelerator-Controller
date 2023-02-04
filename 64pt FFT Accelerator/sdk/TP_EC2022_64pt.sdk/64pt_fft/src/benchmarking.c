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
2019.05.27: Extended by Sunwoo Kim (AlexNet)
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <xtime_l.h>

#include "benchmarking.h"
#include "parameter.h"
/**********************************************************
 * benchmark statistics related functions
 **********************************************************/
void statistics_init(BENCHMARK_STATISTICS *p)
{
	p->uiCount=0;
	p->ullMax=0;
	p->ullMin=0xFFFFFFFFFFFFFFFF;
	p->ullTotal=0;
}
void statistics_add(BENCHMARK_STATISTICS *p, u64 ullTickUsed)
{
	if(ullTickUsed > p->ullMax)    p->ullMax = ullTickUsed;
	if(ullTickUsed < p->ullMin)    p->ullMin = ullTickUsed;
	p->ullTotal += ullTickUsed;
	p->uiCount++;
}

u64 statistics_avg(BENCHMARK_STATISTICS *p)
{
	return (p->ullTotal)/(p->uiCount);  /* unit: tick */
}

u64 statistics_filtered_avg(BENCHMARK_STATISTICS *p)
{
	return (p->ullTotal-p->ullMax-p->ullMin)/(p->uiCount-2);  /* unit: tick */
}

void statistics_print(BENCHMARK_STATISTICS *p)
{
    printf("Nr,         Max,         Min,     Average,    Fltr Avg,  Fltr_Avg(us)\r\n");
    printf("%2u,%12llu,%12llu,%12llu,%12llu,%13.3f\r\n", p->uiCount, p->ullMax, p->ullMin, statistics_avg(p), statistics_filtered_avg(p), ((double)statistics_filtered_avg(p))*TIME_PER_TICK*1000000.0 );
}

//extern void XTime_SetTime(XTime Xtime);
//extern void XTime_GetTime(XTime *Xtime);
/**********************************************************
 * benchmark related functions
 **********************************************************/
int run_benchmark_single(BENCHMARK_CASE *pBenchmarkcase)
{
	u64 ullCntValue1, ullCntValue2;
	BENCHMARK_STATISTICS *pStat=&(pBenchmarkcase->stat);

	int i = 0;
	//unsigned int uiResult;
	unsigned int uiSuccess=0;


    statistics_init(pStat);

    for(i=0;i<pBenchmarkcase->uiTestRounds;i++)
	{
    	pBenchmarkcase->initializor(pBenchmarkcase->uiParam[0], pBenchmarkcase->uiParam[1], pBenchmarkcase->uiParam[2]);
		Xil_DCacheFlush();

		ullCntValue1 = 0;
    	XTime_SetTime(0L);
    	XTime_GetTime(&ullCntValue1);

		pBenchmarkcase->uiRetCode = pBenchmarkcase->benchmarker(pBenchmarkcase->uiParam[0], pBenchmarkcase->uiParam[1], pBenchmarkcase->uiParam[2]);

    	XTime_GetTime(&ullCntValue2);

		statistics_add(pStat, ullCntValue2 - ullCntValue1);

		uiSuccess += pBenchmarkcase->validator(pBenchmarkcase->uiParam[0], pBenchmarkcase->uiParam[1], pBenchmarkcase->uiParam[2]);
	}
    pStat->uiSuccess = uiSuccess;

    return 0;
}
