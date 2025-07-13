// написать шапку надо впихнуть в массив (массив картинок состоящий из массива пикселей) + показать скольпикселей

// написать футтер (типо окончание записи чтобы поставил ласт ; и тд) так как хранить все картинки в оперативе пк будет многовпто

// нужно дописывать!

// *написать функцию для чтения или хотя бы гайд чтобы в дальнейшем было проще

/**
 * @brief Module for work with txt bitmap file.
 *
 */

#ifndef TXT_WRITER_HPP
#define TXT_WRITER_HPP

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief For work with file you need use txtBitMap
 *
 */

struct txtBitMap final
{

    explicit txtBitMap(const std::string &locationAndName, unsigned numberOfPages, unsigned PageSize);

    void addNewPage(const std::vector<uint8_t> &page, bool lastFlag) const;

    void createFooter() const;

    ~txtBitMap();

private:
    std::ofstream file;

    int sizeOfPage;
};

#endif