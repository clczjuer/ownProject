#include "NeuralNetworkLayer.h"
#include "math.h"
class CNeuralNetwork
{
public:
	CNeuralNetwork();
	~CNeuralNetwork();
	  public:
		 CNeuralNetworkLayer     InputLayer;
		 CNeuralNetworkLayer     HiddenLayer;
		  CNeuralNetworkLayer     OutputLayer;
     void     Initialize(int nNodesInput, int nNodesHidden,
                              int nNodesOutput);
     void     CleanUp();
     void     SetInput(int i, double value);
     double GetOutput(int i);
     void     SetDesiredOutput(int i, double value);
     void     FeedForward(void);
     void     BackPropagate(void);
     int      GetMaxOutputID(void);
     double CalculateError(void);
     void     SetLearningRate(double rate);
     void     SetLinearOutput(bool useLinear);
     void     SetMomentum(bool useMomentum, double factor);
     void     DumpData(char* filename);
};

