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


int main()
{
	const std::string filename("irisdata.txt");
    std::vector<unsigned int> data;
    std::vector<unsigned int> label;

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
        		int   ival;
        		DataType fval = std::stof(value);
        		memcpy(&ival, &fval, sizeof(int));
            	if (valueCount < 4)
            	{
            		data.push_back(ival);
            	}
            	else
            	{
            		label.push_back(ival);
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
    unsigned int WeightsIn[4];
    DataType fWeightsIn[] = { 0.1, 0.1, 0.1, 0.1 };

    CopyDataTypeToIntBuffers(fWeightsIn, WeightsIn, 4);

    for (int i = 0; i < 4; i++)
    {
    	printf("w%d = %f ", i, fWeightsIn[i]);
    }


    DataType fBias = 0.1;
    unsigned int Bias;

	memcpy(&Bias, &fBias, sizeof(int));
	printf("\n Bias = %f \n", fBias);

	DataType fCost = 0;
	unsigned int Cost = 0;

    unsigned int DataDim = 4;
	unsigned int TrainingSize = MAX_SAMPLES;

	DataType fLearningRate = 0.02;
    unsigned int LearningRate;
	memcpy(&LearningRate, &fLearningRate, sizeof(int));

	unsigned int NumEpochs = 500;
	unsigned int ActualEpochs = 0;
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0 ; i < 1 ; ++i)
	{
		LogRegression(data.data(), label.data(),
					  WeightsIn, &Bias,
					  &DataDim, &TrainingSize,
					  &LearningRate, &NumEpochs,
					  &Cost, &ActualEpochs);
	}

	auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count();
	 std::cout << "RUNTIME : "
			   << stop <<" ms " << std::endl;

	 CopyIntToDataTypeBuffers(WeightsIn, fWeightsIn, 4);

	for (int i = 0; i < 4; i++)
	{
		printf("wo%d = %f ", i, fWeightsIn[i]);
	}

	 memcpy(&fBias, &Bias, sizeof(int));
	 printf("\n Updated Bias = %f \n", fBias);

	 memcpy(&fCost, &Cost, sizeof(int));
	 printf("\n Updated Cost = %f \n", fCost);

	 printf("Actual Epochs : %d", ActualEpochs);
}
