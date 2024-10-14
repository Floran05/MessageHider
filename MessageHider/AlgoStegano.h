#pragma once
#include <vector>
#include <string>

class AlgoStegano
{
public:
	AlgoStegano();

	unsigned char* pixel_tab;
	std::string word_to_hide;

	std::vector<int> WordToBytesTab(std::string word_to_hide);
	void MainStegano(unsigned char* pixel_tab, const size_t len_tab, std::vector<int> bytes_tab);
	virtual ~AlgoStegano() = default;
};

