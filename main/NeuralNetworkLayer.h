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
	int               NumberOfNodes;  //������Ԫ��Ŀ
	int               NumberOfChildNodes;//�Ӳ���Ԫ��Ŀ
	int               NumberOfParentNodes;//������Ԫ��Ŀ
	double**          Weights;//sȨֵ����
	double**          WeightChanges;//Ȩֵ�ı�����
	double*           NeuronValues;//��Ԫֵ
	double*           DesiredValues;// ��ʦ�ź�
	double*           Errors;//���
	double*           BiasWeights;//ƫ��Ȩֵ
	double*           BiasValues;
	double            LearningRate;// ѧϰЧ��
	bool              LinearOutput;//�Ƿ��������
	bool              UseMomentum;//���ж�������
	double            MomentumFactor;//�ж������صĻ�,�������ش�Сֵ
	CNeuralNetworkLayer*     ParentLayer;//����
	CNeuralNetworkLayer*     ChildLayer;//�Ӳ�
	
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