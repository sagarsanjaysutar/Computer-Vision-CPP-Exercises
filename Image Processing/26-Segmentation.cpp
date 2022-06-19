/*!
 * Image Segmentation using Watershed.
 * @link https://docs.opencv.org/3.4/d2/dbd/tutorial_distance_transform.html
 * @link https://opencv24-python-tutorials.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_watershed/py_watershed.html
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    cv::Mat srcImg(cv::imread("../Dataset/image-10.jpg"));
    if (srcImg.empty())
    {
        cout << "Couldn't open image. Try another one." << endl;
        return -1;
    }

    cv::imshow("Original image", srcImg);

    // Sharpen the image for better foreground object.
    cv::Mat laplacianImg;
    cv::Matx<float, 3, 3> kernel(1, 1, 1,
                                 1, -8, 1,
                                 1, 1, 1);
    cv::filter2D(srcImg, laplacianImg, CV_32F, kernel);
    cv::Mat srcImgFloat;
    srcImg.convertTo(srcImgFloat, CV_32F);         // Converting Matrix to 32F to hold negative value
    cv::Mat sharpImg = srcImgFloat - laplacianImg; // Sharpening
    sharpImg.convertTo(sharpImg, CV_8UC3);         // Coverting Matrix back to normal
    laplacianImg.convertTo(laplacianImg, CV_8UC3);

    // Converting into Grayscale for thresholding purposes. Thresholding helps to estimate objects.
    cv::Mat grayImg;
    cv::cvtColor(srcImg, grayImg, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::threshold(grayImg, grayImg, 40, 255, cv::ThresholdTypes::THRESH_OTSU);

    // Noise reduction
    cv::morphologyEx(
        grayImg,                    // Source
        grayImg,                    // Destination
        cv::MorphTypes::MORPH_OPEN, // Opening = Erosion -> Dilation
        cv::Mat::ones(3, 3, CV_8U), // Kernel
        cv::Point(),                // Anchor ??
        2                           // Higher the iteration number, better the coverage of object.
    );

    // After reduction noise, we need to magnify our objects. Dilation increases object boundaries to a certain level.
    // Beyond that level we can be sure that it's "background"
    cv::Mat background;
    cv::dilate(
        grayImg,                    // Source
        background,                 // Destination
        cv::Mat::ones(3, 3, CV_8U), // Kernel
        cv::Point(),                // Anchor ??
        4                           // Higher the iteration number, better the coverage of object.
    );

    // Applying the distance algorithm followed by thresholding makes the foreground objects definitive.
    cv::Mat distanceImg, foreground, unknownObj;
    cv::distanceTransform(
        background,
        distanceImg,
        cv::DistanceTypes::DIST_L2,
        cv::DistanceTransformMasks::DIST_MASK_3,
        CV_32F);
    cv::normalize(distanceImg, distanceImg, 0, 1, cv::NormTypes::NORM_MINMAX);
    cv::threshold(distanceImg, foreground, 0.5, 1.0, cv::ThresholdTypes::THRESH_BINARY); // Arbituary threshold value
    foreground.convertTo(foreground, CV_8U);                                             // Converting matrix back to normal

    // Contour detection. We find the blobs
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(
        foreground,
        contours,
        cv::RetrievalModes::RETR_EXTERNAL,
        cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    cv::Mat markers(cv::Mat::zeros(foreground.size(), CV_32S));
    // Looping through all the discovered contours (blobs) and drawing it
    for (std::size_t i = 0; i < contours.size(); i++)
    {
        cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i) + 2));
    }

    // Pass the original image and another image with detected-blobs
    cv::watershed(sharpImg, markers);

    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = cv::theRNG().uniform(0, 256);
        int g = cv::theRNG().uniform(0, 256);
        int r = cv::theRNG().uniform(0, 256);

        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    cv::Mat result(cv::Mat::zeros(markers.size(), CV_8UC3));
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i, j);
            if (index > 0 && index <= static_cast<int>(contours.size())) // Can't figure this out.
            {
                result.at<cv::Vec3b>(i, j) = colors[index]; // Same "pixel" values get same "color"
            }
        }
    }

    cv::imshow("Watershed Result", result);
    cv::waitKey(0);

    return 0;
}