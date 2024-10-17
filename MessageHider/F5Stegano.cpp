#include "F5Stegano.h"

F5Stegano::F5Stegano()
{
}

F5Stegano::~F5Stegano()
{
}

std::vector<std::vector<unsigned char>> F5Stegano::GetBlock(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, int index)
{
    /*std::vector<std::vector<unsigned char>> out(8 * nb_bytes_per_pixel, std::vector<unsigned char>(8 * nb_bytes_per_pixel));

    int block = 1;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int start = (block * nb_bytes_per_pixel * i) + ()
        }
    }*/

    return std::vector<std::vector<unsigned char>>();
}

std::vector<std::vector<float>> F5Stegano::ApplyDCT(const std::vector<std::vector<unsigned char>>& block)
{
    return std::vector<std::vector<float>>();
}

std::vector<std::vector<int>> F5Stegano::QuantizeDCT(const std::vector<std::vector<float>>& dctCoeffs)
{
    return std::vector<std::vector<int>>();
}

void F5Stegano::Encrypt(unsigned char* pixel_tab, const size_t len_pixel_tab, int nb_bytes_per_pixel, std::string word_to_hide)
{

}
