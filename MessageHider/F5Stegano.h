#pragma once
#include "AlgoStegano.h"
#include <vector>

class F5Stegano : public AlgoStegano
{

public:

	std::vector<std::vector<unsigned char>> GetBlock(int index);

};

