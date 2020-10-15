#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

Mat rgb2yuv(Mat& img_in);

int main()
{
    Mat img_in = imread("Lena.png");
    Mat img_out1, img_out2;
    cout << "Input Image Size  x : " << img_in.size().width << " , y : " << img_in.size().height << endl;
    cout << "dtype : " << img_in.type() << endl;
    // 변환
    img_out1 = rgb2yuv(img_in);
    cvtColor(img_in, img_out2, cv::COLOR_BGR2YCrCb);
    // 출력
    imshow("input image", img_in);
    imshow("my RGB2YCrCb image", img_out1);
    imshow("CV RGB2YCrCb image", img_out2);

    waitKey(0);
    return 0;
}

Mat rgb2yuv(Mat& img_in)
{
    // OpenCV는 YCrCb 순서로 저장
    // ITU-R BT.601
    /*
        Y  =    0.114 * B +   0.587 * G +    0.229 * R
        Cr = -0.08131 * B - 0.41869 * G +      0.5 * R
        Cb =      0.5 * B - 0.33126 * G + -0.16874 * R
    */
    float data[] = {
           0.114,     0.587,    0.229,
        -0.08131, -0.41869f,      0.5,
             0.5,  -0.33126, -0.16874
    };

    int col = img_in.size().width;
    int row = img_in.size().height;
    
    Mat BGR_float;
    Mat img(img_in.size(), img_in.type());
    Mat Value(Size(3, 3), CV_32FC1, data);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            Mat BGR_uchar(img_in.at<Vec3b>(i, j));            
            BGR_uchar.convertTo(BGR_float, CV_32FC1);
            Mat YCrCb = Value * BGR_float; // (Y, Cr, Cb) = (Value) * (B, G, R)

            img.data[3 * j + 3 * col * i + 0] = (uchar)YCrCb.ptr<float>(0)[0];
            img.data[3 * j + 3 * col * i + 1] = (uchar)YCrCb.ptr<float>(0)[1] + 128;
            img.data[3 * j + 3 * col * i + 2] = (uchar)YCrCb.ptr<float>(0)[2] + 128;
        }
    }
    return img;
}


//Mat img(img_in.size(), img_in.type());
//uchar* pData = img_in.data;
//uchar* pOutData = img.data;
//float Kr = 0.299f;
//float Kg = 0.587f;
//float Kb = 0.114f;
//float delta = 128;

//float Y, Cr, Cb;
//for (int i = 0; i < col * row; i++)
//{
//    float R = pData[3 * i + 2];
//    float G = pData[3 * i + 1];
//    float B = pData[3 * i + 0];

//    //Y = Kr * R + Kg * G + Kb * B;
//    //Cr = (R - Y) * 0.713 + delta;
//    //Cb = (B - Y) * 0.564 + delta;
//    Y = 0.114 * B + 0.587 * G + 0.229 * R;
//    Cr = -0.08131 * B - 0.41869 * G + 0.5 * R + delta;
//    Cb = 0.5 * B - 0.33126 * G + -0.16874 * R + delta;


//    pOutData[3 * i + 0] = (uchar)Y;
//    pOutData[3 * i + 1] = (uchar)Cr;
//    pOutData[3 * i + 2] = (uchar)Cb;
//}