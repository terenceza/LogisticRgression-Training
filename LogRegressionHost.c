/******************************************************************************
* Sensecoding.com Integrated Solutions for Low-Powered Systems
* Run on Vitis ARM FreeRTOS
******************************************************************************/

#include <stdio.h>
//#include "platform.h"
#include "xil_printf.h"
#include "xlogregression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LogRegressionHost.h"
//#include "xscutimer.h"
#include <time.h>

XLogregression LogRegInst;
XLogregression_Config Cfg;

float Data[] =
{
    5.1, 3.5, 1.4, 0.2,
    4.9, 3.0, 1.4, 0.2,
    4.7, 3.2, 1.3, 0.2,
    4.6, 3.1, 1.5, 0.2,
    5.0, 3.6, 1.4, 0.2,
    5.4, 3.9, 1.7, 0.4,
    4.6, 3.4, 1.4, 0.3,
    5.0, 3.4, 1.5, 0.2,
    4.4, 2.9, 1.4, 0.2,
    4.9, 3.1, 1.5, 0.1,
    5.4, 3.7, 1.5, 0.2,
    4.8, 3.4, 1.6, 0.2,
    4.8, 3.0, 1.4, 0.1,
    4.3, 3.0, 1.1, 0.1,
    5.8, 4.0, 1.2, 0.2,
    5.7, 4.4, 1.5, 0.4,
    5.4, 3.9, 1.3, 0.4,
    5.1, 3.5, 1.4, 0.3,
    5.7, 3.8, 1.7, 0.3,
    5.1, 3.8, 1.5, 0.3,
    5.4, 3.4, 1.7, 0.2,
    5.1, 3.7, 1.5, 0.4,
    4.6, 3.6, 1.0, 0.2,
    5.1, 3.3, 1.7, 0.5,
    4.8, 3.4, 1.9, 0.2,
    5.0, 3.0, 1.6, 0.2,
    5.0, 3.4, 1.6, 0.4,
    5.2, 3.5, 1.5, 0.2,
    5.2, 3.4, 1.4, 0.2,
    4.7, 3.2, 1.6, 0.2,
    4.8, 3.1, 1.6, 0.2,
    5.4, 3.4, 1.5, 0.4,
    5.2, 4.1, 1.5, 0.1,
    5.5, 4.2, 1.4, 0.2,
    4.9, 3.1, 1.5, 0.1,
    5.0, 3.2, 1.2, 0.2,
    5.5, 3.5, 1.3, 0.2,
    4.9, 3.1, 1.5, 0.1,
    4.4, 3.0, 1.3, 0.2,
    5.1, 3.4, 1.5, 0.2,
    5.0, 3.5, 1.3, 0.3,
    4.5, 2.3, 1.3, 0.3,
    4.4, 3.2, 1.3, 0.2,
    5.0, 3.5, 1.6, 0.6,
    5.1, 3.8, 1.9, 0.4,
    4.8, 3.0, 1.4, 0.3,
    5.1, 3.8, 1.6, 0.2,
    4.6, 3.2, 1.4, 0.2,
    5.3, 3.7, 1.5, 0.2,
    5.0, 3.3, 1.4, 0.2,
    7.0, 3.2, 4.7, 1.4,
    6.4, 3.2, 4.5, 1.5,
    6.9, 3.1, 4.9, 1.5,
    5.5, 2.3, 4.0, 1.3,
    6.5, 2.8, 4.6, 1.5,
    5.7, 2.8, 4.5, 1.3,
    6.3, 3.3, 4.7, 1.6,
    4.9, 2.4, 3.3, 1.0,
    6.6, 2.9, 4.6, 1.3,
    5.2, 2.7, 3.9, 1.4,
    5.0, 2.0, 3.5, 1.0,
    5.9, 3.0, 4.2, 1.5,
    6.0, 2.2, 4.0, 1.0,
    6.1, 2.9, 4.7, 1.4,
    5.6, 2.9, 3.6, 1.3,
    6.7, 3.1, 4.4, 1.4,
    5.6, 3.0, 4.5, 1.5,
    5.8, 2.7, 4.1, 1.0,
    6.2, 2.2, 4.5, 1.5,
    5.6, 2.5, 3.9, 1.1,
    5.9, 3.2, 4.8, 1.8,
    6.1, 2.8, 4.0, 1.3,
    6.3, 2.5, 4.9, 1.5,
    6.1, 2.8, 4.7, 1.2,
    6.4, 2.9, 4.3, 1.3,
    6.6, 3.0, 4.4, 1.4,
    6.8, 2.8, 4.8, 1.4,
    6.7, 3.0, 5.0, 1.7,
    6.0, 2.9, 4.5, 1.5,
    5.7, 2.6, 3.5, 1.0,
    5.5, 2.4, 3.8, 1.1,
    5.5, 2.4, 3.7, 1.0,
    5.8, 2.7, 3.9, 1.2,
    6.0, 2.7, 5.1, 1.6,
    5.4, 3.0, 4.5, 1.5,
    6.0, 3.4, 4.5, 1.6,
    6.7, 3.1, 4.7, 1.5,
    6.3, 2.3, 4.4, 1.3,
    5.6, 3.0, 4.1, 1.3,
    5.5, 2.5, 4.0, 1.3,
    5.5, 2.6, 4.4, 1.2,
    6.1, 3.0, 4.6, 1.4,
    5.8, 2.6, 4.0, 1.2,
    5.0, 2.3, 3.3, 1.0,
    5.6, 2.7, 4.2, 1.3,
    5.7, 3.0, 4.2, 1.2,
    5.7, 2.9, 4.2, 1.3,
    6.2, 2.9, 4.3, 1.3,
    5.1, 2.5, 3.0, 1.1,
    5.7, 2.8, 4.1, 1.3,
    6.3, 3.3, 6.0, 2.5,
    5.8, 2.7, 5.1, 1.9,
    7.1, 3.0, 5.9, 2.1,
    6.3, 2.9, 5.6, 1.8,
    6.5, 3.0, 5.8, 2.2,
    7.6, 3.0, 6.6, 2.1,
    4.9, 2.5, 4.5, 1.7,
    7.3, 2.9, 6.3, 1.8,
    6.7, 2.5, 5.8, 1.8,
    7.2, 3.6, 6.1, 2.5,
    6.5, 3.2, 5.1, 2.0,
    6.4, 2.7, 5.3, 1.9,
    6.8, 3.0, 5.5, 2.1,
    5.7, 2.5, 5.0, 2.0,
    5.8, 2.8, 5.1, 2.4,
    6.4, 3.2, 5.3, 2.3,
    6.5, 3.0, 5.5, 1.8,
    7.7, 3.8, 6.7, 2.2,
    7.7, 2.6, 6.9, 2.3,
    6.0, 2.2, 5.0, 1.5,
    6.9, 3.2, 5.7, 2.3,
    5.6, 2.8, 4.9, 2.0,
    7.7, 2.8, 6.7, 2.0,
    6.3, 2.7, 4.9, 1.8,
    6.7, 3.3, 5.7, 2.1,
    7.2, 3.2, 6.0, 1.8,
    6.2, 2.8, 4.8, 1.8,
    6.1, 3.0, 4.9, 1.8,
    6.4, 2.8, 5.6, 2.1,
    7.2, 3.0, 5.8, 1.6,
    7.4, 2.8, 6.1, 1.9,
    7.9, 3.8, 6.4, 2.0,
    6.4, 2.8, 5.6, 2.2,
    6.3, 2.8, 5.1, 1.5,
    6.1, 2.6, 5.6, 1.4,
    7.7, 3.0, 6.1, 2.3,
    6.3, 3.4, 5.6, 2.4,
    6.4, 3.1, 5.5, 1.8,
    6.0, 3.0, 4.8, 1.8,
    6.9, 3.1, 5.4, 2.1,
    6.7, 3.1, 5.6, 2.4,
    6.9, 3.1, 5.1, 2.3,
    5.8, 2.7, 5.1, 1.9,
    6.8, 3.2, 5.9, 2.3,
    6.7, 3.3, 5.7, 2.5,
    6.7, 3.0, 5.2, 2.3,
    6.3, 2.5, 5.0, 1.9,
    6.5, 3.0, 5.2, 2.0,
    6.2, 3.4, 5.4, 2.3,
    5.9, 3.0, 5.1, 1.8
}; 

