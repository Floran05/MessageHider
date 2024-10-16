#include "ComplexStegano.h"
#include "Conversion.h"

#include <iostream>
#include <Windows.h>
#include <string>

ComplexStegano::ComplexStegano()
{
}

int ComplexStegano::MessageMaxLenght(const size_t len_pixel_tab, int nb_bytes_per_pixel)
{
	return len_pixel_tab * nb_bytes_per_pixel;
}

void ComplexStegano::Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide)
{
	std::vector<int> bytes_tab = Conversion::WordToBytesTab(word_to_hide);
	if (MessageMaxLenght(len_pixel_tab, nb_bytes_per_pixel) < bytes_tab.size()) {
		std::cerr << "Le message est trop grand et ne peut pas �tre encrypt� dans l'image." << std::endl;
		return;
	}
	std::vector<int>::iterator bytes_iterator = bytes_tab.begin();
	for (std::size_t i = 0; i < len_pixel_tab; i += nb_bytes_per_pixel)
	{
		pixel_tab[i] &= 0b11111110;
		pixel_tab[i] |= *bytes_iterator;
		bytes_iterator++;
		if (bytes_iterator == bytes_tab.end())
		{
			return;
		}

		pixel_tab[len_pixel_tab - i - 4] &= 0b11111110;
		pixel_tab[len_pixel_tab - i - 4] |= *bytes_iterator;
		bytes_iterator++;
		if (bytes_iterator == bytes_tab.end())
		{
			return;
		}
	}
}

std::string ComplexStegano::Decrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel)
{
	std::vector<int> bytes_tab;
	for (std::size_t i = 0; i < len_pixel_tab; i += nb_bytes_per_pixel) {
		int previous_first_bit = pixel_tab[i] & 1;
		bytes_tab.push_back(previous_first_bit);
		int previous_last_bit = pixel_tab[len_pixel_tab - i - 4] & 1;
		bytes_tab.push_back(previous_last_bit);
		if (bytes_tab.size() > 8
			&& bytes_tab.size() % 8 == 0
			&& bytes_tab[bytes_tab.size() - 8] == 0
			&& bytes_tab[bytes_tab.size() - 7] == 0
			&& bytes_tab[bytes_tab.size() - 6] == 0
			&& bytes_tab[bytes_tab.size() - 5] == 0
			&& bytes_tab[bytes_tab.size() - 4] == 0
			&& bytes_tab[bytes_tab.size() - 3] == 0
			&& bytes_tab[bytes_tab.size() - 2] == 1
			&& bytes_tab[bytes_tab.size() - 1] == 1)
		{
			return Conversion::BytesTabToWord(bytes_tab);
		}
	}
	return Conversion::BytesTabToWord(bytes_tab);
}
