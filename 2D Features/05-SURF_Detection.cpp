
#include "opencv2/core.hpp"
#include <iostream>

using std::cout;
using std::endl;

#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

int main()
{
    cv::Mat srcImg(cv::imread("../Dataset/image-06.jpg"));
    if (srcImg.empty())
    {
        cout << "Failed to read the image. Please try a different one. \t" << endl;
        return -1;
    }

    

    return 0;
}
#else
int main()
{
    cout << "You need the OpenCV contrib modules to be able to use the SURF features (alternatives are ORB, KAZE, ... features)." << endl;
    return 0;
}
#endif