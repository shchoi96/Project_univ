#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

int main()
{
	string name = "lena.jpg";
	Mat img = imread(name);
	cout << "파일이름:  " << name << endl;
	cout << "채   널 :  " << img.channels() << endl;
	cout << "세   로 :  " << img.rows << endl;
	cout << "가   로 :  " << img.cols << endl;
	cout << "총 픽셀 :  " << img.total() << endl;

	int x = img.size().width;
	int y = img.size().height;
	uchar* pData = img.data;
	Mat img_out1(y, x, CV_8UC1);
	Mat img_out2(y, x, CV_8UC1);
	Mat img_out3(y, x, img.type());
	imshow("원본", img);
	// 순서대로 저장
	unsigned char* pOutData = (unsigned char*)img_out1.data;
	for (int i = 0; i < x * y; i++)
		pOutData[i] = pData[i];
	imshow("순서대로", img_out1);
	// 3개 단위 중 하나만 저장
	pOutData = (unsigned char*)img_out2.data;
	for (int i = 0; i < x * y; i++)
		pOutData[i] = pData[i * 3 + 0];
	imshow("3개 단위로", img_out2);

	pOutData = (unsigned char*)img_out3.data;
	for (int i = 0; i < x * y; i++)
	{
		pOutData[i * 3 + 0] = pData[i * 3 + 0];
		pOutData[i * 3 + 1] = 0;
		pOutData[i * 3 + 2] = 0;
	}
	imshow("Blue", img_out3);
	for (int i = 0; i < x * y; i++)
	{
		pOutData[i * 3 + 0] = 0;
		pOutData[i * 3 + 1] = pData[i * 3 + 1];
		pOutData[i * 3 + 2] = 0;
	}
	imshow("Green", img_out3);
	for (int i = 0; i < x * y; i++)
	{
		pOutData[i * 3 + 0] = 0;
		pOutData[i * 3 + 1] = 0;
		pOutData[i * 3 + 2] = pData[i * 3 + 2];
	}
	imshow("Red", img_out3);
	
	waitKey(0);
	return 0;
}

