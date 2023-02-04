# Embedded-Computing-term-project
Konkuk Univ. 3rd grade, Embedded Computing lecture - Park Jonghyuk, Ko Ryeowook

# Key objectives of this Term project 
* Designing a __controller__ for memory-based FFT Accelerator using Verilog HDL

## Things provided in class
* Block diagram of the FFT Acclerator 
* Code provided (Verilog)
> axis_fft.v, BF.v, FFT.v, FFTcore.v, MULT.v, TopFFT.v 
* Code provided (C)
> Reference SDK program 
> except "fft_hw" function in main.c, in which flushes and activates DMA. 
* Timing diagram of the whole set

<p align="center">
  <img src="/images/block_diagram.png" width="60%" height="60%" title="total loss" alt="total loss"></img>
</p>

## What we considered while designing the controller
* Considered the input, output, and AXI handshake(valid, ready)
* Considered whether it is possible to synthesize 
* 50MHz clock reference, considered the worst negative slack



# Programming language & S/W tools & FPGA used
* Verilog HDL, C
* Vivado 2017.4, Vivado SDK
* Zybo Z7-20

# Design
## 16pt FFT controller
* Our design : controller_16pt_2.v

## 64pt FFT controller

# Result

# What I learned