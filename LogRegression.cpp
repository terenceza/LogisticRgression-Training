
#include "LogRegression.hpp"
#include <algorithm>
#include <memory>

#define Epsilon 0.00000000000000000000000000001

void LogRegression(DataType* DataInP, DataType* LabelsInP,
		 	 	   DataType* WeightsP, DataType* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   DataType* dw, DataType* db, DataType* cost
				 )
{
	#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	#pragma HLS INTERFACE mode=m_axi port=DataInP    offset=slave bundle=gmem
	#pragma HLS INTERFACE mode=m_axi port=LabelsInP  offset=slave bundle=gmem
	#pragma HLS INTERFACE mode=m_axi port=WeightsP   offset=slave bundle=gmem
    #pragma HLS INTERFACE mode=s_axilite port=BiasP
    #pragma HLS INTERFACE mode=s_axilite port=DataDimensionP
	#pragma HLS INTERFACE mode=s_axilite port=NumSamplesP
    #pragma HLS INTERFACE mode=s_axilite port=dw
    #pragma HLS INTERFACE mode=s_axilite port=db
	#pragma HLS INTERFACE mode=s_axilite port=cost

	#pragma HLS INTERFACE mode=s_axilite port=DataInP
	#pragma HLS INTERFACE mode=s_axilite port=LabelsInP

	unsigned int DataDim    = *DataDimensionP; // number of features per sample
	unsigned int NumSamples = *NumSamplesP;    // number of samples
	unsigned int LabelsDim  = NumSamples;      // 1 label per sample

	//printf("%d %d %d %d %d \n", DataDimension, HiddenDimension, FullDimension, LearningRate, Momentum);


	DataType Inputs[MAX_DATA_SIZE * 150]; // an array of all samples with their features
	DataType Weights[MAX_DATA_SIZE]; // number of features per sample - each feature associated with a weight
	DataType Labels[150]; // max number of samples

	#pragma HLS ARRAY_PARTITION variable=Inputs  type=complete
	#pragma HLS ARRAY_PARTITION variable=Labels  type=complete
	#pragma HLS ARRAY_PARTITION variable=Weights type=complete


	// copy inputs to local array
	//printf("\n Inputs : \n");
	for (int i = 0 ; i < DataDim * NumSamples; ++i)
	{
		Inputs[i] = DataInP[i];
		//printf("%f ", Inputs[i]);
	}


	//printf("\n Weights : \n");

	for (int w = 0 ; w < DataDim; ++w)
	{
		Weights[w] = WeightsP[w];
		//printf("%f ", Weights[w]);

	}

	//printf("\n Labels : \n");

	for (int l = 0 ; l < NumSamples; ++l)
	{
		Labels[l] = LabelsInP[l];
		//printf("%f ", Labels[l]);

	}

	int Count = 0;
	//DataType dw, db, cost = 0;

	for (int epochs = 0; epochs < MAX_EPOCHS; ++epochs)
	{
		Propagate(Inputs, Labels, Weights, *BiasP, DataDim, NumSamples,
				  dw, db, cost);

        for (int j = 0; j < 4; j++)
        {
            Weights[j] -= 0.01 * dw[j];
        }
        *BiasP -= 0.01 * (*db);

		//#ifndef __SYNTHESIS__
			//if ((epochs % 1000) == 0)
				printf("%d %f %f %f \n", ++Count, *dw, *db, *cost);
		//#endif

	}

	//printf("Delta %f \n", Error);
}


