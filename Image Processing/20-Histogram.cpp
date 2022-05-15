/*!
 * @brief Histogram of both original and histogram-equalized image is shown
 * @note Personal notes: https://www.notion.so/Filtering-598385a4c98c441f80faf7139f2e81f6
 * @link Reference code: https://github1s.com/opencv/opencv/blob/3.4/samples/cpp/tutorial_code/Histograms_Matching/calcHist_Demo.cpp
 * @link Tutorials: https://docs.opencv.org/3.4/d4/d1b/tutorial_histogram_equalization.html
 */
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using std::cout;
using std::endl;

class Histogram
{
public:
    explicit Histogram();
    ~Histogram();

    cv::RNG m_rng;         // Random number generator
    int m_width, m_height; // Default dimentions

    cv::Mat getHistGraphImg(cv::Mat img);     // Returns histogram graph of the image
    cv::Mat getHistEqualizedImg(cv::Mat img); // Returns a histogram-equalized image
    cv::Scalar getRandomColor();              // Helper function to return random number
};

Histogram::Histogram() : m_rng(1),
                         m_height(500),
                         m_width(500)

{
}

Histogram::~Histogram() {}

cv::Scalar Histogram::getRandomColor()
{
    int icolor = (unsigned)m_rng;
    return cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

cv::Mat Histogram::getHistGraphImg(cv::Mat img)
{
    // Empty histogram image
    cv::Mat histImg(m_height, m_width, CV_8UC3, cv::Scalar(0, 0, 0));

    if (img.channels() > 1)
    {
        cout << "Please pass a gray scale image." << endl;
        return histImg;
    }

    // #00 - Compute histogram
    int numberOfBin = 256;
    float range[] = {0, 256};
    const float *histRange[] = {range}; // A bit strange way to declare an array. Refer cv::calcHist doc.
    cv::Mat hist;
    cv::calcHist(
        &img,         // Single channeled Image
        1,            // No. of images
        0,            // ?? Refer doc.
        cv::Mat(),    // Empty mask
        hist,         // Output histogram
        1,            // Dimentions of histogram
        &numberOfBin, // Number of bin(vertical bars)
        histRange     // Histogram range.
    );

    int bWidth = cvRound((double)m_width / numberOfBin);

    // #01 - Process / normalize the Histogram to (0, histogramImage.rows).
    // If you don't normalize, the graph will blow out of proportion i.e. will go outside the defined height(no. of rows).
    // Try commenting this part to see the result.
    cv::normalize(
        hist,                       // Source
        hist,                       // Output
        0,                          // Alpha
        histImg.rows,               // Beta
        cv::NormTypes::NORM_MINMAX, // Normalization type
        -1,                         // Datatype
        cv::Mat()                   // Mask
    );

    // #02 - Plot it on an image.
    for (int binIdx = 1; binIdx < numberOfBin; binIdx++)
    {
        // Each bin will be colored different, for understanding purposes.
        // The formula for start and end are intuitive. Refer notes.
        cv::Point start(bWidth * (binIdx - 1), m_height - cvRound(hist.at<float>(binIdx - 1)));
        cv::Point end(bWidth * (binIdx), m_height - cvRound(hist.at<float>(binIdx)));
        cv::line(histImg, start, end, cv::Scalar(221, 221, 221), 1, cv::LineTypes::LINE_AA);
        // cout << start << "\t" << end << endl;
    }

    return histImg;
}

cv::Mat Histogram::getHistEqualizedImg(cv::Mat img)
{
    cv::Mat equalizedImg(m_height, m_width, CV_8UC3, cv::Scalar(0, 0, 0));

    if (img.channels() > 1)
    {
        cout << "Please pass a gray scale image." << endl;
        return equalizedImg;
    }

    cv::equalizeHist(img, equalizedImg);
    return equalizedImg;
}

int main()
{
    cv::Mat srcImg(cv::imread("../Dataset/image-06.jpg"));

    // Resize and grayscale the image
    cv::cvtColor(srcImg, srcImg, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::resize(srcImg, srcImg, cv::Size(500, 500), 0, 0, cv::InterpolationFlags::INTER_AREA);
    Histogram histObj;

    cv::imshow("Original image", srcImg);
    cv::imshow("Equalized image", histObj.getHistEqualizedImg(srcImg));
    cv::imshow("Original image's histogram", histObj.getHistGraphImg(srcImg));
    cv::imshow("Equalized image's histogram", histObj.getHistGraphImg(histObj.getHistEqualizedImg(srcImg)));

    cv::waitKey(0);

    return 0;
}