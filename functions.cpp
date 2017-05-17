// Copyright PinaPL
//
// functions.cpp
// PinaPL
//

#include <math.h>
#include <iostream>

#include "functions.h"


double sigmoid(double x) {
	return (1.0 / (1.0 + exp(-x)));
}

double sigmoidFast(double x) {
	return (0.5 * tanh(x) + 0.5);
}

double sigmoidVeryFast(double x) {
	return (0.5 * x / (1.0 + fabs(x)) + 0.5);
}

double sigmoidDerivative(double x) {
	return x * (1 - x);
}

double tanh2(double x) {
	double y = tanh(x);
	return y * y;
}

double tanhDerivative(double x) {
	double y = tanh(x);
	return 1 - y * y;
}

double threshold(double x) {
	return (double)(x > 0.3);
}

std::string openFile(bool dual) {
	if (dual)
		return("symmetrical_reber_train_2.4M.txt");
	else
		return("reber_train_2.4M.txt");
}

void printMatrix(Eigen::MatrixXd matrix) {
#if DEBUG
	for (unsigned long i = 0; i < matrix.rows(); i++) {
		for (unsigned long j = 0; j < matrix.cols(); j++) {
			std::cout << matrix(i, j) << ", ";
		}
		std::cout << std::endl;
	}
#endif
}

void printVector(Eigen::VectorXd vector) {
#if DEBUG
	for (unsigned long i = 0; i < vector.size(); i++) {
		std::cout << vector(i) << ", ";
	}
	std::cout << std::endl;
#endif
}

void printVectors(Eigen::VectorXd vector1, Eigen::VectorXd vector2) {
#if DEBUG
	for (unsigned long i = 0; i < vector1.size(); i++) {
		std::cout << vector1(i) << ", " << vector2(i) << std::endl;
	}
#endif
}