void Propagate( DataType* Inputs, DataType* Labels,
				DataType *w, DataType b,
				unsigned int num_features, unsigned int num_samples,
				DataType* dw, DataType* db, DataType* cost)
{

    // Inputs: data (num_features, num_samples) - now a 1D array
    // Label: labels (1, num_samples)  - now a 1D array
    // w: weights (num_features, 1) - now a 1D array
    // b: bias (scalar)
    // num_features: number of features (e.g., num_features * num_features * 3 for an image)
    // num_samples: number of training samples
    // dw: gradient of cost with respect to w (output, same shape as w) - 1D array
    // db: gradient of cost with respect to b (output, scalar)
    // cost: cost of the logistic regression (output, scalar)

    int i, j;
    DataType z, a, sum;

    //printf("%d features, %d samples \n", num_features, num_samples);

    // Forward propagation
    DataType Cost = 0.0;
    DataType A[MAX_SAMPLES]; // Allocate up to 150 samples


    for (i = 0; i < num_samples; i++)
    {
    	//printf("sample %d \n", i);
        z = b;
        for (j = 0; j < num_features; j++)
        {
        	//printf("feature (%d,%d) Input: %f Weight: %f \n", i,j, Inputs[i * num_features + j],  w[j]);

            z += w[j] * Inputs[i * num_features + j]; // Indexing into Inputs

        }
        A[i] = sigmoid(z);
        //printf("feature (%d,%d) Input: %f  z: %f sigmoid: %f \n", i,j, Inputs[i * num_features + j],  z, A[i]);
    }
    *cost = 0.0;
    for (int i = 0; i < num_samples; i++)
    {
    	Cost = -((log(A[i])) * Labels[i] + (log(1 - A[i])) * (1 - Labels[i]));
        //printf("cost = %f \n", Cost);
        *cost += Cost;

    }

    *cost = *cost / num_samples;
    printf("Final Cost  = %f \n",  *cost);

    // Backward propagation
    *db = 0.0;
    for (j = 0; j < num_features; j++)
    {
        dw[j] = 0.0;
        for (i = 0; i < num_samples; i++)
        {
            dw[j] += Inputs[i * num_features + j] * (A[i] - Labels[i]); // Indexing into X
            *db += (A[i] - Labels[i]);
        }
        dw[j] /= num_samples;
        printf("dw[%d] = %f ", j, dw[j]);

    }
    printf("\n");

    *db = 0.0;
    for (int i = 0; i < num_samples; i++)
    {
        *db += (A[i] - Labels[i]);
    }
    *db /= num_samples;
    printf("db = %f \n", *db);

}





#ifdef __SYNTHESIS__

//The sigmoid function  can assume all values in the range ]0, 1[.
DataType sigmoid(DataType d)
{
	DataType Res = 1.0 / ( 1.0 + exp(-d));
	//printf("Res(%f) = %f", d, Res);

	return 1.0 / ( 1.0 + exp(-d));
}

DataType squareError(DataType d1, DataType d2)
{
	DataType Diff = d1-d2;
	DataType P=2.0;

	//#pragma HLS PIPELINE off

	return Diff*Diff; //hls::pow(Diff, P);
}

DataType sigmoidDerivation(DataType d)
{
	//#pragma HLS PIPELINE off

	return d * (static_cast<DataType>(1.0) - d);
}

//DataType Relu(DataType d)
//{
//
//	return hls::fmax(0, d);
//}
//
//DataType ReluDerivation(DataType d)
//{
//	return d >= 0.0 ? 0.0 : 1.0;
//}

#else

//The sigmoid function  can assume all values in the range ]0, 1[.
DataType sigmoid(DataType d)
{
	DataType Res = 1.0 / ( 1.0 + exp(-d));
	//printf("Res(%f) = %f", d, Res);

	return 1.0 / ( 1.0 + exp(-d));
}

DataType squareError(DataType d1, DataType d2)
{
	DataType Diff = d1-d2;
	DataType P=2.0;

	return pow(Diff, P);
}

DataType sigmoidDerivation(DataType d)
{
	return d * (static_cast<DataType>(1.0) - d);
}

//ReLU activation function for the output layer is well suited for
//cases when the input observations xi assume a wide range of positive
//real value
DataType Relu(DataType d)
{
	return std::fmax(0, d);
}

DataType ReluDerivation(DataType d)
{
	return d >= 0.0 ? 0.0 : 1.0;
}
#endif
