Code was tested on Zynq-7 SoC xc7z020;

Host code runs on FreeRTOS and commands logistic regression training on FPGA; 
DDR memory used to pass data through AXI and axi_lite;

1) sets inputs required by to the top function : number of samples, number of features,
input data, input weights and bias , learning rate and number of epochs;
2) kicks the IP Kernel
3) waits on interrupt from IP Kernel when training is completed completed
4) reads back cost, actual epochs needed to achieve training and updated weights and bias to be used as inputs in inference

Note: 
- the logarithmic function used to compute the cost is bottleneck. Next step is to integrate the Floating Point IP provided by Vivado
- optimization needed to be done with quantization to reduce memory consumption ( more efficient ap_fixed ). Quantization will help reducing the number of
  bits needed to represent the weights, increasing performance, saving memory and power consumption, without affecting the outcome of the learning process 
