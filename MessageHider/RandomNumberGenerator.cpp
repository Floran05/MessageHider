#include "RandomNumberGenerator.h"
#include <random>

RandomNumberGenerator::RandomNumberGenerator()
{
}

int RandomNumberGenerator::RandomIntGenerator()
{
	int result;
	std::random_device rd;  // Crée une source de graine aléatoire (basée sur le matériel)
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);
	result = distrib(gen);
	return result;
}
