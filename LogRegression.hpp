#ifndef AUTOENCODER_HPP
#define AUTOENCODER_HPP

#include <ap_fixed.h>

#ifdef __SYNTHESIS__
	#include <hls_math.h>
#else
	#include <cmath>
#endif

#define MAX_DATA_SIZE 4
#define MAX_HIDDEN_SIZE 10
#define MAX_FULL_DIM (MAX_DATA_SIZE * MAX_HIDDEN_SIZE)
#define MAX_WEIGHTS_SIZE (MAX_DATA_SIZE * MAX_HIDDEN_SIZE)
#define MAX_EPOCHS 1000
#define MAX_SAMPLES 150

//#ifdef __SYNTHESIS__
//	typedef ap_fixed<1, 16, AP_RND_ZERO> DataType, CoeffType;
//#else
	typedef float DataType;
	typedef double CoeffType;
//#endif

void LogRegression(DataType* DataInP, DataType* LabelsInP,
				   DataType* WeightsP, DataType* BiasP,
				   unsigned int* DataDimensionP, unsigned int* NumSamplesP,
				   DataType* dw, DataType* db, DataType* cost
				 );

void Propagate( DataType* Inputs, DataType* Labels,
				DataType *w, DataType b,
				unsigned int num_px, unsigned int num_samples,
				DataType* dw, DataType* db, DataType* cost);

DataType sigmoid(DataType d);
DataType sigmoidDerivation(DataType d);
DataType squareError(DataType d1, DataType d2) ;
//DataType Relu(DataType d);
//DataType ReluDerivation(DataType d);

//void backpropagate();

#endif
