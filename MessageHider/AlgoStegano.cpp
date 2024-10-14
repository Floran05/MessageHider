#include "AlgoStegano.h"
#include <iostream>
#include <bitset>

AlgoStegano::AlgoStegano()
{
}

std::vector<int> AlgoStegano::WordToBytesTab(std::string word_to_hide)
{
	std::vector<int> result_tab;
	for (char letter : word_to_hide) {
		std::bitset<8> current_letter_bytes(static_cast<int>(letter));
		for (int i = 7; i >= 0; --i) {
			result_tab.push_back(current_letter_bytes[i]);
		}
	}
	return result_tab;
}

void AlgoStegano::MainStegano(unsigned char* pixel_tab, const size_t len_tab, std::vector<int> bytes_tab)
{
		unsigned char temp_pixel;
		for (std::size_t i = 0; i < len_tab; i += 3) {
			temp_pixel = pixel_tab[i] & 0b11111110;
			temp_pixel |= bytes_tab[i];
		}
}
