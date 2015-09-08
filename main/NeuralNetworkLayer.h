#ifndef _NEURAL_NETWORK_LAYER_H
#define _NEURAL_NETWORK_LAYER_H

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
class CNeuralNetworkLayer
{
public:
	CNeuralNetworkLayer();
	~CNeuralNetworkLayer();
public:
	int               NumberOfNodes;  //层中神经元数目
	int               NumberOfChildNodes;//子层神经元数目
	int               NumberOfParentNodes;//父层神经元数目
	double**          Weights;//s权值数组
	double**          WeightChanges;//权值改变数组
	double*           NeuronValues;//神经元值
	double*           DesiredValues;// 导师信号
	double*           Errors;//误差
	double*           BiasWeights;//偏差权值
	double*           BiasValues;
	double            LearningRate;// 学习效率
	bool              LinearOutput;//是否线性输出
	bool              UseMomentum;//否有动力因素
	double            MomentumFactor;//有动力因素的话,则动力因素大小值
	CNeuralNetworkLayer*     ParentLayer;//父层
	CNeuralNetworkLayer*     ChildLayer;//子层
	
	void     Initialize(int NumNodes,
	CNeuralNetworkLayer* parent,
	CNeuralNetworkLayer* child);
	void     CleanUp(void);
	void     RandomizeWeights(void);
	void     CalculateErrors(void);
	void     AdjustWeights(void);
	void     CalculateNeuronValues(void);

};

#endif