#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
/*
    Refer: https://docs.opencv.org/3.4/d7/d37/tutorial_mat_mask_operations.html
*/
void Sharpen(const cv::Mat &img, cv::Mat &result);

int main()
{
    std::string imgPath("/home/sagar/Pictures/a.jpg");
    cv::Mat img, dst0, dst1;
    img = cv::imread(imgPath, cv::ImreadModes::IMREAD_COLOR);

    if (img.empty())
    {
        std::cout << "Failed to open image. Try another one. " << std::endl;
        return -1;
    }
    cv::imshow("Orignal Image", img);

    Sharpen(img, dst0);
    cv::imshow("Sharpened Image with custom function", dst0);
    cv::waitKey();

    // Sharpening Image with a simple Kernel
    cv::Matx<char, 3, 3> kernel(0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(img, dst1, img.depth(), kernel);
    cv::imshow("Sharpened Image with cv::filter2d function", dst1);
    cv::waitKey();

    return 0;
}

// Sharpening Image with a predefined formula
void Sharpen(const cv::Mat &img, cv::Mat &result)
{
    CV_Assert(img.depth() == CV_8U);

    const int nChannels = img.channels();
    result.create(img.size(), img.type());
    for (int i = 1; i < img.rows - 1; ++i)
    {
        // Going through individual row. Pointers to start of each row.
        const uchar *previous = img.ptr<uchar>(i - 1);
        const uchar *current = img.ptr<uchar>(i);
        const uchar *next = img.ptr<uchar>(i + 1);

        uchar *output = result.ptr<uchar>(i);

        for (int j = nChannels; j < nChannels * (img.cols - 1); ++j)
        {
            // Going through individual column, but in "nChannel batches" e.g. in chunk of 3.
            // Refer the doc. to see the formula
            *(output++) = cv::saturate_cast<uchar>(
                5 * current[j] - previous[j] - next[j] - current[j - nChannels] - current[j + nChannels]);
        }
    }
}
