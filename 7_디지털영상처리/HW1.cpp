#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath> // sin, cos

using namespace cv;
using namespace std;

#define PI 3.14159265

Mat Resize(int x, int y, Mat img_in);
Mat Rotate(double theta, Mat img_in);

int main()
{
	// �ʱ�ȭ �� ��ó��
	Mat img_out;
	Mat img_in;
	img_in = imread("Lena.png");					// �̹��� �б�
	cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);	// ȸ������ ��ȯ
	
	int s;
	cout << "������ ��ȯ : 1\n���� ȸ�� : 2\n�Է� : ";
	cin >> s;
	cout << endl;

	switch (s)
	{
	case 1: // ������ ��ȯ
		int x, y;
		cout << "ũ�� : ";
		cin >> x >> y;
		cout << endl;
		img_out = Resize(x, y, img_in);
		break;
	case 2: // ���� ȸ��
		int theta;
		cout << "ȸ���� : ";
		cin >> theta;
		cout << endl;
		img_out = Rotate(theta, img_in);
		break;
	}

	// ���
	imshow("input image", img_in);
	cout << "Input Image Size  x : " << img_in.size().width << " , y : " << img_in.size().height << endl;
	imshow("output image", img_out);
	
	waitKey(0);

	return 0;
}

Mat Resize(int x, int y, Mat img_in)
{
	Mat img(y, x, CV_8UC1); // ��, ��, Type ���� ����
	unsigned char* pOutData = (unsigned char*)img.data;
	unsigned char* pData = (unsigned char*)img_in.data;
	
	int x_in = img_in.size().width;
	int y_in = img_in.size().height;
	// Bilinear Interpolation
	for (int i = 0; i < x * y; i++)
	{
		// �� �ȼ��� 2���� ���� ��ǥ
		int col = i % x;
		int raw = i / x;
		// ���� �̹��� ���� ��ǥ
		double x_BI = col * (double)x_in / (double)x; 
		double y_BI = raw * (double)y_in / (double)y;
		// ����
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
	// radian ���� degree�� ��ȯ
	theta = -theta * CV_PI / 180.0;
	for (int i = 0; i < x * y; i++)
	{
		// �� �ȼ��� 2���� ���� ��ǥ
		int col = i % x;
		int raw = i / x;
		// �̹����� �߽��� ������ �� theta degree ȸ��
		// X = R(-theta)*Y : Y�� ��� �̹��� ��ǥ, X�� �Է� �̹��� ��ǥ
		col -= mid_x;
		raw -= mid_y;
		int rot_col = (cos(theta) * col - sin(theta) * raw);
		int rot_raw = (sin(theta) * col + cos(theta) * raw);
		rot_col += mid_x;
		rot_raw += mid_y;
		// Boundary Check �� ����
		if (0 <= rot_col && rot_col < x && 0 <= rot_raw && rot_raw < y)
			pOutData[i] = pData[rot_col + rot_raw * x];
		else
			pOutData[i] = 255;
	}
	cout << "Rotate " << theta << " Degree" << endl;
	return img;
}


// ������ : ���� �̹����� �ȼ��� ȸ����Ų ��ġ�� ã���� �ߺ��� ��ġ�� ã�ԵǾ� ��� �̹����� �� ������ �����.
// ������ ������ ��� �̹����� �� �ȼ��� ���� �̹����� � �ȼ��� �����Ǵ��� ã�ԵǸ� ��� �̹��� �߰��� �� ������ ����� ���� ������ �� �ִ�.