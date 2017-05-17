// Copyright PinaPL
//
// functions.hpp
// PinaPL
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <string>
#include <eigen3/Eigen/Dense>


double sigmoid(double x);
double sigmoidFast(double x);
double sigmoidVeryFast(double x);
double sigmoidDerivative(double x);

double tanh2(double x);
double tanhDerivative(double x);

double threshold(double x);

std::string openFile(bool dual);

void printMatrix(Eigen::MatrixXd matrix);
void printVector(Eigen::VectorXd vector);
void printVectors(Eigen::VectorXd vector1, Eigen::VectorXd vector2);

#endif
