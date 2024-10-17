#pragma once
#include "AlgoStegano.h"
#include <vector>

class F5Stegano : public AlgoStegano
{
public:

	F5Stegano();
	virtual ~F5Stegano();

protected:

protected:

	std::vector<std::vector<unsigned char>> GetBlock(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, int index);
	std::vector<std::vector<float>> ApplyDCT(const std::vector<std::vector<unsigned char>>& block);
	std::vector<std::vector<int>> QuantizeDCT(const std::vector<std::vector<float>>& dctCoeffs);

public:
	void Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide) override;


};

