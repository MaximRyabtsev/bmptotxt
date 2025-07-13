/**
 * @brief Module for work with txt bitmap file.
 *
 */

#ifndef TXT_WRITER_HPP
#define TXT_WRITER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

/**
 * @brief For work with file you need use txtBitMap
 *
 */

struct txtBitMap final
{

    explicit txtBitMap(const std::string &locationAndName, int numberOfPages, int PageSize);

    void addNewPage(const std::vector<uint8_t> &page, bool lastFlag);

    void createFooter();

    ~txtBitMap();

private:
    std::ofstream file;

    int sizeOfPage;
};

#endif