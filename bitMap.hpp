/**
 * @brief Program for packaging vector of pixels in bit map. First pixel in bitMapData[0] in bit 0.
 *
 */

#ifndef BIT_MAP_HPP
#define BIT_MAP_HPP

#include <cstdint>
#include <vector>

/**
 * @brief Struct for creating and keeping bit map
 *
 */

struct bitMap final
{
    explicit bitMap(const std::vector<uint8_t> &data);

    void rebuildBitMap(const std::vector<uint8_t> &data);

    const std::vector<uint8_t> &getData() const;

private:
    std::vector<uint8_t> bitMapData;

    void createBitMap(const std::vector<uint8_t> &data);
};

#endif