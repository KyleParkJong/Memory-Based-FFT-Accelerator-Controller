# Embedded-Computing-term-project
Konkuk Univ. 3rd grade, Embedded Computing lecture - Park Jonghyuk, Ko Ryeowook

# 0. Key objectives of this Term project 
* Designing a __controller__ for memory-based FFT Accelerator using Verilog HDL

## Things provided in class
* Block diagram of the FFT Acclerator 
* Code provided (Verilog HDL)
    > axis_fft.v, BF.v, FFT.v, FFTcore.v, MULT.v, TopFFT.v 
* Code provided (C)
    > Reference SDK program 
    > except "fft_hw" function in main.c, in which flushes and activates DMA. 
* Timing diagram of the whole set

<p align="center">
  <img src="/images/block_diagram.png" width="90%" height="90%" title="total loss" alt="total loss"></img>
</p>

+ more details : TP_EC22_r3.pdf

## What we considered while designing the controller
* Considered the input, output, and AXI handshake(valid, ready)
* Considered whether it is possible to synthesize 
* 50MHz clock reference, considered the worst negative slack

# 1. Programming language & S/W tools & FPGA used
* Verilog HDL, C
* Vivado 2017.4, Vivado SDK
* Zybo Z7-20

# 2. Design
* Our design : controller_16pt_2.v, controller_64pt.v
* sdk_src main.c "fft_hw" function

<p align="center">
  <img src="/images/fft_hw.png" width="60%" height="60%" title="total loss" alt="total loss"></img>
</p>

+ more details : TP_EC2022_16pt_5조.pdf, TP_EC2022_64pt_5조.pdf

# 3. Result
* Accuracy (NSR) and speed compared to reference(only using SW) were evaluation criteria.
* Comparison between implementing FFT through HW(using DMA) and implementing only through SW(reference)

## 16pt FFT Accelerator

<img src="/images/16pt_result1.png" width="70%" height="70%" title="total loss" alt="total loss"></img>

<img src="/images/16pt_result2.png" width="70%" height="70%" title="total loss" alt="total loss"></img>

## 64pt FFT Accelerator

<img src="/images/64pt_result1.png" width="70%" height="70%" title="total loss" alt="total loss"></img>

<img src="/images/64pt_result2.png" width="70%" height="70%" title="total loss" alt="total loss"></img>

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
