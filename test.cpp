// Copyright PinaPL
//
// test.cpp
// PinaPL
//

#include "test.h"


// Auxiliary functions needed to test

// Convert a letter into a legitimate input
Eigen::VectorXd letterToInput(char letter) {
    Eigen::VectorXd input(7);
    switch (letter) {
        case 'B':
            input << 1, 0, 0, 0, 0, 0, 0;
            break;
        case 'T':
            input << 0, 1, 0, 0, 0, 0, 0;
            break;
        case 'P':
            input << 0, 0, 1, 0, 0, 0, 0;
            break;
        case 'S':
            input << 0, 0, 0, 1, 0, 0, 0;
            break;
        case 'X':
            input << 0, 0, 0, 0, 1, 0, 0;
            break;
        case 'V':
            input << 0, 0, 0, 0, 0, 1, 0;
            break;
        case 'E':
            input << 0, 0, 0, 0, 0, 0, 1;
            break;
    }
    return input;
}

// Convert an output filtered into a letter (useful to debug)
char inputToLetter(Eigen::VectorXd vector) {
    Eigen::VectorXd B(7);
    B << 1, 0, 0, 0, 0, 0, 0;
    Eigen::VectorXd T(7);
    T << 0, 1, 0, 0, 0, 0, 0;
    Eigen::VectorXd P(7);
    P << 0, 0, 1, 0, 0, 0, 0;
    Eigen::VectorXd S(7);
    S << 0, 0, 0, 1, 0, 0, 0;
    Eigen::VectorXd X(7);
    X << 0, 0, 0, 0, 1, 0, 0;
    Eigen::VectorXd V(7);
    V << 0, 0, 0, 0, 0, 1, 0;
    Eigen::VectorXd E(7);
    E << 0, 0, 0, 0, 0, 0, 1;

    char letter = 0;
    if (vector == B) letter = 'B';
    else if (vector == T) letter = 'T';
    else if (vector == P) letter = 'P';
    else if (vector == S) letter = 'S';
    else if (vector == X) letter = 'X';
    else if (vector == V) letter = 'V';
    else if (vector == E) letter = 'E';
    else letter = '*';
	
    return letter;
}

// Apply a threshold : if value > 0.3 then value =1, else value = 0
std::vector<Eigen::VectorXd> applyThreshold(std::vector<Eigen::VectorXd> outputs) {
    for (size_t i = 0; i < outputs.size(); i++) {
        outputs.at(i) = outputs.at(i).unaryExpr(&threshold);
    }
    return(outputs);
}

// Used to evaluate the preditcted transitions by the network and the expected transition
// Simple reber grammar
long compareSimple(std::vector<Eigen::VectorXd> outputs, std::vector<Eigen::VectorXd> expectedOutputs) {
    long score = 0;
	Eigen::VectorXd diff;
    bool transitionPredicted = true;
	
    // for each VectorXd
    for (unsigned long i = 0; i < outputs.size(); i++) {
        // We compare the state predicted and the next state
        diff = outputs.at(i) - expectedOutputs.at(i);
        transitionPredicted = true;
        for (unsigned long j = 0; j < diff.size(); j++) {
            // if one of the coordinates is <0 there is a transition not predicted
            if (diff(j) < 0)
				transitionPredicted = false;
        }
        // If we did not found any error, we score
        if (transitionPredicted)
			score += 1;
    }
    // Checks if we preditected ALL the transitions
	return (long)(score == outputs.size());
}

// Same thing for the double reber grammar
long compareDouble(std::vector<Eigen::VectorXd> outputs, std::vector<Eigen::VectorXd> expectedOutputs) {
    long score = 0;
    Eigen::VectorXd diff;
    bool transitionPredicted = true;

    // We compare the last state predicted and the first transition
    diff = outputs.at(outputs.size() - 2) - expectedOutputs.at(outputs.size() - 2);
    for (unsigned long j = 0; j < diff.size(); j++) {
        // if one of the coordinates is <0 there is a transition not predicted
        if (fabs(diff(j)) > 0.1) {
            transitionPredicted = false;
        }
    }
	// If we did not found any error, we score
    if (transitionPredicted)
		score = 1;
    return score;
}


