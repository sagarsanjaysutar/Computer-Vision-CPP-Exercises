#include <iostream>
#include <opencv2/core/core.hpp>

int main(int, char **)
{
    cv::Size2i twoDSize(4, 4);
    int threeDSize[3] = {2, 2, 2};

    // 2D Matrix
    cv::Mat A(twoDSize, CV_8U, cv::Scalar_<uint8_t>(10));
    std::cout << A.size() << " sized, 8 bit uint matrix." << std::endl
              << cv::format(A, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;
    cv::Mat B(twoDSize, CV_32FC1, cv::Scalar_<uint8_t>(96.25874));
    std::cout << B.size() << " sized, 32 bit float matrix with 1 color channel." << std::endl
              << cv::format(B, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;
    cv::Mat C(twoDSize, CV_32FC4, cv::Scalar(1.1, 0.2, 0.2, 0.2));
    std::cout << C.size() << " sized, 32 bit float matrix with 4 color channel." << std::endl
              << cv::format(C, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;

    // 3D Matrix
    cv::Mat D(3, threeDSize, CV_32FC4, cv::Scalar(1.1, 0.2, 0.2, 0.2));
    std::cout << D.size() << "sized, 32 bit float matrix with 4 color channel." << std::endl;

    for (int row = 0; row < (D.rows - 1); row++)
    {
        cv::Mat DSlice(row, D.cols, CV_32FC4, D.ptr());
        std::cout << cv::format(DSlice, cv::Formatter::FMT_PYTHON) << std::endl
                  << std::endl;
    }
    return 0;
}