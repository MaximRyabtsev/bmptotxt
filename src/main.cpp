#include <iostream>
#include "bmpReader.hpp"
#include "bitMap.hpp"
#include "txtWriter.hpp"

#include <string>
#include <vector>
#include <fstream>

int main()
{

    int numberOfFiles;
    std::string bmpFileLocation, txtFile;

    std::cout << "Set bmp file location \n";
    std::cin >> bmpFileLocation;

    std::cout << "Set number of files\n";
    std::cin >> numberOfFiles;

    std::cout << "Set location and name of txt file\n";
    std::cin >> txtFile;

    bmpFile bmpfile_(bmpFileLocation + std::to_string(0) + ".bmp");
    bitMap bitmap_(bmpfile_.getData());
    txtBitMap txtbitmap_(txtFile, numberOfFiles, bitmap_.getData().size());
    txtbitmap_.addNewPage(bitmap_.getData(), 0);

    for (int i = 1; i < numberOfFiles; ++i)
    {
        bmpfile_.rebuildFile(bmpFileLocation + std::to_string(i) + ".bmp");
        bitmap_.rebuildBitMap(bmpfile_.getData());
        if (i == numberOfFiles - 1)
        {
            txtbitmap_.addNewPage(bitmap_.getData(), 1);
            break;
        }
        txtbitmap_.addNewPage(bitmap_.getData(), 0);
    }

    txtbitmap_.createFooter();

    std::cout << "Text file created successfully: " << txtFile << std::endl;
}