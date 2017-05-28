// Copyright PinaPL
//
// network.cpp
// PinaPL
//

#include "network.h"


Network::Network(unsigned long inputCount, unsigned long layerCount, unsigned long outputCount) {
	this->inputCount = inputCount + 1;
	this->layerCount = layerCount;
	this->outputCount = outputCount;
	learningRate = 0.1;
	
	this->weights = 0.1 * Eigen::MatrixXd::Random(this->layerCount, this->inputCount + this->layerCount);
	
	this->allInputs = Eigen::VectorXd::Zero(this->inputCount + this->layerCount);
	this->allOutputs = Eigen::VectorXd::Zero(this->layerCount);
	this->gradients = std::vector<Eigen::MatrixXd>(this->layerCount, Eigen::MatrixXd::Zero(this->layerCount, this->inputCount + this->layerCount));
	this->newGradients = std::vector<Eigen::MatrixXd>(this->layerCount, Eigen::MatrixXd::Zero(this->layerCount, this->inputCount + this->layerCount));
	
	this->weightChange = Eigen::MatrixXd::Zero(this->layerCount, this->inputCount + this->layerCount);
}

void Network::propagate(Eigen::VectorXd inputs) {
	assert(inputs.size() == this->inputCount - 1);
	this->allInputs <<
		this->allOutputs,
		inputs,
		1.0;
	this->allOutputs = (this->weights * allInputs).unaryExpr(&sigmoid);
}

void Network::backpropagate(Eigen::VectorXd expectedOutputs) {
	double value = 0;
	Eigen::VectorXd antecedents = this->allOutputs.unaryExpr(&sigmoidDerivative);

// For each weight:
	for (unsigned long i = 0; i < this->layerCount; i++) {
		for (unsigned long j = 0; j < this->inputCount + this->layerCount; j++) {
// First: calculate new gradients
			for (unsigned long k = 0; k < this->layerCount; k++) {
				value = 0;
				for (unsigned long n = 0; n < this->layerCount; n++) {
					value += this->weights(k, n) * this->gradients.at(n)(i, j);
				}
				if (i == k)
					value += this->allInputs(j);
				value *= antecedents(k);
				this->newGradients.at(k)(i, j) = value;
			}

// Second: calculate delta
			value = 0;
			for (unsigned long k = 0; k < this->outputCount; k++) {
				value += (expectedOutputs(k) - allOutputs(k)) * this->newGradients.at(k)(i, j);
			}
			this->weightChange(i, j) += learningRate * value;
		}
	}
	this->gradients = this->newGradients;
}

void Network::updateWeigths() {
	this->weights += this->weightChange;
	this->weightChange -= this->weightChange;
	for (unsigned long k = 0; k < this->gradients.size(); k++)
		this->gradients.at(k) -= this->gradients.at(k);
}

void Network::setLearningRate(double newLearningRate) {
	this->learningRate = newLearningRate;
}

Eigen::VectorXd Network::getOutputs() {
	return this->allOutputs.block(0, 0, this->outputCount, 1);
}

Eigen::MatrixXd Network::getWeights() {
	return this->weights;
}

Eigen::VectorXd Network::getAllOutputs() {
	return this->allOutputs;
}
