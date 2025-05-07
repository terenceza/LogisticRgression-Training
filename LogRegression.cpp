/******************************************************************************
* Sensecoding.com Integrated Solutions for Low-Powered Systems
* Run on Vitis HLS
******************************************************************************/

#include "LogRegression.hpp"
#include <algorithm>
#include <memory>

void LogRegression(unsigned int* DataInBuff, unsigned int* LabelsInBuff,
				   unsigned int* WeightsInBuff, unsigned int* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   unsigned int* LearningRateP, unsigned int* NumEpochsP,
				   unsigned int* CostP, unsigned int* actualEpochsP )

{
	#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	#pragma HLS INTERFACE mode=m_axi port=DataInBuff     offset=slave bundle=gmem
	#pragma HLS INTERFACE mode=m_axi port=LabelsInBuff   offset=slave bundle=gmem
	#pragma HLS INTERFACE mode=m_axi port=WeightsInBuff  offset=slave bundle=gmem

    #pragma HLS INTERFACE mode=s_axilite port=BiasP
    #pragma HLS INTERFACE mode=s_axilite port=DataDimensionP
	#pragma HLS INTERFACE mode=s_axilite port=NumSamplesP
	#pragma HLS INTERFACE mode=s_axilite port=LearningRateP
	#pragma HLS INTERFACE mode=s_axilite port=NumEpochsP
	#pragma HLS INTERFACE mode=s_axilite port=CostP
	#pragma HLS INTERFACE mode=s_axilite port=actualEpochsP



	unsigned int DataDim    = *DataDimensionP; // number of features per sample
	unsigned int NumSamples = *NumSamplesP;    // number of samples
	unsigned int NumEpochs  = *NumEpochsP;

	//printf("%d %d %d %d %d \n", DataDimension, HiddenDimension, FullDimension, LearningRate, Momentum);

	DataType  Inputs [MAX_DATA_SIZE * MAX_SAMPLES]; // an array of all samples with their features
	DataType  Weights[MAX_DATA_SIZE]; // number of features per sample - each feature associated with a weight
	DataType  Labels [MAX_SAMPLES]; // max number of samples
	DataType  Bias;
	DataType  Cost = (float)10.0;

//	#pragma HLS ARRAY_PARTITION variable=Inputs  type=complete
//	#pragma HLS ARRAY_PARTITION variable=Labels  type=complete
//	#pragma HLS ARRAY_PARTITION variable=Weights type=complete

	// copy inputs to local array

	CopyIntToDataTypeBuffers(DataInBuff,    Inputs,  DataDim * NumSamples);
	CopyIntToDataTypeBuffers(LabelsInBuff,  Labels,  NumSamples);
	CopyIntToDataTypeBuffers(WeightsInBuff, Weights, DataDim);

	float fbias;
	memcpy(&fbias, BiasP, sizeof(float));
	Bias = static_cast<DataType>(fbias);

	float fLearningRate;
	memcpy(&fLearningRate, LearningRateP, sizeof(float));
	DataType LearningRate = static_cast<DataType>(fLearningRate);

	// internal use
	DataType dw[MAX_DATA_SIZE];
	DataType db = static_cast<DataType>(0.0);

	unsigned int epochs = 0;

	do
	{
		#pragma HLS PIPELINE off

		Propagate(Inputs, Labels, Weights, &Bias, LearningRate, DataDim, NumSamples,
				  dw, &db, &Cost);

	}while ((++epochs <= NumEpochs) && ((float)Cost > 0.3));

	*actualEpochsP = epochs;
	#pragma HLS PIPELINE off

	CopyDataTypeToIntBuffers(Weights, WeightsInBuff, DataDim);

	float fBias = (float)Bias;
	memcpy(BiasP, &fBias, sizeof(float));

	float fCost = (float)Cost;
	memcpy(CostP, &fCost, sizeof(float));
}


void BackPropagate( DataType* Inputs, DataType* Labels,
					DataType *Weights, DataType* Bias, DataType LeaningRate,
					unsigned int NumFeatures, unsigned int NumSamples,
					DataType* A, DataType* dw, DataType* db)
{
	float fNumSamples = static_cast<float>(NumSamples);

    *db = 0.0;
    LOOP3:
    for (int j = 0; j < NumFeatures; j++)
    {
		#pragma HLS PIPELINE off

        dw[j] = 0.0;
        LOOP31:
        for (int i = 0; i < NumSamples; i++)
        {
			#pragma HLS PIPELINE off
            dw[j] += Inputs[i * NumFeatures + j] * (A[i] - Labels[i]); // Indexing into X
            *db += (A[i] - Labels[i]);
        }

		#ifdef __SYNTHESIS__
        dw[j] = hls::divide(dw[j], fNumSamples);
		#else
        dw[j] /= (DataType)NumSamples;
		#endif
    }


	#ifdef __SYNTHESIS__
    *db = hls::divide(*db, fNumSamples);
	#else
    *db /= (DataType)NumSamples;
	#endif

    LOOP5:
    for (int j = 0; j < NumFeatures; j++)
    {
		#pragma HLS PIPELINE off
        Weights[j] -= LeaningRate * dw[j];
    }
    *Bias -= LeaningRate * (*db);

}

