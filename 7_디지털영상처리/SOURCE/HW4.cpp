#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

void AddSaltAndPepperNoise(Mat& img, double noise_ratio);
void AddGaussianNoise(Mat& img, Mat& noise, Mat& out);
void SmoothingFiltering(Mat& img, int n, Mat& out);
void MedianFiltering(Mat& img, Mat& out);

int main()
{
	Mat img = imread("lena.png");
	imshow("input", img);

	// Gaussian Noise
	Mat noise_gaussian(img.size(), img.type());
	double average = 0.0;
	double std = 50.0;
	randn(noise_gaussian, Scalar::all(average), Scalar::all(std));
	AddGaussianNoise(img, noise_gaussian, noise_gaussian);
	imshow("Gaussian Noise", noise_gaussian);
	
	// Salt & Pepper Noise
	Mat noise_sp(img);
	double ratio = 0.05; 
	AddSaltAndPepperNoise(noise_sp, ratio);
	imshow("Salt&Pepper Noise", noise_sp);

	// Smoothing Filter를 이용하여 Gaussian Noise 제거
	Mat cancel_gaussian(img.size(), img.type());
	SmoothingFiltering(noise_gaussian, 1, cancel_gaussian); // 3 x 3
	imshow("Cancel Gaussian Noise. 3 x 3", cancel_gaussian);
	SmoothingFiltering(noise_gaussian, 2, cancel_gaussian); // 5 x 5
	imshow("Cancel Gaussian Noise. 5 x 5", cancel_gaussian);

	// 3x3 median filter. salt/pepper noise 영상에 적용(Noise 확률 5%, 10%, 25%)
	Mat cancel_sp(img.size(), img.type());
	MedianFiltering(noise_sp, cancel_sp);
	imshow("Cancel Salt&Pepper Noise", cancel_sp);
	
	waitKey(0);
	return 0;
}

void AddSaltAndPepperNoise(Mat& img, double noise_ratio)
{
	int rows = img.rows;
	int cols = img.cols;
	int ch = img.channels();
	int nNoisePixels = (int)((double)(rows * cols * ch) * noise_ratio);

	uchar* pData = img.data;
	for (int i = 0; i < nNoisePixels; i++)
	{
		int row = rand() % rows;
		int col = rand() % cols;
		int c = rand() % ch;

		pData[row * cols * ch + col * ch + c] = (rand() % 2) * 255;
	}
}

void AddGaussianNoise(Mat& img, Mat& noise, Mat& out)
{
	int rows = img.rows;
	int cols = img.cols;
	int ch = img.channels();

	uchar* pData = img.data;
	uchar* pNoiseData = noise.data;
	uchar* pOutData = out.data;
	for (int i = 0; i < rows * cols * ch; i++)
	{
		pOutData[i] = (int)(pData[i] + pNoiseData[i]) > 255 ? 255 : pData[i] + pNoiseData[i];
	}
}

void SmoothingFiltering(Mat& img, int n, Mat& out)
{
	int rows = img.rows;
	int cols = img.cols;
	int ch = img.channels();
	int size = 2 * n + 1;

	uchar* pOutData = out.data;
	uchar* pData = img.data;
	for (int k = 0; k < rows * cols; k++)
	{
		int col = k % cols;
		int row = k / cols;
		for (int c = 0; c < ch; c++)
		{
			double ret = 0;
			for(int i=-n;i<=n;i++)
				for (int j = -n; j <= n; j++)
				{
					int x = col + i;
					int y = row + j;
					// Mirror Padding
					if (!(0 <= x && x < cols && 0 <= y && y < rows))
					{
						if (x < 0)
							x = abs(x);
						else if (x >= cols)
							x = 2 * cols - x - 2;
						if (y < 0)
							y = abs(y);
						else if (y >= rows)
							y = 2 * rows - y - 2;
					}
					ret += (double)pData[(x + cols * y) * ch + c];
				}
			ret /= (double)(size * size);
			if (ret > 255) ret = 255;
			pOutData[k * ch + c] = ret;
		}
	}
}

void MedianFiltering(Mat& img, Mat& out)
{
	int rows = img.rows;
	int cols = img.cols;
	int ch = img.channels();
	
	vector<uchar> item;
	uchar* pOutData = out.data;
	uchar* pData = img.data;
	for (int k = 0; k < rows * cols; k++)
	{
		int col = k % cols;
		int row = k / cols;
		for (int c = 0; c < ch; c++)
		{
			item.clear();
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					int x = col + i;
					int y = row + j;
					if (0 <= x && x < cols && 0 <= y && y < rows)
					{
						item.push_back(pData[(x + cols * y) * ch + c]);
					}
				}
			sort(item.begin(), item.end());
			pOutData[k * ch + c] = item[item.size()/2];
		}
	}
}