float Labels[] =
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

float Weights[] = 
{
    0.1, 0.1, 0.1, 0.1
};


float Bias = 0.1;
float LearningRate = 0.02;

// XScuTimer	Timer;
// XScuTimer_Config *TMRConfigPtr;

void InitTimer()
{
    // TMRConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    // XScuTimer_CfgInitialize(&Timer, TMRConfigPtr, TMRConfigPtr->BaseAddr);
    // XScuTimer_SelfTest(&Timer);
    // //load the timer
    // XScuTimer_LoadTimer(&Timer, TIMER_LOAD_VALUE);
}

void LogRegressionInit(XLogregression* Inst, XLogregression_Config* Cfg)
{
	if ( XLogregression_Initialize(Inst, XPAR_XLOGREGRESSION_0_DEVICE_ID) != XST_SUCCESS)
	{
		xil_printf("XLogregression_Initialize failed \n");
	}
    else
    {
	    xil_printf("XLogregression_Initialize success \n");
    }

	XLogregression_InterruptGlobalDisable(Inst);
	XLogregression_InterruptDisable(Inst, 1);

    XLogregression_DisableAutoRestart(Inst);
}

void WriteDataToBus()
{
    int numElements = sizeof(Data) / sizeof(Data[0]);

    //xil_printf("Writing float input data to DDR, numElements = %d\n", numElements);

    // Write the data
    for (int i = 0; i < numElements; ++i)
    {
        int32_t ival;
        memcpy(&ival, &Data[i], sizeof(int32_t));
        XLogregression_WriteReg(DDR_DATA_BASE_ADDR_IN, i*4, ival);

        //xil_printf( " 0x%x ", ival);
    }

    //xil_printf("Finished writing data.\n");

    // Read back and verify
    // xil_printf("Reading back data from DDR for verification:\n");

    // for (int i = 0; i < numElements; ++i)
    // {
    //     uint32_t val = XLogregression_ReadReg(DDR_DATA_BASE_ADDR_IN, i*4);
    //     xil_printf("Read: 0x%x\n",  val);
    // }

}

