#include "Conversion.h"
#include <iostream>
#include <bitset>

Conversion::Conversion()
{
}

std::vector<int> Conversion::WordToBytesTab(std::string word_to_hide)
{
	word_to_hide += 0x3;
	std::vector<int> result_tab;
	for (char letter : word_to_hide) {
		std::bitset<8> current_letter_bytes(static_cast<int>(letter));
		for (int i = 7; i >= 0; --i) {
			result_tab.push_back(current_letter_bytes[i]);
		}
	}
	return result_tab;
}

std::string Conversion::BytesTabToWord(std::vector<int> bytes_tab)
{
	std::string result;
	if (bytes_tab.size() % 8 != 0) {
		std::cerr << "Le nombre de bits n'est pas un multiple de 8" << std::endl;
		return "";
	}
	for (size_t i = 0; i < bytes_tab.size(); i += 8) {
		unsigned char byte = 0;
		for (size_t j = 0; j < 8; ++j) {
			byte = (byte << 1) | bytes_tab[i + j];
		}
		result += static_cast<char>(byte);
	}
	return result;
}
