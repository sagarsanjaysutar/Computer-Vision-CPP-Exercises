/*!
 * @file LinearBlending.cpp
 * @brief Blending two images like a slide show effect.
 * @author Sagar Sutar
 * @link https://docs.opencv.org/3.4/d5/dc4/tutorial_adding_images.html
 */

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    std::string img0Path("/home/sagar/Pictures/doge.jpg");
    std::string img1Path("/home/sagar/Pictures/angry-doge.jpg");

    cv::Mat img0, img1, result;
    double alpha = 0.5, beta; // Make sure alpha is between 0-1

    img0 = cv::imread(img0Path); // DType of each pixel is 8uchar
    img1 = cv::imread(img1Path);

    cv::resize(img1, img1, img0.size(), 0, 0, cv::InterpolationFlags::INTER_AREA); // Resizing based on 1st image.

    if (img0.empty() || img1.empty())
    {
        cout << "Invalid image. Try different one." << endl;
        return -1;
    }

    beta = (1.0 - alpha);
    cout << alpha << beta << endl;
    cv::addWeighted(img0, alpha, img1, beta, 0.0, result);

    cv::imshow("Blended image", result);
    cv::waitKey();

    return 0;
}
