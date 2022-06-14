/*!
    @brief This is a retarted program. I don't see any use/point of this. I've wasted 2 days on this.
    @link https://docs.opencv.org/3.4/da/d7f/tutorial_back_projection.html
 */

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    cv::Mat hue;
    cv::Mat srcImg(cv::imread("../Dataset/image-07.jpg"));

    if (srcImg.empty())
    {
        cout << "Could not open image. Try another one." << endl;
        return -1;
    }

    // Transform image into HSV format for Hue Seperation
    cv::Mat hsv;
    cv::cvtColor(srcImg, hsv, cv::ColorConversionCodes::COLOR_BGR2HSV);

    // Extracting Hue value from "hsv" and adding it to "hue"
    hue.create(hsv.size(), hsv.depth());
    int ch[] = {0, 0}; // Copying from 0th src idx to 0th dest index
    cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);

    // Histogram of the Hue Image
    cv::Mat hist;
    int bin = 15;
    float hue_range[] = {0, 180};
    const float *range[] = {hue_range}; // Array of float pointer
    cv::calcHist(&hue, 1, 0, cv::Mat(), hist, 1, &bin, range);
    cv::normalize(hist, hist, 0, 255, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    // Back Projection
    cv::Mat backProject;
    cv::calcBackProject(&hue, 1, 0, hist, backProject, range, 1, true);

    // Show Back Project
    cv::imshow("Back Projected Image", backProject);
    cv::waitKey(0);

    return 0;
}
