#include "txtWriter.hpp"
#include <string>
#include <vector>
#include <iostream>

txtBitMap::txtBitMap(const std::string &locationAndName, int numberOfPages, int sizeOfPage) : sizeOfPage(sizeOfPage)
{
    file.open(locationAndName, std::ios::out);
    if (!file.is_open())
    {
        std::cerr << "Invalid location" << std::endl;
        abort();
    }
    file << "First pixel in animation[0][0] last in animation[last picture][last pixel].\n";
    file << "char animation[" << std::to_string(numberOfPages) << "][" << std::to_string(sizeOfPage) << "] = {";
}

void txtBitMap::addNewPage(const std::vector<uint8_t> &page, bool lastFlag) const
{
    if (sizeOfPage != page.size())
    {
        std::cerr << "sizeOfPage != page.size()" << std::endl;
        abort();
    }

    file << "{";

    for (int i = 0; i < sizeOfPage; ++i)
    {
        file << std::to_string(page[i]);
        if (i != (sizeOfPage - 1))
            file << ",";
    }

    file << "}";
    if (lastFlag == false)
        file << ",";
}

void txtBitMap::createFooter() const
{
    file << "};\n";
}

txtBitMap::~txtBitMap()
{
    file.close();
}