#pragma once

#include "Utils.h"

#pragma once

#include <vector>

class PerlinNoise {

public:
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);

	// Get a noise value, for 2D images z can have any value
	double GenerateNoise(double x, double y, double z);

private:

	double Fade(double t);
	double Lerp(double t, double a, double b);
	double Gradient(int hash, double x, double y, double z);

private:
	std::vector<int> p;

};
