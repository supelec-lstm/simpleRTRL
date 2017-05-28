// Copyright PinaPL
//
// layer.hh
// PinaPL
//

#ifndef NETWORK_H
#define NETWORK_H


#include <eigen3/Eigen/Dense>
#include <vector>

#include "functions.h"


class Network {
 public:
	unsigned long inputCount;
	unsigned long layerCount;
	unsigned long outputCount;
	double learningRate;
	
	Eigen::MatrixXd weights;
	Eigen::VectorXd allInputs;
	Eigen::VectorXd allOutputs;
	std::vector<Eigen::MatrixXd> gradients;
	std::vector<Eigen::MatrixXd> newGradients;
	Eigen::MatrixXd weightChange;
	
	Network(unsigned long inputCount, unsigned long layerCount, unsigned long outputCount);
	
	void propagate(Eigen::VectorXd inputs);
	void backpropagate(Eigen::VectorXd expectedOutputs);
	void updateWeigths();
	
	void setLearningRate(double newLearningRate);
	
	Eigen::VectorXd getOutputs();
	Eigen::MatrixXd getWeights();
	Eigen::VectorXd getAllOutputs();
};

#endif
