#include "../src/LogRegression.hpp"
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <cerrno>
#include <stdlib.h>
#include <chrono>

using namespace std;

#define RAND_MAX 0x7fff

void FullPrint(	DataType* InputValues, DataType* HiddenValues,
				unsigned int DataDimension, unsigned int HiddenDimension,
				DataType* Weights,
				DataType* WeightsChanges,
				DataType* PrevWeightsChanges)

{
	cout.precision(10);
	cout << fixed;

	cout << "Encoder:" << endl;
	cout << "Inputs Values: " << endl;

	for (int i = 0; i < DataDimension; i++)
	{
		printf(" InputValues[%d] = %f \n", i, InputValues[i]);
	}
	cout << endl;
	cout << "Encoder Weights: " << endl;

	for (auto d = 0; d < DataDimension; d++)
	{
		for (auto h = 0; h < HiddenDimension; h++)
		{
			cout <<	Weights[d*HiddenDimension + h] << ", "
				 << WeightsChanges[d*HiddenDimension + h] << ", "
				 << PrevWeightsChanges[d*HiddenDimension + h] << endl;
		}
	}
	cout << "Hidden Values: " << endl;
	for (auto i = 0; i < HiddenDimension; i++)
	{
		printf(" HiddenValues[%d] = %f \n", i, HiddenValues[i]);
	}

	cout << endl;

}


vector<DataType *> ReadData(string path)
{
	std::vector<DataType *>data;
	ifstream file(path, ios::in);
	if (!file.is_open())
	{
		cerr << "Iris data file could not be read" << endl;
		return data;
	}

	string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);
		vector<string> tokens;
		DataType *d = new DataType[5];
		DataType *dptr = d;
		DataType value;
		for (string s; ss >> value;)
		{
			if (ss.peek() == ',')
			{
				ss.ignore();
			}
			*dptr++ = value;
		}
		data.push_back(d);
	}

	// lets normalize data for the sigmoid function need all values in the range ]0, 1[.

	DataType max = std::numeric_limits<DataType>::max();
	DataType min = std::numeric_limits<DataType>::min();
	DataType mins[5] = { max, max, max, max, max};
	DataType maxes[5] = { min, min, min, min, max};

	for (auto row : data)
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (row[i] > maxes[i])
			{
				maxes[i] = row[i];
			}

			if (row[i] < mins[i])
			{
				mins[i] = row[i];
			}
		}

//		printf("%f %f %f %f ", row[0], row[1], row[2], row[3]);
//		printf("\n");
	}
	for (auto row : data)
	{
		for (size_t i = 0; i < 5; i++)
		{
			row[i] = (row[i] - mins[i]) / (maxes[i] - mins[i]);
		}

//		printf("%f %f %f %f ", row[0], row[1], row[2], row[3]);
//		printf("\n");
	}
	return data;
}


void Report(DataType Error)
{
	cout.precision(10);
	cout << fixed;
	cout << "Delta: " << Error << endl;

	//printf("Error = %f \n", Error);
}

int main()
{
	const std::string filename("irisdata.txt");
    std::vector<DataType> data;
    std::vector<DataType> label;

    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return {}; // Return an empty vector to indicate failure
    }

    int lineCount = 0;
    while (std::getline(file, line) && lineCount < MAX_SAMPLES) // Read line by line, limit to 100 lines
    {
        std::stringstream ss(line);
        std::string value;
        int valueCount = 0;
        //printf("\n");
        while (std::getline(ss, value, ',') && valueCount < 5) // Read values separated by commas, limit to 4
        {
            try
            {
            	if (valueCount < 4)
            	{
            		data.push_back(std::stof(value)); // Convert string to float and add to data
            	}
            	else
            	{
            		label.push_back(std::stof(value));// Convert string to float and add to label
            	}
                //printf("%f ", std::stof(value));
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: Invalid number format in file " << filename << " at line " << lineCount + 1 << ", value: " << value << std::endl;
                // Consider how you want to handle this error: skip the value, skip the row, or stop reading.
                // Here, we skip the value and continue to the next.
            }
            valueCount++;
        }

        lineCount++;
    }
    if (lineCount < MAX_SAMPLES)
    {
        std::cerr << "Warning: File " << filename << " has less than 100 lines. Read " << lineCount << " lines." << std::endl;
    }
    file.close();

	// push 1 weight per feature
    DataType Weights[4];
    for (int i = 0; i < 4; i++)
    {
    	Weights[i] = (0.1 * (i + 1));
    }

    DataType bias = 0.0;
    DataType learningRate = 0.01;

	auto start = std::chrono::high_resolution_clock::now();
	DataType db, cost = 0;
	DataType dw[4];
    unsigned int DataDim = 4;
	unsigned int TrainingSize = MAX_SAMPLES;


	LogRegression(data.data(), label.data(),
				  Weights, &bias,
				  &DataDim, &TrainingSize,
				  dw, &db, &cost);

	auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count();
	 std::cout << "RUNTIME : "
			   << stop <<" ms " << std::endl;

}

#if 0
int  main()
{
	unsigned int DataDimension = MAX_DATA_SIZE;


	DataType Input[MAX_DATA_SIZE];
	int Label[MAX_DATA_SIZE];
	DataType Weights[MAX_FULL_DIM];

	vector<DataType *> Data = ReadData(string("irisdata.txt"));

	std::cout << "Something is working here" << std::endl;

	std::random_shuffle(Data.begin(), Data.end());


	printf("\n");

	//int epochs = 100000;
	int epochs = 100000;

	unsigned int TrainingSize = 100;
	CoeffType LearningRate = 0.1;
	CoeffType Momentum = 0.9;

	//for (auto i = 0; i < epochs; i++)
	{
		std::vector<DataType> DataVec;
		std::vector<DataType> LabelVec;
		std::vector<DataType> Weights;
		DataType Error = 0.0;

		for (int count = 0; count < TrainingSize; count++)
		{
			DataType *row = Data.at(count);
			//DataType Input[4] = {row[0], row[1], row[2], row[3]};
			printf("row[0] = %f %f %f %f \n", row[0], row[1], row[2], row[3]);
			DataVec.push_back(row[0]);
			DataVec.push_back(row[1]);
			DataVec.push_back(row[2]);
			DataVec.push_back(row[3]);
			LabelVec.push_back(row[4]);


			//printf(" Row %d : %f %f %f %f \n ", count, row[0], row[1], row[2], row[3]);

		}

		// push 1 weight per feature
	    for (int i = 0; i < 4; i++)
	    {
	    	Weights.push_back(0.1 * (i + 1));
	    }

	    DataType bias = 0.0;
	    unsigned int DataDim = 4;

		auto start = std::chrono::high_resolution_clock::now();
		DataType db, dw, cost = 0;

		LogRegression(DataVec.data(), LabelVec.data(),
					  Weights.data(), &bias,
					  &DataDim, &TrainingSize,
					  &dw, &db, &cost);

		auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count();
		 std::cout << "RUNTIME : "
				   << stop <<" ms " << std::endl;

		printf("dw = %f  db = %f  cost = %f", (float)dw, (float)db, (float)cost);
		//if (i % 1000 == 0)
		{
			//printf("i = %d Error = %f\n", i, Error);
			Report(Error);
		}
	}

//	FullPrint(	Input, Output,
//				DataDimension,  HiddenDimension,
//				Weightss,
//				Weights,
//				Weights );

	printf("End .... \n");
	return 0;
}
#endif

