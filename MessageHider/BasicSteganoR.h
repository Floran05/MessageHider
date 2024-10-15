#pragma once
#include "AlgoStegano.h"

class BasicSteganoR : public AlgoStegano
{
public:
	BasicSteganoR();

	unsigned char* pixel_tab;
	std::string word_to_hide;

	void Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide) override;
	std::string Decrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel) override;
};

