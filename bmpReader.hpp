
/**
 * @brief
 *
 */

#ifndef _BMPREADER
#define _BMPREADER

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief First header of bmp file
 *
 */

#pragma pack(push, 1)
struct bmpFileHeader final
{
    uint16_t bfType;      ///< File signature: 'BM' (0x4D42)
    uint32_t bfSize;      ///< File size in bytes
    uint16_t bfReserved1; ///< Reserved (must be 0)
    uint16_t bfReserved2; ///< Reserved (must be 0)
    uint32_t bfOffBits;   ///< Offset to pixel data from file start

    bool isValid() const
    {
        return bfType == 0x4D42;
    }
};

/**
 * @brief Second header of bmp file
 *
 */

struct bmpInfoHeader final
{
    uint32_t biSize;         ///< Size of this header (40 bytes)
    int32_t biWidth;         ///< Image width in pixels
    int32_t biHeight;        ///< Image height in pixels (negative = top-down)
    uint16_t biPlanes;       ///< Must be 1
    uint16_t biBitCount;     ///< Bits per pixel (expected: 24)
    uint32_t biCompression;  ///< Compression type (0 = BI_RGB)
    uint32_t biSizeImage;    ///< Image size (can be 0 for BI_RGB)
    int32_t biXPelsPerMeter; ///< Horizontal resolution (pixels per meter)
    int32_t biYPelsPerMeter; ///< Vertical resolution (pixels per meter)
    uint32_t biClrUsed;      ///< Number of used colors (0 = all)
    uint32_t biClrImportant; ///< Number of important colors (0 = all)

    bool isValid() const
    {
        return biBitCount == 24 && biCompression == 0;
    }
};
#pragma pack(pop)

/**
 * @brief * Represents a 24-bit uncompressed BMP image as a binary mask:
 *      white pixels are treated as background (0), others as foreground (1).
 *
 */

struct bmpFile final
{

    explicit bmpFile(std::string &&fileLocation) : fileLocation(std::move(fileLocation))
    {
        if (!readFileHeaders())
        {
            std::cerr << "Invalid BMP header in " << fileLocation << std::endl;
            abort();
        }
    }

    explicit bmpFile(std::string &fileLocation) : fileLocation(fileLocation)
    {
        if (!readFileHeaders())
        {
            std::cerr << "Invalid BMP header in " << fileLocation << std::endl;
            abort();
        }
    }

    uint32_t getWidth() const { return width; }

    uint32_t getHeight() const { return height; }

    const std::vector<uint8_t> &getGrayscaleData() const { return data; }

private:
    bmpFileHeader fileHeader;
    bmpInfoHeader infoHeader;

    std::string fileLocation;

    uint32_t height = 0;
    uint32_t width = 0;

    std::vector<uint8_t> data; ///< Pixels. Whites = 0, another = 1

    bool readFileHeaders()
    {
        std::ifstream file(fileLocation, std::ios::binary);
        if (!file.is_open())
            return false;

        if (!file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader)))
            return false;
        if (!fileHeader.isValid())
            return false;

        if (!file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader)))
            return false;
        if (!infoHeader.isValid())
            return false;

        width = infoHeader.biWidth;
        height = std::abs(infoHeader.biHeight);
        bool flip = infoHeader.biHeight > 0;

        const int rowSize = ((width * 3 + 3) / 4) * 4;
        std::vector<uint8_t> rawPixels(rowSize * height);
        file.seekg(fileHeader.bfOffBits);
        file.read(reinterpret_cast<char *>(rawPixels.data()), rawPixels.size());

        auto isWhite = [](uint8_t r, uint8_t g, uint8_t b)
        {
            return r > 250 && g > 250 && b > 250;
        };

        for (int y = 0; y < height; ++y)
        {
            int srcY = flip ? (height - 1 - y) : y;
            for (int x = 0; x < width; ++x)
            {
                int srcIdx = srcY * rowSize + x * 3;
                uint8_t b = rawPixels[srcIdx];
                uint8_t g = rawPixels[srcIdx + 1];
                uint8_t r = rawPixels[srcIdx + 2];
                data[y * width + x] = isWhite(r, g, b) ? 0 : 1;
            }
        }

        file.close();
        return true;
    }
};

#endif
