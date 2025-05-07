/******************************************************************************
* Sensecoding.com Integrated Solutions for Low-Powered Systems
* Run on Vitis HLS
******************************************************************************/

#ifndef LOG_REGRESSION_TRAIN
#define LOG_REGRESSION_TRAIN

#include <ap_fixed.h>
#include <iostream>

#ifdef __SYNTHESIS__
	#include <hls_math.h>
#else
	#include <cmath>
#endif

#define MAX_DATA_SIZE 4
#define MAX_EPOCHS 1000
#define MAX_SAMPLES 150

#ifdef __SYNTHESIS__
	typedef ap_fixed<32, 16, AP_RND_ZERO> DataType;
#else
	typedef float DataType;
#endif

void LogRegression(unsigned int* DataInBuff, unsigned int* LabelsInBuff,
				   unsigned int* WeightsInBuff, unsigned int* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   unsigned int* LearningRateP, unsigned int* NumEpochsP,
				   unsigned int* CostP, unsigned int* actualEpochsP );

void Propagate( DataType* Inputs, DataType* Labels,
				DataType *Weights, DataType* Bias, DataType LeaningRate,
				unsigned int NumFeatures, unsigned int NumSamples,
				DataType* dw, DataType* db, DataType* cost);

void CopyFloatToDataTypeBuffers(float* From, DataType* To, unsigned int Dim);
void CopyDataTypeToFloatBuffers(DataType* From, float* To, unsigned int Dim);

void CopyIntToDataTypeBuffers(unsigned int* From, DataType* To, unsigned int Dim);
void CopyDataTypeToIntBuffers(DataType* From, unsigned int* To, unsigned int Dim);

DataType sigmoid(DataType d);
DataType squareError(DataType d1, DataType d2) ;
DataType Relu(DataType d);

//void backpropagate();

#endif
