#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <cmath> // sin, cos

using namespace cv;
using namespace std;

Mat Resize(int x, int y, Mat img_in);
Mat Rotate(double theta, Mat img_in);

const double PI = 3.14159265;

int main()
{
	// 초기화 및 전처리
	string out_name;
	Mat img_in, img_out;
	img_in = imread("Lena.png");					// 이미지 읽기
	cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);	// 회색으로 변환
	
	int s;
	cout << "사이즈 변환 : 1\n영상 회전 : 2\n입력 : ";
	cin >> s;
	cout << endl;

	switch (s)
	{
	case 1: // 사이즈 변환
		int x, y;
		cout << "크기 : ";
		cin >> x >> y;
		cout << endl;
		img_out = Resize(x, y, img_in);
		out_name = "Lena" + to_string(img_out.size().width) + "x" + to_string(img_out.size().height) + ".png";

		break;
	case 2: // 영상 회전
		int theta;
		cout << "회전각 : ";
		cin >> theta;
		cout << endl;
		img_out = Rotate(theta, img_in);
		out_name = "LenaTurn" + to_string(theta) + ".png";
		break;
	}

	// 출력
	imshow("input image", img_in);
	cout << "Input Image Size  x : " << img_in.size().width << " , y : " << img_in.size().height << endl;
	imshow("output image", img_out);
	imwrite(out_name, img_out);

	waitKey(0);

	return 0;
}

Mat Resize(int x, int y, Mat img_in)
{
	Mat img(y, x, CV_8UC1); // 행, 열, Type 순서 주의
	unsigned char* pOutData = (unsigned char*)img.data;
	unsigned char* pData = (unsigned char*)img_in.data;
	
	int x_in = img_in.size().width;
	int y_in = img_in.size().height;
	// Bilinear Interpolation
	for (int i = 0; i < x * y; i++)
	{
		// 각 픽셀의 2차원 상의 좌표
		int col = i % x;
		int raw = i / x;
		// 원본 이미지 상의 좌표
		double x_BI = col * (double)x_in / (double)x; 
		double y_BI = raw * (double)y_in / (double)y;
		// 비율
		double p = x_BI - (int)x_BI;
		double q = 1 - p;
		double alpha = y_BI - (int)y_BI;
		double beta = 1 - alpha;
		// Bilinear Interpolation
		pOutData[i] = q * beta * pData[(int)x_BI + (int)y_BI * x_in] 
			+ q * alpha * pData[(int)x_BI + ((int)y_BI + 1) * x_in]
			+ p * beta * pData[(int)(x_BI + 1) + (int)y_BI * x_in] 
			+ p * alpha * pData[(int)(x_BI + 1) + (int)(y_BI + 1) * x_in];
	}
	cout << "Output Image Size  x : " << x << " , y : " << y << endl;
	return img;
}

Mat Rotate(double theta, Mat img_in)
{
	int x = img_in.size().width;
	int y = img_in.size().height;
	Mat img(y, x, CV_8UC1);
	unsigned char* pOutData = (unsigned char*)img.data;
	unsigned char* pData = (unsigned char*)img_in.data;
	
	int mid_x = x / 2;
	int mid_y = y / 2;
	cout << "Rotate " << theta << " Degree" << endl;
	// radian 에서 degree로 변환
	theta = -theta * CV_PI / 180.0;
	for (int i = 0; i < x * y; i++)
	{
		// 각 픽셀의 2차원 상의 좌표
		int col = i % x;
		int raw = i / x;
		// 이미지의 중심을 축으로 한 theta degree 회전
		// X = R(-theta)*Y : Y는 출력 이미지 좌표, X는 입력 이미지 좌표
		col -= mid_x;
		raw -= mid_y;
		int rot_col = (cos(theta) * col - sin(theta) * raw);
		int rot_raw = (sin(theta) * col + cos(theta) * raw);
		rot_col += mid_x;
		rot_raw += mid_y;
		// Boundary Check 및 대입
		if (0 <= rot_col && rot_col < x && 0 <= rot_raw && rot_raw < y)
			pOutData[i] = pData[rot_col + rot_raw * x];
		else
			pOutData[i] = 255;
	}
	return img;
}


// 느낀점 : 원본 이미지의 픽셀을 회전시킨 위치를 찾으면 중복된 위치를 찾게되어 출력 이미지에 빈 공간이 생긴다.
// 하지만 역으로 출력 이미지의 한 픽셀이 원본 이미지의 어떤 픽셀과 대응되는지 찾게되면 출력 이미지 중간에 빈 공간이 생기는 것을 방지할 수 있다.