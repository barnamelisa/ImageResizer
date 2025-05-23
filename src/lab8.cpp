//
// Created by barna on 4/29/2025.
//

#include "lab8.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

// INFO:
    // static_cast<int> e folosit ca sa transformam un numar real in intreg
    // am nevoie de indexi intregi ca sa accesez pixelii din matrice

void nearestNeighborResize(const Mat& src, Mat& dst, double scale_x, double scale_y) {

    // calc noile dimensiuni ale imagii, practic factorul de redimensionare il dau ca si parametru pentru
    // scale_x si scale_y ca sa pot calcula dimensiunea noii imagini
    int new_width = static_cast<int>(src.cols * scale_x);
    int new_height = static_cast<int>(src.rows * scale_y);

    // dupa care initializez matricea destinatie cu noile dimensiuni
    dst = Mat(new_height, new_width, src.type());

    // acum parcurg pixel cu pixel noua imagine
    // merg INVERS, adica prima data parcurg imaginea noua si dupa merg la cea veche
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // pentru fiecare pixel calculez pozitia din imaginea anterioata
            int orig_x = static_cast<int>(round(x / scale_x)); // calc coloana pixelului corespunzator din original
            int orig_y = static_cast<int>(round(y / scale_y)); // calc linia (randul) pixelului corespunzator din original

            // ma asiguri ca nu depasesc cadranul imaginii
            // daca x = 19 in imaginea marita (20 pixeli), orig_x = round(19 / 2.0) = 10
            // dar imaginea originala are index maxim 9 → risc sa ies din matrice
            // de asta folosesc min(orig_x, cols - 1) ca sa raman in limitele imaginii
            orig_x = min(orig_x, src.cols - 1);
            orig_y = min(orig_y, src.rows - 1);

            // setez noua val a pixelului in imaginea redimensionata
            dst.at<uchar>(y, x) = src.at<uchar>(orig_y, orig_x);
        }
    }
}

void bilinearResize(const Mat& src, Mat& dst, double scale_x, double scale_y) {
    // calculez noile dimensiuni in functie de factorii de scalare
    int new_width = static_cast<int>(src.cols * scale_x);
    int new_height = static_cast<int>(src.rows * scale_y);

    // initializez imaginea destinatie cu dimensiunile noi
    dst = Mat(new_height, new_width, src.type());

    // parcurg fiecare pixel din imaginea redimensionata
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // aflu pozitia reala corespunzatoare in imaginea originala
            float gx = x / scale_x;
            float gy = y / scale_y;

            // calculez coltul stanga sus al patratului din care fac media
            int x1 = static_cast<int>(gx);
            int y1 = static_cast<int>(gy);

            // calculez coltul dreapta jos, fara sa ies din matrice
            int x2 = std::min(x1 + 1, src.cols - 1);
            int y2 = std::min(y1 + 1, src.rows - 1);

            // calculez distantele de la punctul real pana la x1 si y1
            float dx = gx - x1;
            float dy = gy - y1;

            // extrag cei 4 pixeli vecini
            uchar p1 = src.at<uchar>(y1, x1); // stanga sus
            uchar p2 = src.at<uchar>(y1, x2); // dreapta sus
            uchar p3 = src.at<uchar>(y2, x1); // stanga jos
            uchar p4 = src.at<uchar>(y2, x2); // dreapta jos

            // aplic formula de interpolare biliniara
            // practic fac o medie ponderata a celor 4 pixeli vecini in functie de distanta fata de punctul rea
            float value = (1 - dx) * (1 - dy) * p1 +
                          dx * (1 - dy) * p2 +
                          (1 - dx) * dy * p3 +
                          dx * dy * p4;

            // salvez rezultatul in imaginea finala
            dst.at<uchar>(y, x) = static_cast<uchar>(value);
        }
    }
}

float cubicInterpolate(float p[4], float x) {
    // formula de interpolare cubica o folosesc pentru a calcula valoarea unui pixel
    // pe baza valorilor celor patru pixeli vecini
    return p[1] + 0.5f * x * (p[2] - p[0] +
                              x * (2.0f*p[0] - 5.0f*p[1] + 4.0f*p[2] - p[3] +
                                   x * (3.0f*(p[1] - p[2]) + p[3] - p[0])));
}

