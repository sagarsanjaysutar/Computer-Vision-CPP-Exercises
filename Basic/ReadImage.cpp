#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    /* Impletement this if you want to pass image as an argument.
    if (argc != 2)
    {
        printf("Provide an image path please. e.g.: ./app /home/root/basic.png \n");
        return -1;
    }*/
    std::string imgPath("/home/sagar/Pictures/doge.jpg");
    cv::Mat image;
    image = cv::imread(imgPath, 1);

    if (!image.data)
    {
        printf("No image data");
        return -1;
    }
    cv::Size imageSize = image.size();
    std::cout << "Size: " << imageSize.height << " - " << imageSize.width << std::endl;
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);

    cv::waitKey(0);

    return 0;
}
