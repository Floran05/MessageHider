#pragma once
#include <string>

class AlgoStegano
{
public:
	AlgoStegano();

	unsigned char* pixel_tab;
	std::string word_to_hide;

	virtual void Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide) = 0;
	virtual std::string Decrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel) = 0;
	virtual int MessageMaxLenght(const size_t len_pixel_tab, int nb_bytes_per_pixel) = 0;
	virtual ~AlgoStegano() = default;
};