void bicubicResize(const Mat& src, Mat& dst, double scale_x, double scale_y) {
    // calculez noile dimensiuni ale imaginii dupa redimensionare
    int new_width = static_cast<int>(src.cols * scale_x);
    int new_height = static_cast<int>(src.rows * scale_y);

    // creez o noua imagine cu noile dimensiuni
    dst = Mat(new_height, new_width, src.type());

    // parcurg fiecare pixel al imaginii redimensionate
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // calculez coordonatele reale in imaginea sursa
            float gx = x / scale_x;
            float gy = y / scale_y;

            // pozitia pixelului in imaginea sursa la care vreau sa aplic interpolarea
            int x_int = static_cast<int>(gx);
            int y_int = static_cast<int>(gy);

            // diferenta fata de coordonatele intregi pt a face interpolare
            float dx = gx - x_int;
            float dy = gy - y_int;

            // folosesc 4 valori de pixeli pentru interpolare pe coloana
            float col[4];
            for (int i = -1; i <= 2; ++i) {
                float p[4];
                for (int j = -1; j <= 2; ++j) {
                    // calc indecsii valizi pentru imagine
                    int xi = min(max(x_int + j, 0), src.cols - 1);
                    int yi = min(max(y_int + i, 0), src.rows - 1);

                    // si aici salvez valorile pixelilor pt interpolare
                    p[j + 1] = static_cast<float>(src.at<uchar>(yi, xi));
                }
                // aplic interpolarea cubica pe fiecare coloana
                col[i + 1] = cubicInterpolate(p, dx);
            }
            // aplic interpolarea cubica pe rezultate pt a obtine valoarea finala
            float value = cubicInterpolate(col, dy);
            // ma asigur ca valoarea pixelului este in intervalul 0-255
            dst.at<uchar>(y, x) = static_cast<uchar>(min(max(value, 0.0f), 255.0f));
        }
    }
}

// functia pt calcularea valorii kernelului Gaussian
float gaussianKernel(int x, int y, double sigma) {
    // functia gaussiana o folosesc pt a calcula greutatea fiecarui pixel pe baza distantei fata de punctul central
    return exp(-(x * x + y * y) / (2 * sigma * sigma));
}

void gaussianInterpolation(const Mat& src, Mat& dst, double scale_x, double scale_y, double sigma) {
    int new_width = static_cast<int>(src.cols * scale_x);
    int new_height = static_cast<int>(src.rows * scale_y);

    dst = Mat(new_height, new_width, src.type());

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // calculez coordonatele reale
            float gx = x / scale_x;
            float gy = y / scale_y;

            // calc pozitia intrega a pixelului in imaginea sursa
            int x_int = static_cast<int>(gx);
            int y_int = static_cast<int>(gy);

            // parametrii pt a ma asigura ca nu depasesc limitele imaginii
            // acel + sau -1 este de fapt raza zonei vecine si momentan inseamna ca folosesc un kernel de 3x3, deci daca
            // modific aceste valori pot sa extind sau sa restrang zona
            // regula este sa foloesc - la start_x si start_y si + la end_x si end_y, pt ca altfel imaginea va fi
            // distorsionata sau uniforma => interpolare gresita

            // practic daca in loc de 1 folosesc 3 sau alta cifra, practic cresc zona de vecini de la 3x3 la 7x7
            // deci pixelul nou devine o medie ponderata mai mare
            // imaginea poate deveni mai neteda sau mai blurata, adica detaliile vor fii intinse mai mult, dar
            // se poate pierde si din claritate la text
            int start_x = std::max(x_int - 1, 0);
            int end_x = std::min(x_int + 1, src.cols - 1);
            int start_y = std::max(y_int - 1, 0);
            int end_y = std::min(y_int + 1, src.rows - 1);

            float weighted_sum = 0.0f;
            float total_weight = 0.0f;

            // calculez suma ponderata a pixelilor vecini
            for (int dy = start_y; dy <= end_y; ++dy) {
                for (int dx = start_x; dx <= end_x; ++dx) {
                    // calc greutatea fiecarui pixel folosind functia gaussiana
                    float weight = gaussianKernel(dx - x_int, dy - y_int, sigma);
                    weighted_sum += src.at<uchar>(dy, dx) * weight;
                    total_weight += weight;
                }
            }

            // calculez valoarea finala a pixelului
            dst.at<uchar>(y, x) = static_cast<uchar>(weighted_sum / total_weight);
        }
    }
}

