//
// Created by barna on 4/29/2025.
//

#ifndef LAB8_PROJECT_H
#define LAB8_PROJECT_H
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void nearestNeighborResize(const Mat& src, Mat& dst, double scale_x, double scale_y);

void bilinearResize(const Mat& src, Mat& dst, double scale_x, double scale_y);

float cubicInterpolate(float p[4], float x);

void bicubicResize(const Mat& src, Mat& dst, double scale_x, double scale_y);

void gaussianInterpolation(const Mat& src, Mat& dst, double scale_x, double scale_y, double sigma);

float gaussianKernel(int x, int y, double sigma);

void testResizeFunctions(const Mat& source);

#endif //LAB8_PROJECT_H
