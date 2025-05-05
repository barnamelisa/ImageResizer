#include <opencv2/opencv.hpp>
#include "src/project.h"
using namespace std;
using namespace cv;

int main() {

    Mat source = imread("C:\\Users\\barna\\OneDrive\\Desktop\\an3-sem2\\pi\\lab\\laboratoare\\lab8\\Image_Processing_UTCN_Labs\\images\\balloons.bmp",IMREAD_GRAYSCALE);
    imshow("Original Image", source);

    Mat resized_img;
    double scale_factor = 0.5; // redimensionare la 100%
    nearestNeighborResize(source, resized_img, scale_factor, scale_factor);
    imshow("Nearest Neighbor Resize", resized_img);

    Mat resized_bilinear;
    bilinearResize(source, resized_bilinear, scale_factor, scale_factor);
    imshow("Bilinear Resize", resized_bilinear);

    Mat resized_bicubic;
    bicubicResize(source,resized_bicubic,scale_factor,scale_factor);
    imshow("Bicubic Resize", resized_bicubic);

    // pentru parametrul sigma- daca este mai mic intre 0.5 si 1 atunci rezultatul va fi mai clar si daca este mai mare
    // atunci pixelii mai indepartati vor influenta, adica vor adaga o forma de estompare usoara
    Mat resized_gauss;
    double sigma=1.0;
    gaussianInterpolation(source,resized_gauss,scale_factor,scale_factor,sigma);
    imshow("Gussian Resize", resized_bicubic);

    waitKey();

    return 0;
}