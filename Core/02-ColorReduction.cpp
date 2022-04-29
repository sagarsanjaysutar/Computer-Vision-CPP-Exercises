/*!
 * @brief Color Reduction and Lookup table for faster performance
 * @author Sagar Sutar
 * @link https://docs.opencv.org/3.4/db/da5/tutorial_how_to_scan_images.html
 */

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>

cv::Mat &ScanImageAndReduceC(cv::Mat &I, const uchar *table);
void printTable(const uchar (&table)[256]);

int main()
{
    std::string imgPath("/home/sagar/Pictures/doge.jpg"); // Path to image.
    int divideWith = 30;                                  // Number to divide the image pixels with.
    uchar table[256];                                     // Table containing a particular pixel value for a range of pixel values e.g. 0..10=0, 10..19=10, 20-29= 20, etc.
    cv::Mat I, J;

    I = cv::imread(imgPath, cv::ImreadModes::IMREAD_COLOR);

    if (I.empty())
    {
        std::cout << "Invalid image. Try another one." << std::endl;
        return -1;
    }

    // Initialize the table
    for (int pixelValue = 0; pixelValue < 256; ++pixelValue)
    {
        table[pixelValue] = (uchar)((pixelValue / divideWith) * divideWith); // Formula derived from OpenCV Site.
    }

    printTable(table);
    cv::imshow("Original images", I);

    cv::imshow("Reduced images", ScanImageAndReduceC(I, table));
    cv::waitKey(0);

    return 0;
}
/*!
    @brief Reduces the image pixel intensity
    @param Mat &I: reference to the Mat I in caller function. (Pass by reference as its a class and contains huge data)
    @param const uchar* table: constant uchar table value. (Pass by pointer as it's a primitive type)
    @return Mat &: Reference of mutated image matrix.
    There isn't a need to return as the we are mutating a reference var. in the func., which by definition mutates the caller function's originally passed var. argumenet.
*/
cv::Mat &ScanImageAndReduceC(cv::Mat &I, const uchar *table)
{
    CV_Assert(I.depth() == CV_8U);

    int nRows = I.rows;
    int nCols = I.cols * I.channels();

    // If the Matrix is stored in a continuos manner in the memory i.e. row0: BGR-BGR-..row1: BGR-BGR.., put it single row for easier read.
    if (I.isContinuous())
    {
        nCols = nCols * nRows;
        nRows = 1;
    }

    int i, j;
    uchar *p;
    for (i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i); // Pointer extracted from reference var. I

        for (j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]]; // Pixel value is replaced with the new table value e.g. table[189] => 180
        }
    }

    return I;
}

/*!
    @brief Prints the table array.
    @param const uchar (&table)[256]: A constant uchar reference of table.

    It's written this way because of array decay.
    Check out https://www.geeksforgeeks.org/what-is-array-decay-in-c-how-can-it-be-prevented/
*/
void printTable(const uchar (&table)[256])
{
    int tableSize = (sizeof(table) / sizeof(table[0]));
    std::stringstream sstream;
    for (int i = 0; i < tableSize; ++i)
    {
        if (i != 0)
            sstream << ",";

        sstream << (uint)table[i];
    }
    std::string tableString = sstream.str();
    std::cout << "[" << tableString << "]" << std::endl
              << "Size: " << tableSize << std::endl;
}