// functie pt calculul RMSE
double computeRMSE(const Mat& img1, const Mat& img2) {
    Mat diff;
    absdiff(img1, img2, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    Scalar sumSq = sum(diff);
    double rmse = sqrt(sumSq[0] / (img1.total()));
    return rmse;
}

void testResizeFunctions(const Mat& source) {
    // un vector cu factorii de scalare pe care ii testam
    vector<double> scale_factors = {0.5, 1.0, 2.0};

    // un vector cu valorile sigma folosite la interpolarea Gaussian ca sa vedem cum se comporat pt fiecare factor
    // cu cat factorul sigma este mai mare cu atat imaginea este mai neclara
    vector<double> sigma_values = {0.5, 1.0, 2.0};

    // pt fiecare factor de scalare testez toate metodele de redimensionare
    for (double scale_factor : scale_factors) {
        // timpii de exectutie pentru fiecare metoda in functie de factorii respectivi vor aparea in terminal
        cout << "----------------------------------------" << endl;
        cout << "Testare pentru scale factor = " << scale_factor << endl;

        // matrici pentru imaginile rezultate pt cele 3 metode, deoarece pt gausiana vom avea alt for deoarece vom testa
        // pe 3 valori ale parametrului sigma
        Mat resized_nn, resized_bilinear, resized_bicubic;

        // add comentariu
        Mat ocv_nn, ocv_bilinear, ocv_bicubic;

        int start, end;
        double duration;

        // -------------Nearest Neighbor Interpolation--------------

        // getTickCount()-este o metoda din OpenCV care masoara unitatile de timp interne de cand a inceput contorul de timp al sistemului
        // pe scurt, cu ajutorul acestei functii incep sa masor timpul de executie
        start = getTickCount();

        // aici apelez metoda de interpolare a celui mai apropiat vecin implementata mai sus pt a doua faza a proiectului
        nearestNeighborResize(source, resized_nn, scale_factor, scale_factor);

        // getTickCount-obtin nr de timpi interni la finalul functiei de redimensionare
        end = getTickCount();

        // calculez timpul de executie in secunde al algoritmului, unde:
        // (end - start) este nr de timpi in timpul executiei
        // getTickFrequency()-returneaza cati timpi are un secunda, deci impartind obtin timpul in secunde
        duration = (end - start) / getTickFrequency();
        cout << "Custom NN time: " << duration << " sec" << endl;

        resize(source, ocv_nn, Size(), scale_factor, scale_factor, INTER_NEAREST);
        double psnr_nn = PSNR(resized_nn, ocv_nn);
        double rmse_nn = computeRMSE(resized_nn, ocv_nn);
        cout << "NN PSNR vs OpenCV: " << psnr_nn << " dB | RMSE: " << rmse_nn << endl;

        imwrite("nn_custom_" + to_string(scale_factor) + ".png", resized_nn);
        imwrite("nn_opencv_" + to_string(scale_factor) + ".png", ocv_nn);

        // ---------- Bilinear ----------
        start = getTickCount();
        bilinearResize(source, resized_bilinear, scale_factor, scale_factor);
        end = getTickCount();
        duration = (end - start) / getTickFrequency();
        cout << "Custom Bilinear time: " << duration << " sec" << endl;

        resize(source, ocv_bilinear, Size(), scale_factor, scale_factor, INTER_LINEAR);
        double psnr_bilinear = PSNR(resized_bilinear, ocv_bilinear);
        double rmse_bilinear = computeRMSE(resized_bilinear, ocv_bilinear);
        cout << "Bilinear PSNR vs OpenCV: " << psnr_bilinear << " dB | RMSE: " << rmse_bilinear << endl;

        imwrite("bilinear_custom_" + to_string(scale_factor) + ".png", resized_bilinear);
        imwrite("bilinear_opencv_" + to_string(scale_factor) + ".png", ocv_bilinear);

        // ---------- Bicubic ----------
        start = getTickCount();
        bicubicResize(source, resized_bicubic, scale_factor, scale_factor);
        end = getTickCount();
        duration = (end - start) / getTickFrequency();
        cout << "Custom Bicubic time: " << duration << " sec" << endl;

        resize(source, ocv_bicubic, Size(), scale_factor, scale_factor, INTER_CUBIC);
        double psnr_bicubic = PSNR(resized_bicubic, ocv_bicubic);
        double rmse_bicubic = computeRMSE(resized_bicubic, ocv_bicubic);
        cout << "Bicubic PSNR vs OpenCV: " << psnr_bicubic << " dB | RMSE: " << rmse_bicubic << endl;

        imwrite("bicubic_custom_" + to_string(scale_factor) + ".png", resized_bicubic);
        imwrite("bicubic_opencv_" + to_string(scale_factor) + ".png", ocv_bicubic);

        // ---------- Gaussian ----------
        for (double sigma : sigma_values) {
            Mat resized_gauss;
            start = getTickCount();
            gaussianInterpolation(source, resized_gauss, scale_factor, scale_factor, sigma);
            end = getTickCount();
            duration = (end - start) / getTickFrequency();
            cout << "Gaussian time (sigma=" << sigma << "): " << duration << " sec" << endl;
            imwrite("gaussian_scale_" + to_string(scale_factor) + "_sigma_" + to_string(sigma) + ".png", resized_gauss);

            // Comparatie cu OpenCV: blur + resize bilinear (echivalent aproximativ) pt ca opencv nu are flag pt interpolarea gaussiana
            Mat ocv_blurred, ocv_simulated;
            GaussianBlur(source, ocv_blurred, Size(0, 0), sigma);
            resize(ocv_blurred, ocv_simulated, Size(), scale_factor, scale_factor, INTER_LINEAR);

            // Calculeaza metricile
            double psnr = PSNR(resized_gauss, ocv_simulated);
            double rmse = computeRMSE(resized_gauss, ocv_simulated);
            cout << "PSNR Gaussian vs OpenCV simulation: " << psnr << endl;
            cout << "RMSE Gaussian vs OpenCV simulation: " << rmse << endl;
        }
    }
}
