#include "RandomNumberGenerator.h"
#include <random>

RandomNumberGenerator::RandomNumberGenerator()
{
}

int RandomNumberGenerator::RandomIntGenerator()
{
	int result;
	std::random_device rd;  // Cr�e une source de graine al�atoire (bas�e sur le mat�riel)
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);
	result = distrib(gen);
	return result;
}
