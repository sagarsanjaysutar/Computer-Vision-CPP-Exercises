/*!
 * @brief Brightness and Constrast pixel transformation.
 * @author Sagar Sutar
 * @link https://docs.opencv.org/3.4/d3/dc1/tutorial_basic_linear_transform.html
 *
 * Image transforms can be seen as:
 * - Point operators (pixel transforms)
 * - Neighborhood (area-based) operators
 */

// TODO: Do a gamma correction function, combine brightness, contrast and color reduction into 1 file. Use look up table.

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    std::string imgPath("../Dataset/image-03.jpg");
    cv::Mat img, result;

    img = cv::imread(imgPath); // DType of each pixel is 8uchar
    result.create(img.size(), img.type());

    if (img.empty())
    {
        cout << "Invalid image. Try different one." << endl;
        return -1;
    }

    double alpha = 1.0; // 1-3
    int beta = 50;      // 1-100
    int nChannels = img.channels();

    for (int row = 0; row < img.rows; ++row)
    {
        const uchar *current = img.ptr<uchar>(row);
        uchar *resultRow = result.ptr<uchar>(row);

        // Flattened cols i.e. iterating through column * channel. Refer link for formula.
        for (int col = nChannels; col < (img.cols - 1) * nChannels; ++col)
        {
            *(resultRow++) = cv::saturate_cast<uchar>(alpha * current[col] + beta);
        }
    }
    cv::imshow("Original", img);
    cv::imshow("Brightened", result);
    cv::waitKey();

    return 0;
}
