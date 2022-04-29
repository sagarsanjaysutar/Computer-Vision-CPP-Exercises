/*!
 * @brief Exploring cv::Mat
 * @author Sagar Sutar
 * @link https://docs.opencv.org/3.4/d6/d6d/tutorial_mat_the_basic_image_container.html
 *
 * Mat is a structure that keeps matrix/image characteristics (rows and columns number, data type etc) and a pointer to data.
 * So nothing prevents us from having several instances of Mat corresponding to the same data. A Mat keeps a
 * reference count that tells if data has to be deallocated when a particular instance of Mat is destroyed.
 */

#include <iostream>
#include <opencv2/core/core.hpp>

int main(int, char **)
{
    cv::Size2i twoDSize(4, 4);
    int threeDSize[3] = {2, 2, 2};

    // 2D Matrix
    cv::Mat A(twoDSize, CV_8U, cv::Scalar_<uint8_t>(10));
    std::cout << A.size() << " sized, 8 bit uint matrix with " << A.channels() << " color channel" << std::endl
              << cv::format(A, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;
    cv::Mat B(twoDSize, CV_32FC1, cv::Scalar_<uint8_t>(96.25874));
    std::cout << B.size() << " sized, 32 bit float matrix with " << B.channels() << " color channel." << std::endl
              << cv::format(B, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;
    cv::Mat C(twoDSize, CV_32FC3, cv::Scalar(1.1, 0.2, 3.2, 1.2));
    std::cout << C.size() << " sized, 32 bit float matrix with " << C.channels() << " color channel." << std::endl
              << cv::format(C, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;

    // 2D Matrix
    cv::Matx<char, 3, 3> D(0, -1, 0, -1, 5, -1, 0, -1, 0);
    std::cout << D.rows << "x" << D.cols << " sized, 32 bit float matrix with " << D.channels << " color channel." << std::endl
              << cv::format(D, cv::Formatter::FMT_PYTHON) << std::endl
              << std::endl;

    // 3D need to figure out
    return 0;
}