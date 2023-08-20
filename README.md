# Embedded-Computing-term-project
> Konkuk Univ. junior, Embedded Computing class - Park Jonghyuk, Ko Ryeowook

# 0. Key objectives of this Term project 
> _Designing a __controller__ for memory-based FFT Accelerator using Verilog HDL_

## > Things provided in class
* Block diagram of the FFT Acclerator 
* Code provided (__Verilog HDL__)
    + axis_fft.v, BF.v, FFT.v, FFTcore.v, MULT.v, TopFFT.v 
* Code provided (C)
    + Reference SDK program 
    + except "fft_hw" function in main.c, in which flushes and activates DMA. 
* Timing diagram of the whole set

## > Block diagram

<p align="center">
  <img src="/images/block_diagram.png" width="90%" height="90%" title="total loss" alt="total loss"></img>
</p>

## > Vivado block design
<p align="center">
  <img src="/images/block_design.png" width="90%" height="90%" title="total loss" alt="total loss"></img>
</p>

+ More details : TP_EC22_r3.pdf

## What we considered while designing the controller
* Output result accuracy compared to reference output
* Considered the input, output, and AXI handshake(valid, ready)
* Considered whether it is possible to synthesize 
* 50MHz clock reference, considered the worst negative slack

# 1. Programming language & S/W tools & FPGA used
* __Verilog HDL__, C
* Vivado 2017.4, Vivado SDK
* Zybo Z7-20

# 2. Our Design
* __controller_16pt_2.v__, __controller_64pt.v__
* sdk_src main.c "fft_hw" function

<p align="center">
  <img src="/images/fft_hw.png" width="60%" height="60%" title="total loss" alt="total loss"></img>
</p>

+ More details : TP_EC2022_16pt_5조.pdf, TP_EC2022_64pt_5조.pdf

# 3. Result
* Accuracy (NSR) and speed compared to reference(only using SW) were evaluation criteria.
* Comparison between implementing FFT through HW(using DMA) and implementing only through SW(reference)

## 16pt FFT Accelerator

<img src="/images/16pt_result1.jpg" width="70%" height="70%" title="total loss" alt="total loss"></img>

<img src="/images/16pt_result2.jpg" width="70%" height="70%" title="total loss" alt="total loss"></img>

## 64pt FFT Accelerator

<img src="/images/64pt_result1.jpg" width="70%" height="70%" title="total loss" alt="total loss"></img>

<img src="/images/64pt_result2.jpg" width="70%" height="70%" title="total loss" alt="total loss"></img>

+ More details : TP_EC2022_16pt_5조.pdf, TP_EC2022_64pt_5조.pdf

# 4. What I learned
* Improved my Verilog HDL and C language skills
    + Synthesis-able Verilog code 
    + Understanding of AXI handshake
* Be skillful of Vivado, Vivado SDK SW tools
    + Vivado, SDK settings
    + Simulation (Behavioral, Synthesis, Implementation)
    + Timing Summary (setup/hold time, worst negative slack)
    + Hardware debugging (ILA wave)
    + Creating IP project
    + Understanding of Vivado block design (AXI interconnect, DMA, ZYNQ7 processing system), port connecting
* Understanding of Computer Architecture
    + DMA, AMBA AXI protocol
* Understanding of FFT