// Learn a grammar
void grammarLearn(bool symmetrical) {
    unsigned long inputSize = 7;
    unsigned long outputSize = 7;
	unsigned long layerSize = 30;
    unsigned long batchCount = 2000;
    unsigned long batchSize = 50;

    Network network = Network(inputSize, layerSize, outputSize);

    std::ifstream file(openFile(symmetrical));
    std::string inputString;

    // random offset in data set
    unsigned long offset = arc4random_uniform(10000);
    for (unsigned long i = 0; i < offset; i++)
        std::getline(file, inputString);

    // std::cout << "===== Beginnning of Learning =====" << '\n';
    for (unsigned long batch = 0; batch < batchCount; batch++) {
        // std::cout << "batch no"<< batch;
        std::cout << batch;
		long remainingWords = batchSize;
		while (std::getline(file, inputString) && remainingWords > 0) {
			//std::cout << inputString << std::endl;
            for (unsigned long i = 0; i < inputString.length() - 1; i++) {
				network.propagate(letterToInput(inputString.at(i)));
				network.backpropagate(letterToInput(inputString.at(i + 1)));
				//printVector(letterToInput(inputString.at(i)));
				//printVector(network.getOutputs());
            }
			//printVector(letterToInput(inputString.back()));
			//std::cout << std::endl;
			network.updateWeigths();
            remainingWords -= 1;
        }
        if (symmetrical)
            grammarTestDouble(network, 1000);
        else
            grammarTestSimple(network, 1000);
    }
}

void grammarTestSimple(Network network, unsigned long wordCount) {
    std::ifstream file("reber_test_1M.txt");
    std::string inputString;
    long score = 0;
    long remainingWords = wordCount;

	unsigned long offset = arc4random_uniform(10000);
    for (unsigned long i = 0; i < offset; i++)
        std::getline(file, inputString);

    while (std::getline(file, inputString) && remainingWords > 0) {
		std::vector<Eigen::VectorXd> allOutputs = std::vector<Eigen::VectorXd>();
		std::vector<Eigen::VectorXd> expectedOutputs = std::vector<Eigen::VectorXd>();
		for (unsigned long i = 0; i < inputString.length() - 1; ++i) {
			network.propagate(letterToInput(inputString.at(i)));
			allOutputs.push_back(network.getOutputs());
			expectedOutputs.push_back(letterToInput(inputString.at(i + 1)));
		}
		score += compareSimple(applyThreshold(allOutputs), expectedOutputs);
        remainingWords -= 1;
    }
	
    double scorePercent = ((double)100.0 * score / wordCount);
    std::cout << "," <<scorePercent << '\n';
}

void grammarTestDouble(Network network, unsigned long wordCount) {
    std::ifstream file("symmetrical_reber_test_1M.txt");
    std::string inputString;
	long score = 0;
	long remainingWords = wordCount;

	unsigned long offset = arc4random_uniform(10000);
	for (unsigned long i = 0; i < offset; i++)
		std::getline(file, inputString);

	while (std::getline(file, inputString) && remainingWords > 0) {
		std::vector<Eigen::VectorXd> allOutputs = std::vector<Eigen::VectorXd>();
		std::vector<Eigen::VectorXd> expectedOutputs = std::vector<Eigen::VectorXd>();
		for (unsigned long i = 0; i < inputString.length() - 1; ++i) {
			network.propagate(letterToInput(inputString.at(i)));
			allOutputs.push_back(network.getOutputs());
			expectedOutputs.push_back(letterToInput(inputString.at(i + 1)));
		}
		score += compareDouble(applyThreshold(allOutputs), expectedOutputs);
		remainingWords -= 1;
	}

	double scorePercent = ((double)100.0 * score / wordCount);
	std::cout << "," <<scorePercent << '\n';
}
