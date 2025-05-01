/******************************************************************************
* Sensecoding.com Integrated Solutions for Low-Powered Systems
* Run on Vitis HLS
******************************************************************************/

#ifndef LOG_REGRESSION_TRAIN
#define LOG_REGRESSION_TRAIN

#include <ap_fixed.h>

#ifdef __SYNTHESIS__
	#include <hls_math.h>
#else
	#include <cmath>
#endif

#define MAX_DATA_SIZE 4
#define MAX_EPOCHS 1000
#define MAX_SAMPLES 150

#ifdef __SYNTHESIS__
	typedef ap_fixed<25, 5, AP_RND_ZERO> DataType;
#else
	typedef float DataType;
#endif

void LogRegression(int* DataInBuff, int* LabelsInBuff,
				   int* WeightsInBuff, int* WeightsOutBuff, int* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   int* LearningRate, unsigned int* NumEpochs,
				   int* CostP );

void Propagate( DataType* Inputs, DataType* Labels,
				DataType *w, DataType* b,
				unsigned int num_px, unsigned int num_samples,
				DataType* dw, DataType* db, DataType* cost);

void CopyFloatToDataTypeBuffers(float* From, DataType* To, unsigned int Dim);
void CopyDataTypeToFloatBuffers(DataType* From, float* To, unsigned int Dim);

void CopyIntToDataTypeBuffers(int* From, DataType* To, unsigned int Dim);
void CopyDataTypeToIntBuffers(DataType* From, int* To, unsigned int Dim);

DataType sigmoid(DataType d);
DataType squareError(DataType d1, DataType d2) ;
DataType Relu(DataType d);

//void backpropagate();

#endif
