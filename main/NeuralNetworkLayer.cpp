#include "NeuralNetworkLayer.h"



CNeuralNetworkLayer::CNeuralNetworkLayer()
{
	ParentLayer = NULL;
	ChildLayer = NULL;
	LinearOutput = false;
	UseMomentum = false;
	MomentumFactor = 0.9;
}


CNeuralNetworkLayer::~CNeuralNetworkLayer()
{
}

void CNeuralNetworkLayer::Initialize(int NumNodes, CNeuralNetworkLayer* parent, CNeuralNetworkLayer* child)
{
	int     i, j;
	// 分配内存
	NeuronValues = (double*)malloc(sizeof(double) *		NumberOfNodes);
	DesiredValues = (double*)malloc(sizeof(double) *		NumberOfNodes);
	Errors = (double*)malloc(sizeof(double) * NumberOfNodes);
	if (parent != NULL)
	{
		ParentLayer = parent;
	}
	if (child != NULL)
	{
		ChildLayer = child;
		Weights = (double**)malloc(sizeof(double*) *	NumberOfNodes);
		WeightChanges = (double**)malloc(sizeof(double*) *			NumberOfNodes);
		for (i = 0; i < NumberOfNodes; i++)
		{
			Weights[i] = (double*)malloc(sizeof(double) *   NumberOfChildNodes);
			WeightChanges[i] = (double*)malloc(sizeof(double) *      NumberOfChildNodes);
		}
		BiasValues = (double*)malloc(sizeof(double) *			NumberOfChildNodes);
		BiasWeights = (double*)malloc(sizeof(double) *			NumberOfChildNodes);
	}
	else {
		Weights = NULL;
		BiasValues = NULL;
		BiasWeights = NULL;
		WeightChanges = NULL;
	}
	// 确保所有归 0
	for (i = 0; i < NumberOfNodes; i++)
	{
		NeuronValues[i] = 0;
		DesiredValues[i] = 0;
		Errors[i] = 0;
		if (ChildLayer != NULL)
			for (j = 0; j < NumberOfChildNodes; j++)
			{
				Weights[i][j] = 0;
				WeightChanges[i][j] = 0;
			}
	}
	// Initialize the bias values and weights
	if (ChildLayer != NULL)
		for (j = 0; j < NumberOfChildNodes; j++)
		{
			BiasValues[j] = -1;
			BiasWeights[j] = 0;
		}
}

void CNeuralNetworkLayer::CleanUp(void)
{
	int i;
	free(NeuronValues);
	free(DesiredValues);
	free(Errors);
	if (Weights != NULL)
	{
		for (i = 0; i < NumberOfNodes; i++)
		{
			free(Weights[i]);
			free(WeightChanges[i]);
		}
		free(Weights);
		free(WeightChanges);
	}
	if (BiasValues != NULL) free(BiasValues);
	if (BiasWeights != NULL) free(BiasWeights);
}

void CNeuralNetworkLayer::RandomizeWeights(void)
{
	int     i, j;
	int     min = 0;
	int     max = 200;
	int     number;
	//srand((unsigned)time(NULL));
	for (i = 0; i < NumberOfNodes; i++)
	{
		for (j = 0; j < NumberOfChildNodes; j++)
		{
			number = (((abs(rand()) % (max - min + 1)) + min));
			if (number > max)
				number = max;
			if (number < min)
				number = min;
			Weights[i][j] = number / 100.0f - 1;
		}
	}
	for (j = 0; j < NumberOfChildNodes; j++)
	{
		number = (((abs(rand()) % (max - min + 1)) + min));
		if (number > max)
			number = max;
		if (number < min)
			number = min;
		BiasWeights[j] = number / 100.0f - 1;
	}
}

void CNeuralNetworkLayer::CalculateErrors(void)
{
	int i, j;
	double sum;
	if (ChildLayer == NULL) // output layer
	{
		for (i = 0; i < NumberOfNodes; i++)
		{
			Errors[i] = (DesiredValues[i] - NeuronValues[i]) *
				NeuronValues[i] *
				(1.0f - NeuronValues[i]);
		}
	}
	else if (ParentLayer == NULL) { // 输入层
		for (i = 0; i < NumberOfNodes; i++)
		{
			Errors[i] = 0.0f;
		}
	}
	else { // 隐层
		for (i = 0; i < NumberOfNodes; i++)
		{
			sum = 0;
			for (j = 0; j < NumberOfChildNodes; j++)
			{
				sum += ChildLayer->Errors[j] * Weights[i][j];
			}
			Errors[i] = sum * NeuronValues[i] *
				(1.0f - NeuronValues[i]);
		}
	}
}

void CNeuralNetworkLayer::CalculateNeuronValues(void)
{
	int i, j;
	double x;
	if (ParentLayer != NULL)
	{
		for (j = 0; j < NumberOfNodes; j++)
		{
			x = 0;
			for (i = 0; i < NumberOfParentNodes; i++)
			{
				x += ParentLayer->NeuronValues[i] *
					ParentLayer->Weights[i][j];
			}
			x += ParentLayer->BiasValues[j] *
				ParentLayer->BiasWeights[j];
			if ((ChildLayer == NULL) && LinearOutput)
				NeuronValues[j] = x;
			else
				NeuronValues[j] = 1.0f / (1 + exp(-x));
		}
	}
}

void CNeuralNetworkLayer::AdjustWeights(void)
{
	int          i, j;
	double       dw;
	if (ChildLayer != NULL)
	{
		for (i = 0; i < NumberOfNodes; i++)
		{
			for (j = 0; j < NumberOfChildNodes; j++)
			{
				dw = LearningRate * ChildLayer->Errors[j] *
					NeuronValues[i];
				if (UseMomentum)
				{
					Weights[i][j] += dw + MomentumFactor *
						WeightChanges[i][j];
					WeightChanges[i][j] = dw;
				}
				else {
					Weights[i][j] += dw;
				}
			}
		}
		for (j = 0; j < NumberOfChildNodes; j++)
		{
			BiasWeights[j] += LearningRate *
				ChildLayer->Errors[j] *
				BiasValues[j];
		}
	}
}
