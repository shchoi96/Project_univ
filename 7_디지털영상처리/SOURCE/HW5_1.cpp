#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat img, img_gray;
Mat out, out_norm, out_norm_scaled;
int thresh = 100;
void cornerHarris_demo();
void initialize();
void print();

int main(int argc, char** argv)
{
    initialize();
    
    cornerHarris_demo();

    print();
    return(0);
}

// void cornerHarris(InputArray src, OutputArray dst, int blockSize, int ksize, double k, int borderType = BORDER_DEFAULT)
// 1) src : �𼭸� ���� �� gray �̹���
// 2) dst : �𼭸� ���� ��� �̹���
// 3) blcokSize : �̿� ������
// 4) ksize : Sobel()������ ���� Aperture �Ű�����
// 5) k : Harris detector free parameter
// 6) borderType : �ȼ� ���� Ÿ��
void cornerHarris_demo()
{
    out = Mat::zeros(img.size(), CV_32FC1);
    
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    
    // Detecting corners
    cornerHarris(img_gray, out, blockSize, apertureSize, k, BORDER_DEFAULT);

    // Normalize
    normalize(out, out_norm, 0, 255, NORM_MINMAX);
    convertScaleAbs(out_norm, out_norm_scaled);

    // Drawing a circle around corners
    for (int j = 0; j < out_norm.rows; j++) 
        for (int i = 0; i < out_norm.cols; i++)
            if ((int)out_norm.at<float>(j, i) > thresh)
                circle(out_norm_scaled, Point(i, j), 5, Scalar(0), 2);

}

void initialize()
{
    img = imread("harris.png");
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
}

void print()
{
    imshow("Input Image", img);
    imshow("OUtput Image", out_norm_scaled);
    waitKey(0);
}