// Inputs: data (NumFeatures, NumSamples) - now a 1D array
// Label: labels (1, NumSamples)  - now a 1D array
// w: weights (NumFeatures, 1) - now a 1D array
// b: bias (scalar)
// NumFeatures: number of features (e.g., NumFeatures * NumFeatures * 3 for an image)
// NumSamples: number of training samples
// dw: gradient of cost with respect to w (output, same shape as w) - 1D array
// db: gradient of cost with respect to b (output, scalar)
// cost: cost of the logistic regression (output, scalar)

void Propagate( DataType* Inputs, DataType* Labels,
				DataType *Weights, DataType* Bias, DataType LeaningRate,
				unsigned int NumFeatures, unsigned int NumSamples,
				DataType* dw, DataType* db, DataType* cost)
{
	int i, j;
    DataType z, a, sum;
	float fNumSamples = static_cast<float>(NumSamples);

    // Forward propagation
    DataType Prob[MAX_SAMPLES]; // Allocate up to 150 samples
    DataType Cost[MAX_SAMPLES];
    *cost = 0.0;

    LOOP1:
    for (int i = 0; i < NumSamples; i++)
    {
		#pragma HLS PIPELINE off
        z = *Bias;
        LOOP11:
        for (int j = 0; j < NumFeatures; j++)
        {
			#pragma HLS PIPELINE off
			#pragma HLS UNROLL off
            z += Weights[j] * Inputs[i * NumFeatures + j]; // Indexing into Inputs

        }


        Prob[i] = sigmoid(z);
        if (Prob[i] >= 1) Prob[i] = 0.999999; // protect 1-Prob[i] to be greater than 0 to avoid log nan
        if (Prob[i] <= 0) Prob[i] = 0.000001; // protect 1-Prob[i] to be greater than 0 to avoid log nan

        DataType cost1, cost2;
		#ifdef __SYNTHESIS__
        cost1 =       Labels[i] *  static_cast<DataType>(hls::log(    (Prob[i]).to_float()));
        cost2 =  (1 - Labels[i]) * static_cast<DataType>(hls::log(1 - (Prob[i]).to_float()));
		#else
        cost1 =       Labels[i] *  static_cast<DataType>(log(    Prob[i]));
        cost2 =  (1 - Labels[i]) * static_cast<DataType>(log(1 - Prob[i]));
		#endif
        *cost += -(cost1+cost2);
    }

	#ifdef __SYNTHESIS__
    *cost = hls::divide(*cost, fNumSamples);
	#else
    *cost = *cost / static_cast<DataType>(static_cast<float>(NumSamples));
	#endif

    // Backward propagation
    BackPropagate(Inputs, Labels, Weights, Bias, LeaningRate, NumFeatures, NumSamples, Prob, dw, db);

    printf(" %f ", (float)(*cost));
}

void CopyIntToDataTypeBuffers(unsigned int* From, DataType* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		#pragma HLS PIPELINE off

		memcpy(&fval, &From[i], sizeof(float));
		To[i] = static_cast<DataType>(fval);
	}
}

void CopyDataTypeToIntBuffers(DataType* From, unsigned int* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		#pragma HLS PIPELINE off

		fval = From[i];
		memcpy(&To[i], &fval, sizeof(float));
	}
}

void CopyFloatToDataTypeBuffers(float* From, DataType* To, unsigned int Dim)
{
	Loop3:
	float fval;
	for (int i = 0 ; i < Dim ; ++i)
	{
		To[i] = static_cast<DataType>(From[i]);
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

#ifdef __SYNTHESIS__
//The sigmoid function  can assume all values in the range ]0, 1[.
DataType sigmoid(DataType d)
{
	DataType Res;

	if (d > 0)
	{
		DataType e = hls::expf(-d);
		Res = static_cast<DataType>(1.0) / ( static_cast<DataType>(1.0) + e);
	}
	else
	{
		DataType e = hls::expf(d);
		Res = e /  ( static_cast<DataType>(1.0) + e);
	}
	return Res;
}
#else
//The sigmoid function  can assume all values in the range ]0, 1[.
DataType sigmoid(DataType d)
{
	DataType Res;

	if (d > 0)
	{
		DataType e = exp(-d);
		Res = static_cast<DataType>(1.0) / ( static_cast<DataType>(1.0) + e);
	}
	else
	{
		DataType e = exp(d);
		Res = e /  ( static_cast<DataType>(1.0) + e);
	}
	return Res;
}
#endif

DataType Relu(DataType x)
{
    return (x > 0.0) ? x : static_cast<DataType>(0.0); // Use ternary operator for ap_fixed
}

DataType squareError(DataType d1, DataType d2)
{
	DataType Diff = d1-d2;
	DataType P=2.0;

	//#pragma HLS PIPELINE off

	return Diff*Diff;
}

