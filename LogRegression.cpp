/******************************************************************************
* Sensecoding.com Integrated Solutions for Low-Powered Systems
* Run on Vitis HLS
******************************************************************************/

#include "LogRegression.hpp"
#include <algorithm>
#include <memory>

void LogRegression(int* DataInBuff, int* LabelsInBuff,
				   int* WeightsInBuff, int* WeightsOutBuff, int* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   int* LearningRateP, unsigned int* NumEpochsP,
				   int* CostP )

{
	#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	#pragma HLS INTERFACE mode=m_axi port=DataInBuff     offset=slave bundle=gmem depth=1024
	#pragma HLS INTERFACE mode=m_axi port=LabelsInBuff   offset=slave bundle=gmem depth=256
	#pragma HLS INTERFACE mode=m_axi port=WeightsInBuff  offset=slave bundle=gmem depth=64
	#pragma HLS INTERFACE mode=m_axi port=WeightsOutBuff offset=slave bundle=gmem depth=64


//	#pragma HLS INTERFACE mode=s_axilite port=DataInBuff
//	#pragma HLS INTERFACE mode=s_axilite port=LabelsInBuff
//	#pragma HLS INTERFACE mode=s_axilite port=WeightsInBuff

    #pragma HLS INTERFACE mode=s_axilite port=BiasP
    #pragma HLS INTERFACE mode=s_axilite port=DataDimensionP
	#pragma HLS INTERFACE mode=s_axilite port=NumSamplesP
	#pragma HLS INTERFACE mode=s_axilite port=CostP

	unsigned int DataDim    = 4; 	//*DataDimensionP; // number of features per sample
	unsigned int NumSamples = 150; 	//*NumSamplesP;    // number of samples
	unsigned int NumEpochs  = 1000; //*NumEpochsP

	//printf("%d %d %d %d %d \n", DataDimension, HiddenDimension, FullDimension, LearningRate, Momentum);

	DataType  Inputs [MAX_DATA_SIZE * MAX_SAMPLES]; // an array of all samples with their features
	DataType  Weights[MAX_DATA_SIZE]; // number of features per sample - each feature associated with a weight
	DataType  Labels [MAX_SAMPLES]; // max number of samples
	DataType  Bias;
	DataType  Cost;

//	#pragma HLS ARRAY_PARTITION variable=Inputs  type=complete
//	#pragma HLS ARRAY_PARTITION variable=Labels  type=complete
//	#pragma HLS ARRAY_PARTITION variable=Weights type=complete

	// copy inputs to local array

	CopyIntToDataTypeBuffers(DataInBuff,    Inputs,  DataDim * NumSamples);
	CopyIntToDataTypeBuffers(LabelsInBuff,  Labels,  NumSamples);
	CopyIntToDataTypeBuffers(WeightsInBuff, Weights, DataDim);

	float fbias;
	memcpy(&fbias, BiasP, sizeof(float));
	Bias = (DataType)fbias;

	float fLearningRate;
	memcpy(&fLearningRate, LearningRateP, sizeof(float));

	// internal use
	DataType dw[MAX_DATA_SIZE];
	DataType db = 0.0;

	for (int epochs = 0; epochs < NumEpochs; ++epochs)
	{
		Propagate(Inputs, Labels, Weights, &Bias, DataDim, NumSamples,
				  dw, &db, &Cost);
	}

	CopyDataTypeToIntBuffers(Weights, WeightsOutBuff, DataDim);

	float fBias = static_cast<float>(Bias);
	memcpy(BiasP, &fBias, sizeof(float));

	float fCost = static_cast<float>(Cost);
	memcpy(CostP, &fCost, sizeof(float));
}

// Inputs: data (num_features, NumSamples) - now a 1D array
// Label: labels (1, NumSamples)  - now a 1D array
// w: weights (num_features, 1) - now a 1D array
// b: bias (scalar)
// num_features: number of features (e.g., num_features * num_features * 3 for an image)
// NumSamples: number of training samples
// dw: gradient of cost with respect to w (output, same shape as w) - 1D array
// db: gradient of cost with respect to b (output, scalar)
// cost: cost of the logistic regression (output, scalar)

void Propagate( DataType* Inputs, DataType* Labels,
				DataType *w, DataType* b,
				unsigned int num_features, unsigned int NumSamples,
				DataType* dw, DataType* db, DataType* cost)
{
    int i, j;
    DataType z, a, sum;

    // Forward propagation
    DataType A[MAX_SAMPLES]; // Allocate up to 150 samples
    DataType Cost[MAX_SAMPLES];
    *cost = 0.0;

    for (i = 0; i < NumSamples; i++)
    {
        z = *b;
        for (j = 0; j < num_features; j++)
        {
			#pragma HLS PIPELINE off
            z += w[j] * Inputs[i * num_features + j]; // Indexing into Inputs

        }
        A[i] = sigmoid(z);
    	Cost[i] = -((static_cast<DataType>(log(A[i]))) * Labels[i] + (static_cast<DataType>(log(1 - A[i]))) * (1 - Labels[i]));
    }

    for (i = 0; i < NumSamples; i++)
    {
    	*cost += Cost[i];
    }


    *cost = *cost / NumSamples;

    // Backward propagation
    *db = 0.0;
    for (j = 0; j < num_features; j++)
    {
		#pragma HLS PIPELINE off

        dw[j] = 0.0;

        for (i = 0; i < NumSamples; i++)
        {
			#pragma HLS PIPELINE off
            dw[j] += Inputs[i * num_features + j] * (A[i] - Labels[i]); // Indexing into X
            *db += (A[i] - Labels[i]);
        }
        dw[j] /= NumSamples;
    }

    *db = 0.0;
    for (int i = 0; i < NumSamples; i++)
    {
		#pragma HLS PIPELINE off
        *db += (A[i] - Labels[i]);
    }
    *db /= NumSamples;

    for (int j = 0; j < 4; j++)
    {
		#pragma HLS PIPELINE off
        w[j] -= static_cast<DataType>(0.01) * dw[j];
    }
    *b -= static_cast<DataType>(0.01) * (*db);
}



void CopyIntToDataTypeBuffers(int* From, DataType* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		memcpy(&fval, &From[i], sizeof(float));
		To[i] = static_cast<DataType>(fval);
	}
}

void CopyDataTypeToIntBuffers(DataType* From, int* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		fval = static_cast<float>(From[i]);
		memcpy(&To[i], &fval, sizeof(float));
	}
}

void CopyFloatToDataTypeBuffers(float* From, DataType* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		To[i] = (DataType)From[i];
	}
}

void CopyDataTypeToFloatBuffers(DataType* From, float* To, unsigned int Dim)
{
	Loop4:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		To[i] = (float)From[i];
	}
}


//The sigmoid function  can assume all values in the range ]0, 1[.
DataType sigmoid(DataType d)
{
	DataType Res = (DataType)1.0 / ( (DataType)1.0 + (DataType)exp(-d));
	return Res;
}

DataType Relu(DataType x)
{
    return (x > 0) ? x : (DataType)0; // Use ternary operator for ap_fixed
}

DataType squareError(DataType d1, DataType d2)
{
	DataType Diff = d1-d2;
	DataType P=2.0;

	//#pragma HLS PIPELINE off

	return Diff*Diff;
}