void WriteLabelsToBus()
{
    int numElements = sizeof(Labels) / sizeof(Labels[0]);

    //xil_printf("Writing float Labels data to DDR, numElements = %d\n", numElements);

    // Write the data
    for (int i = 0; i < numElements; ++i)
    {
        int32_t ival;
        memcpy(&ival, &Labels[i], sizeof(int32_t));
        XLogregression_WriteReg(DDR_LABELS_BASE_ADDR, i*4, ival);
        //xil_printf( " 0x%x ", ival);
    }

    //xil_printf("Finished writing labels.\n");

    // // Read back and verify
    // xil_printf("Reading back labels from DDR for verification:\n");

    // for (int i = 0; i < numElements; ++i)
    // {
    //     float fval;
    //     int32_t val = XLogregression_ReadReg(DDR_LABELS_BASE_ADDR, i*4);
    //     xil_printf("Read: 0x%x \n",  val);
    // }

}

void WriteWeightsToBus()
{
    int numElements = sizeof(Weights) / sizeof(Weights[0]);

    //xil_printf("Writing float weights data to DDR, numElements = %d\n", numElements);

    // Write the data
    for (int i = 0; i < numElements; ++i)
    {
        
        int32_t ival;
        memcpy(&ival, &Weights[i], sizeof(int32_t));
        XLogregression_WriteReg(DDR_WEIGHTS_BASE_ADDR_IN, i*4, ival);
        //xil_printf( " 0x%x ", ival);
    }

    //xil_printf("Finished writing weights.\n");

    // Read back and verify
    // xil_printf("Reading back weights from DDR for verification:\n");

    // for (int i = 0; i < numElements; ++i)
    // {
    //     uint32_t val = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_IN, i*4);
    //     xil_printf("Read: 0x%08x \n",  val);
    // }
}

