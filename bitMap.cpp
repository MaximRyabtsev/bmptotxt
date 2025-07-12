#include "bitMap.hpp"
#include <vector>

bitMap::bitMap(const std::vector<uint8_t> &data)
{
    bitMapData.reserve(data.size() / 8 + 1);
    createBitMap(data);
}

void bitMap::rebuildBitMap(const std::vector<uint8_t> &data)
{
    bitMapData.clear();
    createBitMap(data);
}

const std::vector<uint8_t> &bitMap::getData() const
{
    return bitMapData;
}

void bitMap::createBitMap(const std::vector<uint8_t> &data)
{
    uint8_t counter = 0;
    uint8_t box = 0;
    for (uint8_t pixel : data)
    {
        box |= (pixel << counter);
        ++counter;
        if (counter == 8)
        {
            bitMapData.push_back(box);
            counter = 0;
            box = 0;
        }
    }
    if (counter != 0)
        bitMapData.push_back(box);
}