#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main()
{

    cv::Mat srcImg(cv::imread("../Dataset/image-09.png"));
    if (srcImg.empty())
    {
        cout << "Couldn't open image. Try another one." << endl;
        return -1;
    }

    cv::imshow("Original image", srcImg);

    // 00. Applying filter to sharpen image
    cv::Mat laplacianImg;
    cv::Matx<float, 3, 3> kernel(1, 1, 1,
                                 1, -8, 1,
                                 1, 1, 1);
    cv::filter2D(srcImg, laplacianImg, CV_32F, kernel);

    // 01. Converting Matrix to 32F to hold negative value
    cv::Mat srcImgFloat;
    srcImg.convertTo(srcImgFloat, CV_32F);
    cv::Mat sharpImg = srcImgFloat - laplacianImg; // Sharpening

    // 02. Coverting Matrix back to 8U
    sharpImg.convertTo(sharpImg, CV_8UC3);
    laplacianImg.convertTo(laplacianImg, CV_8UC3);

    // 03. Converting into Grayscale and thresholding to focus on ROI
    cv::Mat grayImg;
    cv::cvtColor(srcImg, grayImg, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(
        grayImg,
        grayImg,
        255,
        cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C,
        cv::ThresholdTypes::THRESH_BINARY,
        11,
        0);
    // cv::imshow("Threshold Image", grayImg);

    // 04. Applying distance algorithm
    cv::Mat distanceImg;
    cv::distanceTransform(grayImg, distanceImg, cv::DistanceTypes::DIST_L2, 3);

    cv::normalize(distanceImg, distanceImg, 0, 1, cv::NormTypes::NORM_MINMAX);
    cv::imshow("Distance Image", distanceImg);

    cv::waitKey(0);

    return -1;
}