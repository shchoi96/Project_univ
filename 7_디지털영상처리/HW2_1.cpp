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
	cout << "�����̸�:  " << name << endl;
	cout << "ä   �� :  " << img.channels() << endl;
	cout << "��   �� :  " << img.rows << endl;
	cout << "��   �� :  " << img.cols << endl;
	cout << "�� �ȼ� :  " << img.total() << endl;

	int x = img.size().width;
	int y = img.size().height;
	uchar* pData = img.data;
	Mat img_out1(y, x, CV_8UC1);
	Mat img_out2(y, x, CV_8UC1);
	Mat img_out3(y, x, img.type());
	imshow("����", img);
	// ������� ����
	unsigned char* pOutData = (unsigned char*)img_out1.data;
	for (int i = 0; i < x * y; i++)
		pOutData[i] = pData[i];
	imshow("�������", img_out1);
	// 3�� ���� �� �ϳ��� ����
	pOutData = (unsigned char*)img_out2.data;
	for (int i = 0; i < x * y; i++)
		pOutData[i] = pData[i * 3 + 0];
	imshow("3�� ������", img_out2);

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

