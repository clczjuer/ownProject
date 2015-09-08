#include "NeuralNetwork.h"



CNeuralNetwork::CNeuralNetwork()
{
}


CNeuralNetwork::~CNeuralNetwork()
{
}

void CNeuralNetwork::Initialize(int nNodesInput, int nNodesHidden, int nNodesOutput)
{
	InputLayer.NumberOfNodes = nNodesInput;
	InputLayer.NumberOfChildNodes = nNodesHidden;
	InputLayer.NumberOfParentNodes = 0;
	InputLayer.Initialize(nNodesInput, NULL, &HiddenLayer);
	InputLayer.RandomizeWeights();
	HiddenLayer.NumberOfNodes = nNodesHidden;
	HiddenLayer.NumberOfChildNodes = nNodesOutput;
	HiddenLayer.NumberOfParentNodes = nNodesInput;
	HiddenLayer.Initialize(nNodesHidden, &InputLayer, &OutputLayer);
	HiddenLayer.RandomizeWeights();
	OutputLayer.NumberOfNodes = nNodesOutput;
	OutputLayer.NumberOfChildNodes = 0;
	OutputLayer.NumberOfParentNodes = nNodesHidden;
	OutputLayer.Initialize(nNodesOutput, &HiddenLayer, NULL);
}

void CNeuralNetwork::CleanUp()
{
	InputLayer.CleanUp();
	HiddenLayer.CleanUp();
	OutputLayer.CleanUp();
}

void CNeuralNetwork::SetInput(int i, double value)
{
	if ((i >= 0) && (i < InputLayer.NumberOfNodes))
	{
		InputLayer.NeuronValues[i] = value;
	}
}

double CNeuralNetwork::GetOutput(int i)
{
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes))
	{
		return OutputLayer.NeuronValues[i];
	}
	return (double)INT_MAX;
}

void CNeuralNetwork::SetDesiredOutput(int i, double value)
{
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes))
	{
		OutputLayer.DesiredValues[i] = value;
	}
}

void CNeuralNetwork::BackPropagate(void)
{
	OutputLayer.CalculateErrors();
	HiddenLayer.CalculateErrors();
	HiddenLayer.AdjustWeights();
	InputLayer.AdjustWeights();
}

void CNeuralNetwork::FeedForward(void)
{
	InputLayer.CalculateNeuronValues();
	HiddenLayer.CalculateNeuronValues(); OutputLayer.CalculateNeuronValues();
}

int CNeuralNetwork::GetMaxOutputID(void)
{
	int          i, id;
	double     maxval;
	maxval = OutputLayer.NeuronValues[0];
	id = 0;
	for (i = 1; i < OutputLayer.NumberOfNodes; i++)
	{
		if (OutputLayer.NeuronValues[i] > maxval)
		{
			maxval = OutputLayer.NeuronValues[i];
			id = i;
		}
	}
	return id;
}

double CNeuralNetwork::CalculateError(void)
{
	int          i;
	double     error = 0;
	for (i = 0; i < OutputLayer.NumberOfNodes; i++)
	{
		error += pow(OutputLayer.NeuronValues[i] -
			OutputLayer.DesiredValues[i], 2);////
	}
	error = error / OutputLayer.NumberOfNodes;
	return error;
}

void CNeuralNetwork::SetLearningRate(double rate)
{
	InputLayer.LearningRate = rate;
	HiddenLayer.LearningRate = rate;
	OutputLayer.LearningRate = rate;
}

void CNeuralNetwork::SetLinearOutput(bool useLinear)
{
	InputLayer.LinearOutput = useLinear;
	HiddenLayer.LinearOutput = useLinear;
	OutputLayer.LinearOutput = useLinear;
}

void CNeuralNetwork::SetMomentum(bool useMomentum, double factor)
{
	InputLayer.UseMomentum = useMomentum;
	HiddenLayer.UseMomentum = useMomentum;
	OutputLayer.UseMomentum = useMomentum;
	InputLayer.MomentumFactor = factor;
	HiddenLayer.MomentumFactor = factor;
	OutputLayer.MomentumFactor = factor;
}

void CNeuralNetwork::DumpData(char* filename)
{

}
