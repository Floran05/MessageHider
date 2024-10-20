#include "BasicSteganoR.h"
#include "Conversion.h"
#include "JournalManager.h"

#include <iostream>
#include <Windows.h>
#include <string>

BasicSteganoR::BasicSteganoR()
{
}

int BasicSteganoR::MessageMaxLenght(const size_t len_pixel_tab, int nb_bytes_per_pixel)
{
	return static_cast<int>(len_pixel_tab / 8);
}

void BasicSteganoR::Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide)
{
	JournalManager::Instance->LogWrite(L"Start of encryption with the basic algorithm...");
	std::vector<int> bytes_tab = Conversion::WordToBytesTab(word_to_hide);
	if (MessageMaxLenght(len_pixel_tab, nb_bytes_per_pixel) < bytes_tab.size()) {
		std::cerr << "The message is too long, thus can't be encrytped." << std::endl;
		JournalManager::Instance->LogWarning(L"The message is too long, thus can't be encrytped.");
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
			JournalManager::Instance->LogWrite(L"End of encryption.");
			return;
		}
	}
	JournalManager::Instance->LogWrite(L"End of encryption.");
}

std::string BasicSteganoR::Decrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel)
{
	JournalManager::Instance->LogWrite(L"Start of decryption with the basic algorithm...");
	std::vector<int> bytes_tab;
	for (std::size_t i = 0; i < len_pixel_tab; i += nb_bytes_per_pixel) {
		int last_bit = pixel_tab[i] & 1;
		bytes_tab.push_back(last_bit);
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
			JournalManager::Instance->LogWrite(L"End of decryption.");
			return Conversion::BytesTabToWord(bytes_tab);
		}
	}
	JournalManager::Instance->LogWarning(L"No end of message found.");
	return "No end of message found.";
}
