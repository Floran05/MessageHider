#include "AlgoStegano.h"
#include <iostream>

AlgoStegano::AlgoStegano()
{
}

void AlgoStegano::MainStegano(unsigned char* pixel_tab, const size_t len)
{

	for (std::size_t i = 0; i < len; i += 3) {
		//pixel_tab[i] &= 0b11111110;
	}
}

