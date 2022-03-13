#include <iostream>
#include <opencv2/core/core.hpp>

int main(int, char **)
{
    cv::Mat M(4, 4, CV_8U);
    std::cout << "M = " << std::endl
              << M << std::endl
              << std::endl;
    return 0;
}