#pragma once
#include <vector>
#include <string>

class Conversion
{
public:
	Conversion();

	static std::vector<int> WordToBytesTab(std::string word_to_hide);
	static std::string BytesTabToWord(std::vector<int> bytes_tab);

	virtual ~Conversion() = default;
};

