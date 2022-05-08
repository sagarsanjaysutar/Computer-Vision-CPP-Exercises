/*!
 * @brief Draws a simple circle with background.
 * @link https://docs.opencv.org/3.4/d3/d96/tutorial_basic_geometric_drawing.html
 * @link https://docs.opencv.org/3.4/df/d61/tutorial_random_generator_and_text.html
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout;
using std::endl;

class Canvas
{
public:
    explicit Canvas(int width);
    ~Canvas();

    Mat m_image;             // Image Matrix
    void fillBackground();   // Fills up the background
    void drawRandomCirles(); // Draws random circle
    void drawRandomLines();  // Draws random lines

private:
    RNG m_rng;               // Random number generator
    int m_width;             // Width of image
    Scalar getRandomColor(); // Generates random color.
};

Canvas::Canvas(int width) : m_rng(1),
                            m_width(width),
                            m_image(width, width, CV_8UC3)
{
    cout << "Contructor called" << endl;
}

Canvas::~Canvas()
{
    cout << "Destructor called" << endl;
}

Scalar Canvas::getRandomColor()
{
    int icolor = (unsigned)m_rng;
    return Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

void Canvas::fillBackground()
{
    rectangle(m_image,
              Point(0, 0),             // Start point
              Point(m_width, m_width), // End point
              Scalar(161, 113, 136),   // Color, Mauve
              LineTypes::FILLED,       // Thickness or Filled
              LineTypes::LINE_4);      // Line Type
}

void Canvas::drawRandomCirles()
{
    for (int i = 0; i < 20; i++)
    {
        int randomNumber = m_rng.uniform(0, m_width);
        circle(
            m_image,
            Point(randomNumber, randomNumber), // Center
            randomNumber / 5,                  // Radius
            getRandomColor(),                  // Color, Grey
            m_rng.uniform(-1, 4),              // -1 fills the circle or else gives thickness to its border.
            LineTypes::LINE_AA                 // Line Type
        );
    }
}

void Canvas::drawRandomLines()
{
    Point start, end;
    for (int i = 0; i < 32; i++)
    {
        // Generates a random point from a gaussian distribution from 0, sigma.
        start.x = m_rng.gaussian(m_width);
        start.y = m_rng.gaussian(m_width);
        end.x = m_rng.gaussian(m_width);
        end.y = m_rng.gaussian(m_width);

        line(
            m_image,             // Image
            start,               // Start point
            end,                 // End point
            getRandomColor(),    // A random color
            m_rng.uniform(1, 7), // Random thickness of line
            LineTypes::LINE_AA   // Smooth line type
        );
    }
}

int main()
{
    Canvas canvas(500);

    canvas.fillBackground();
    canvas.drawRandomLines();
    canvas.drawRandomCirles();

    imshow("Canvas", canvas.m_image);
    waitKey(0);
    return 0;
}