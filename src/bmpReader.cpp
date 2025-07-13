#include "bmpReader.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

bool bmpFileHeader::isValid() const
{
    return bfType == 0x4D42;
}

bool bmpInfoHeader::isValid() const
{
    return biBitCount == 32 && (biCompression == 0 || biCompression == 3);
}

bmpFile::bmpFile(std::string &&fileLocation)
    : fileLocation(std::move(fileLocation))
{
    if (!readFileHeaders())
    {
        std::cerr << "Invalid BMP header in " << this->fileLocation << std::endl;
        abort();
    }
}

bmpFile::bmpFile(std::string &fileLocation)
    : fileLocation(fileLocation)
{
    if (!readFileHeaders())
    {
        std::cerr << "Invalid BMP header in " << this->fileLocation << std::endl;
        abort();
    }
}

uint32_t bmpFile::getWidth() const
{
    return width;
}

uint32_t bmpFile::getHeight() const
{
    return height;
}

const std::vector<uint8_t> &bmpFile::getData() const
{
    return data;
}

void bmpFile::rebuildFile(std::string &newFileLocation)
{
    fileLocation = newFileLocation;
    infoHeader.biBitCount = 0;
    fileHeader.bfType = 0;
    data.clear();
    if (!readFileHeaders())
    {
        std::cerr << "Invalid BMP header in " << this->fileLocation << std::endl;
        abort();
    }
}

void bmpFile::rebuildFile(std::string &&newFileLocation)
{
    fileLocation = std::move(newFileLocation);
    infoHeader.biBitCount = 0;
    fileHeader.bfType = 0;
    data.clear();
    if (!readFileHeaders())
    {
        std::cerr << "Invalid BMP header in " << this->fileLocation << std::endl;
        abort();
    }
}

bool bmpFile::readFileHeaders()
{
    std::ifstream file(fileLocation, std::ios::binary);
    if (!file.is_open())
        return false;

    if (!file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader)))
        return false;
    std::cout << "bfType = " << std::hex << fileHeader.bfType << std::dec << std::endl;
    if (!fileHeader.isValid())
        return false;

    if (!file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader)))
        return false;
    std::cout << "biBitCount = " << infoHeader.biBitCount << std::endl;
    std::cout << "biCompression = " << infoHeader.biCompression << std::endl;
    if (!infoHeader.isValid())
        return false;

    width = infoHeader.biWidth;
    height = std::abs(infoHeader.biHeight);
    bool flip = infoHeader.biHeight > 0;

    int bytesPerPixel = infoHeader.biBitCount / 8;
    const int rowSize = ((width * bytesPerPixel + 3) / 4) * 4;
    std::vector<uint8_t> rawPixels(rowSize * height);
    file.seekg(fileHeader.bfOffBits);
    file.read(reinterpret_cast<char *>(rawPixels.data()), rawPixels.size());

    auto isWhite = [](uint8_t r, uint8_t g, uint8_t b)
    {
        return r > 250 && g > 250 && b > 250;
    };

    data.resize(width * height);
    for (int y = 0; y < height; ++y)
    {
        int srcY = flip ? (height - 1 - y) : y;
        for (int x = 0; x < width; ++x)
        {
            int srcIdx = srcY * rowSize + x * bytesPerPixel;
            uint8_t b = rawPixels[srcIdx];
            uint8_t g = rawPixels[srcIdx + 1];
            uint8_t r = rawPixels[srcIdx + 2];
            data[y * width + x] = isWhite(r, g, b) ? 0 : 1;
        }
    }

    file.close();
    return true;
}