int RunLogRegressionTrain()
{
    unsigned int  LoopCount = 0;
    unsigned long TickStart = 0;
    unsigned long TickStop = 0;
    unsigned long Delta = 0;
    uint32_t iBias = 0;
    uint32_t iWeight;
    uint32_t iLearningRate;
    uint32_t numTestData = sizeof(Data)   / sizeof(Data[0]);
    uint32_t numSamples = sizeof(Labels)  / sizeof(Labels[0]);
    uint32_t numFeatures= sizeof(Weights) / sizeof(Weights[0]);

    //init_platform();
    InitTimer();
    LogRegressionInit(&LogRegInst, &Cfg);

    // Set start address for data
    XLogregression_Set_DataInBuff(&LogRegInst, DDR_DATA_BASE_ADDR_IN);

    // Set start address for labels
    XLogregression_Set_LabelsInBuff(&LogRegInst, DDR_LABELS_BASE_ADDR);

    // Set start address for starting weights
    XLogregression_Set_WeightsInBuff(&LogRegInst, DDR_WEIGHTS_BASE_ADDR_IN);

    // Writes once  data and labels in memory 
    WriteDataToBus();
    WriteLabelsToBus();
    WriteWeightsToBus();

    XLogregression_Set_DataDimensionP(&LogRegInst, numFeatures);
    XLogregression_Set_NumSamplesP(&LogRegInst,numSamples);

    memcpy(&iLearningRate, &LearningRate, sizeof(float));
    XLogregression_Set_LearningRateP(&LogRegInst, iLearningRate);

    // one epoch at a time
    XLogregression_Set_NumEpochsP(&LogRegInst, 500);

    // set bias to start
    memcpy(&iBias, &Bias, sizeof(float));
    XLogregression_Set_BiasP_i(&LogRegInst, iBias);
    //xil_printf("Bias = 0x%x \n", iBias);

    for (int i = 0 ; i < 1 ; ++i)
    {

        XLogregression_Start(&LogRegInst);
        
        // XScuTimer_Start(&Timer);
        // TickStart = XScuTimer_GetCounterValue(&Timer);

        while(!XLogregression_IsDone(&LogRegInst)){}
        //xil_printf("Response back from FPGA \n");
        
        // TickStop = XScuTimer_GetCounterValue(&Timer);
        // XScuTimer_Stop(&Timer);
        // Delta = TickStart - TickStop; //down count timer
        // double DeltaTimeSec = (double)Delta / (double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ;
        // xil_printf("Took %d ticks , %d micro sec \n", Delta, (int)((double)1000000.0f * DeltaTimeSec));

        // xil_printf("Got response from FPGA...\n");

        //xil_printf("Read Weights: \n");
        // for (int i = 0; i < 4; ++i)
        // {
        //     iWeight = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_OUT, i*4);
        //     XLogregression_WriteReg(DDR_WEIGHTS_BASE_ADDR_IN, iWeight, i*4);
        //     ///memcpy(&Weights[i], &iWeight, sizeof(int));
        //     //xil_printf(" 0x%x ", NewWeights[i]);
        //     //memcpy(&NewWeights[i], &val, sizeof(float));
        // }

        // iBias = XLogregression_Get_BiasP_o(&LogRegInst);
        // XLogregression_Set_BiasP_i(&LogRegInst, iBias);
        // memcpy(&Bias, iBias, sizeof(int));

        //sleep(1);
        //xil_printf("Read Bias: 0x%x \n",  biasval);
    }



    unsigned int costval = (int32_t)XLogregression_Get_CostP(&LogRegInst);
    xil_printf("Read Cost: 0x%08x \n",  costval);

    unsigned int biasval =  (int32_t)XLogregression_Get_BiasP_o(&LogRegInst);
    xil_printf("Read updated bias: 0x%08x \n",  biasval);

    unsigned int wval = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_IN, 0);
    xil_printf("Read: 0x%08x \n",  wval);
    wval = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_IN, 4);
    xil_printf("Read: 0x%08x \n",  wval);
    wval = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_IN, 8);
    xil_printf("Read: 0x%08x \n",  wval);
    wval = XLogregression_ReadReg(DDR_WEIGHTS_BASE_ADDR_IN, 12);
    xil_printf("Read: 0x%08x \n",  wval);

    unsigned int eval = XLogregression_Get_actualEpochsP(&LogRegInst);
    xil_printf("Actual epochs = %d", eval);

    xil_printf("Successfully completed logistic training\n");
    //while(1){}
    //cleanup_platform();
    return 0;
}

