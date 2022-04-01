/*!
 * @note Can't figure out the pixel to complex number formula on line 85
 * @brief Implementing MandelBrot set of complex numbers which outputs a beautiful representation.
 * @author Sagar Sutar
 * @link https://docs.opencv.org/3.4/d6/d6d/tutorial_mat_the_basic_image_container.html
 * @link https://www.youtube.com/watch?v=NGMRB4O922I
 *
 * MadelBrot set is a set of complex number who's (z*z + c) value is between <=2.
 * Here we are interested in behaviour of zero under iteration.
 * i.e. given a complex number 'c' and z starting from 0,
 * the output of that will be value of z in next iteration with same c.
 *
 * e.g. Lets take z = 0 and c = 1
 * f(z) = z*z + c
 * f(0) = 0*0 + 1 = 1
 * f(1) = 1*1 + 1 = 2
 * f(2) = 2*2 + 1 = 5
 * f(5) = 5*5 + 1 = 26
 * f(26) = 26*26 + 1.. so on.
 *
 * Clearly c = 1 is not a part of MadelBrot set because the o/p is not under 2.
 *
 * Another example: Lets take z = 0 and c = -1
 * f(z) = z*z + c
 * f(0) = 0*0 + (-1) = -1
 * f(-1) = -1*-1 + (-1) = 0
 * f(0) = 0*0 + (-1) = -1
 * f(-1) = -1*-1 + (-1) = 0
 * f(0) = 0*0 + (-1).. so on.
 *
 * Clearly c = -1 is a part of MadelBrot set because the o/p is under 2.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using std::cout;
using std::endl;

int mandelBrot(const std::complex<float> z0, const int max)
{
    std::complex<float> z = z0;
    for (int i = 0; i < max; i++)
    {
        if (z.real() * z.real() + z.imag() * z.imag() > 4.0f) // why
            return i;
        z = z * z + z0; // main formula
    }

    return max;
}

int mandelBrotFormula(const std::complex<float> z, const int maxIteration = 500)
{
    int value = mandelBrot(z, maxIteration);

    // Pixel is black if it reaches the maximum value
    if (maxIteration - value == 0)
    {
        return 0;
    }

    // If not then appropiate grayscale value would be choosen.
    return cvRound(sqrt(value / (float)maxIteration) * 255);
}

class MandelBrotParallelLoopBody : public cv::ParallelLoopBody
{
public:
    MandelBrotParallelLoopBody(cv::Mat &img, const float x1, const float y1, const float scaledReal, const float scaledImaginary)
        : m_img(img),
          m_x1(x1),
          m_y1(y1),
          m_scaledX(scaledReal),
          m_scaledY(scaledImaginary){};

    /*!
     * Overloading the "operator ()" to put our logic.
     */
    virtual void operator()(const cv::Range &range) const override
    {
        for (int r = range.start; r < range.end; r++)
        {
            // ???
            int i = r / m_img.cols;
            int j = r % m_img.cols;
            float x0 = j / m_scaledX + m_x1;
            float y0 = i / m_scaledY + m_y1;

            std::complex<float> z0(x0, y0);
            uchar value = (uchar)mandelBrotFormula(z0);
            m_img.ptr<uchar>(i)[j] = value;
            break;
        }
    }

private:
    cv::Mat &m_img;
    float m_x1, m_y1, m_scaledX, m_scaledY;
};

int main()
{
    // Declare the MadelBrot set.
    cv::Mat mandelBrotImg(4800, 5400, CV_8U);

    /* Complex number is made up of real and imaginary part. Real part is plotted on x axis and Img. on Y axis.
     * MandelBrot sets are under following threshold.
     * i.e. when you see the diagram on a x-y axis, the shapes are within the following x-y axis values.
     * */
    float x0 = -2.1f, x1 = 0.6f;
    float y0 = -1.2f, y1 = 1.2f;
    float scaleX = mandelBrotImg.cols / (x1 - x0); // Brings the row/col values in a particular range with relation to axis values.
    float scaleY = mandelBrotImg.rows / (y1 - y0);

    MandelBrotParallelLoopBody mandelBrot(mandelBrotImg, x0, y0, scaleX, scaleY);
    // Range is the total no. of operations to be executed.
    // In our case we are exploring all pixels paralley/mutltithreading.
    cv::parallel_for_(cv::Range(0, mandelBrotImg.rows * mandelBrotImg.cols), mandelBrot);
    cv::imwrite("a.png", mandelBrotImg);
    return 0;
}