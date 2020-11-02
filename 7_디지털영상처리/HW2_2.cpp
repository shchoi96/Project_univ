#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("얼굴사진.jpg");
	uchar* pData = img.data;
	int x = img.size().width;
	int y = img.size().height;
	imshow("원본", img);

	int e = 40;
	unsigned char* pOutData;	
	// 2)(1)
	Mat img_out1(y, x, img.type());
	pOutData = (unsigned char*)img_out1.data;
	for (int i = 0; i < x * y; i++)
	{
		uchar B = pData[3 * i + 0];
		uchar G = pData[3 * i + 1];
		uchar R = pData[3 * i + 2];
		// 대표값 (202, 154, 123) : 3.1)에서 구한 값 평균
		// 대표값과 오차 +- 30 사이의 색 선택
		if (abs(B-123) <= e && abs(G-154) <= e && abs(R-202) <= e)
		{
			pOutData[3 * i + 0] = B;
			pOutData[3 * i + 1] = G;
			pOutData[3 * i + 2] = R;
		}
		else
		{
			pOutData[3 * i + 0] = 0;
			pOutData[3 * i + 1] = 0;
			pOutData[3 * i + 2] = 0;
		}
	}
	imshow("3.2)(1)", img_out1);
	
	// 2)(2)
	Mat img_out2(y, x, CV_8UC1);
	pOutData = (unsigned char*)img_out2.data;
	for (int i = 0; i < x * y; i++)
	{
		uchar B = pData[3 * i + 0];
		uchar G = pData[3 * i + 1];
		uchar R = pData[3 * i + 2];
		if (abs(B - 123) <= e && abs(G - 154) <= e && abs(R - 202) <= e)
			pOutData[i] = 255;
		else
			pOutData[i] = 0;
	}
	imshow("3.2)(2)", img_out2);

	waitKey(0);
	return 0;
}