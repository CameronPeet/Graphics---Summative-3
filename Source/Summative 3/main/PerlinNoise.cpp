#include "PerlinNoiseTess.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::GenerateNoise(double x, double y, double z) {

	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = Lerp(w, Lerp(v, Lerp(u, Gradient(p[AA], x, y, z), Gradient(p[BA], x - 1, y, z)), Lerp(u, Gradient(p[AB], x, y - 1, z), Gradient(p[BB], x - 1, y - 1, z))), Lerp(v, Lerp(u, Gradient(p[AA + 1], x, y, z - 1), Gradient(p[BA + 1], x - 1, y, z - 1)), Lerp(u, Gradient(p[AB + 1], x, y - 1, z - 1), Gradient(p[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;

}

double PerlinNoise::Fade(double t) {

	return t * t * t * (t * (t * 6 - 15) + 10);

}

double PerlinNoise::Lerp(double t, double a, double b) {

	return a + t * (b - a);

}

double PerlinNoise::Gradient(int hash, double x, double y, double z) {

	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);

}