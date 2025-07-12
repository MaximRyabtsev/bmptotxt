#ifndef BMP_READER_HPP
#define BMP_READER_HPP

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief First header of a BMP file
 */
#pragma pack(push, 1)
struct bmpFileHeader final
{
    uint16_t bfType = 0;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;

    bool isValid() const;
};

/**
 * @brief Second header of a BMP file
 */
struct bmpInfoHeader final
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount = 0;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

    bool isValid() const;
};
#pragma pack(pop)

/**
 * @brief Represents a BMP file as a binary mask: white pixels = 0, others = 1
 */
struct bmpFile final
{
    explicit bmpFile(std::string &&fileLocation);
    explicit bmpFile(std::string &fileLocation);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    const std::vector<uint8_t> &getData() const;

    void rebuildFile(std::string &newFileLocation);
    void rebuildFile(std::string &&newFileLocation);

private:
    bool readFileHeaders();

    bmpFileHeader fileHeader;
    bmpInfoHeader infoHeader;

    std::string fileLocation;
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t> data; // 0 = white pixel, 1 = other
};

#endif // BMP_READER_HPP
