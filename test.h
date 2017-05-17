// Copyright PinaPL
//
// layer.hh
// PinaPL
//

#ifndef TEST_HH
#define TEST_HH


#include <stdlib.h>
#include <eigen3/Eigen/Dense>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "network.h"


Eigen::VectorXd letterToInput(char letter);
char inputToLetter(Eigen::VectorXd vector);

std::vector<Eigen::VectorXd> applyThreshold(std::vector<Eigen::VectorXd> outputs);

long compare(std::vector<Eigen::VectorXd> outputs, std::vector<Eigen::VectorXd> expectedOutputs);
long compareDouble(std::vector<Eigen::VectorXd> outputs, std::vector<Eigen::VectorXd> expectedOutputs);

void grammarLearn(bool symmetrical);

void grammarTestSimple(Network network, unsigned long wordCount);
void grammarTestDouble(Network network, unsigned long wordCount);

#endif
